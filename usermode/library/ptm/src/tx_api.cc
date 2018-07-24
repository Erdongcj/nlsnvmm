#include <stdio.h>
#include <cstdlib>
#include <malloc.h>
#include <sys/time.h>
#include <assert.h>
#include <numa.h>
#include "atomic.h"
#include "nvm_emulate.h"
#include "tx_api.h"

uint64_t init_clock;
static __thread threadID_t *currthreadID;
#define MAX_OP_NUM 2*1024*1024
#define MAX_LOCK_NUM 2*1024*1024
#define MAX_RA_NUM 64
#define MAX_ADDR_NUM 2*1024*1024
#define MAX_NUM 1024
static Transaction* get_tx()
{
	static __thread Transaction tx;
	return &tx;
}


void init_thread(threadID_t* threadID, int flag)
{   
	bfalloc_init();
	threadID->buffer = create_buffer(flag);
	if(flag == 1 ){
		threadID->addr_set.entries = (addr_entry*)malloc(MAX_ADDR_NUM * sizeof(addr_entry));
		threadID->addr_set.nb_entries = 0;
		threadID->r_set.entries = (r_entry*)malloc(MAX_RA_NUM * sizeof(r_entry));
		threadID->r_set.nb_entries = 0;
		threadID->l_set.entries = (l_entry*)malloc(MAX_LOCK_NUM * sizeof(l_entry));
		threadID->l_set.nb_entries = 0;
		threadID->op_set.entries = (op_entry*)malloc(MAX_OP_NUM * sizeof(op_entry));
		threadID->op_set.nb_entries = 0;
	}else{
		threadID->addr_set.entries = (addr_entry*)malloc(MAX_NUM * sizeof(addr_entry));
		threadID->addr_set.nb_entries = 0;
		threadID->r_set.entries = (r_entry*)malloc(MAX_NUM * sizeof(r_entry));
		threadID->r_set.nb_entries = 0;
		threadID->l_set.entries = (l_entry*)malloc(MAX_NUM * sizeof(l_entry));
		threadID->l_set.nb_entries = 0;
		threadID->op_set.entries = (op_entry*)malloc(MAX_NUM * sizeof(op_entry));
		threadID->op_set.nb_entries = 0;
	}
	ssalloc_init();
	threadID->mycpuID = sched_getcpu();
	for(int i = 0; i<cpu_status[0]; i++){   
		if((threadID->mycpuID >= i*(cpu_status[1]/2) && threadID->mycpuID < (i+1)*(cpu_status[1]/2)) ||
	  		(threadID->mycpuID >= (i+cpu_status[0])*(cpu_status[1]/2) && threadID->mycpuID < (i+cpu_status[0]+1)*(cpu_status[1]/2))){
			threadID->myMemory = NODE_MEMORY + i;
			break;
		}
	}
	currthreadID = threadID;
}   

static void tx_rollback(Transaction* tx)
{
	l_entry* l;
	if(tx->mythreadID->l_set.nb_entries > 0){
		for(l = tx->mythreadID->l_set.entries; l < tx->mythreadID->l_set.entries + tx->mythreadID->l_set.nb_entries; l++)
		{	
			if(!ATOMIC_CAS_FULL(l->lock, 1, 0)){
				printf("release lock failed rollback\n");\
				exit(0);
			}
		}
		clear_buffer(tx->mythreadID->buffer);
	}
	tx->mythreadID->addr_set.nb_entries = 0;
	tx->mythreadID->r_set.nb_entries = 0;
	tx->mythreadID->l_set.nb_entries = 0;
	tx->mythreadID->op_set.nb_entries = 0;
	tx->status = TX_ABORTED;
}

void tx_begin()
{
	Transaction *tx = get_tx();
	tx->status = TX_ACTIVE;
	tx->start = tx->end = GET_CLOCK;
	tx->mythreadID = currthreadID;
	tx->nodeMemory = tx->mythreadID->myMemory;
}


bool tx_add(void* addr, size_t size)
{
	Transaction *tx = get_tx();
	assert(tx->status == TX_ACTIVE);
	if(write_to_buffer(tx, addr, size) == true){
		return true;
	}
	tx_rollback(tx);
	return false;
}


void tx_end()
{
	Transaction *tx = get_tx();
	assert(tx->status == TX_ACTIVE);
	uintptr_t t;

	if((tx->mythreadID->buffer)->count > 0)
	{
		t = FETCH_INC_CLOCK + 1;
		if (tx->start != t - 1 && !validate(tx))
		{
			tx_rollback(tx);
			return;
		}
//		printf("t is ..............%d\n", t);
		size_t len;
		void* result = log_append(tx, len, t);
		assert(result != NULL);
		nvm_flush((uintptr_t*)result, len);

		l_entry* l;
		for(l = tx->mythreadID->l_set.entries; l < tx->mythreadID->l_set.entries + tx->mythreadID->l_set.nb_entries; l++)		
		{
			
			if(!ATOMIC_CAS_FULL(l->lock, 1, 0)){
				printf("release lock failed end\n");
				exit(0);
//				assert(0);
			}
		}
		
		new_ht_ops(tx, t);   //update the hashtable
		
		combine_concurrent(tx->nodeMemory, tx->mythreadID->op_set);
		
		clear_buffer(tx->mythreadID->buffer);
	}
	tx->mythreadID->addr_set.nb_entries = 0;
	tx->mythreadID->r_set.nb_entries = 0;
	tx->mythreadID->l_set.nb_entries = 0;
	tx->mythreadID->op_set.nb_entries = 0;
	tx->status = TX_COMMITTED;
}


void* tx_rw(void* addr)
{
	Transaction *tx = get_tx();
	assert(tx->status == TX_ACTIVE);
/*	if(tx->nodeMemory->localTail != SHARED_MEMORY.logTail){
		updateFromLog(tx->nodeMemory, SHARED_MEMORY.logTail);
		tx->nodeMemory->localTail = SHARED_MEMORY.logTail;
	}
*/
	void* res = read_from_buffer(tx, addr);	
	if(res!=NULL)
		return res;
	tx_rollback(tx);
	return NULL;

}

void* tx_read(void* addr)
{
	Transaction *tx = get_tx();
	assert(tx->status == TX_ACTIVE);
/*	if(tx->nodeMemory->localTail != SHARED_MEMORY.logTail){
		updateFromLog(tx->nodeMemory, SHARED_MEMORY.logTail);
		tx->nodeMemory->localTail = SHARED_MEMORY.logTail;
	}
*/	void* value = log_find(tx, addr);
	if(value == NULL){
		tx_rollback(tx);
	}
	return value;
}


void* tx_ro(void* addr)
{
	Transaction *tx = get_tx();
	assert(tx->status == TX_ACTIVE);
/*	if(tx->nodeMemory->localTail != SHARED_MEMORY.logTail){
		updateFromLog(tx->nodeMemory, SHARED_MEMORY.logTail);
		tx->nodeMemory->localTail = SHARED_MEMORY.logTail;
	}
*/
	void* value = find(tx, addr);
	if(value == NULL){
		tx_rollback(tx);
	}
	return value;
}
/*
void* gc_concurrent()
{
	return log_clean_concurrent();
}

void* restore_concurrent()
{
	return crash_restore_concurrent();
}
*/
