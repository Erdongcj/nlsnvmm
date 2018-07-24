#include <string.h>
#include <malloc.h>
#include <sys/time.h>
#include <wrapper_log.hh>
#include <numa.h>
#include "log_handle.h"
#include "atomic.h"

static int count;
extern uint64_t totalSize;
extern uint64_t init_clock;

int validate(Transaction *tx)
{
	r_entry* r;
	l_entry* l;
	for(r = tx->mythreadID->r_set.entries; r < tx->mythreadID->r_set.entries+tx->mythreadID->r_set.nb_entries; r++)
	{
		if(r->lock != 0)
		{
			for(l = tx->mythreadID->l_set.entries; l < tx->mythreadID->l_set.entries + tx->mythreadID->l_set.nb_entries; l++)
			{
				if(r->node == l->node)
				{
					goto down; 
				}
			}
			return 0; 
		}
down:		sval_t *val = ht_contains(tx->nodeMemory->ht, reinterpret_cast<uintptr_t>(r->node));
			if(val != NULL){
				if(val->version != r->version) 
				return 0;
			}
	}
	return 1; //前面两个条件都成功可说明验证成功
}

static inline 
int 
snapshot_extend (Transaction *tx)
{
	uintptr_t now;

	/* Check status */
	assert(tx->status == TX_ACTIVE);

	/* Get current time */
	now = GET_CLOCK;

	if(validate(tx)){
		tx->end = now;
		return 1;
	}
	return 0;
}

void* has_read(Transaction* tx, void* addr) 
{
	void* ret;
	r_entry* r;

	for(r = tx->mythreadID->r_set.entries; r < tx->mythreadID->r_set.entries+tx->mythreadID->r_set.nb_entries; r++)	
	{
		if(r->node == addr)
			return r->node;
	}
	return NULL;
}

void* log_append(Transaction* tx, size_t& len, uintptr_t t)
{
	addr_entry* ae;
	size_t totalSize = 0;
	struct bfentry *var, *next;
	if(tx->mythreadID->buffer != NULL && tx->mythreadID->buffer->buckets != NULL){
/*		for (int i=0; i < tx->mythreadID->buffer->buckets->nbuckets; i++)
		{
			var = tx->mythreadID->buffer->buckets->bucket[i];
			while (var != NULL) 
			{
				next = var->next;
		    	totalSize += var->val.size;                      
				var = next;
			}   
		}   
*/		if(totalSize == 0)
			return NULL;
		len = totalSize;
		void *ptr = log_malloc(totalSize + tx->mythreadID->buffer->count * sizeof(struct entry_head));
//		printf("ptr is ............%d\n", reinterpret_cast<uint64_t>(ptr));
		if(ptr == NULL){
			assert(0);
		}
		uint64_t result = reinterpret_cast<uint64_t>(ptr);
		struct bfentry *e1, *e2;
		struct entry_head eh;
		for(int i=0; i < tx->mythreadID->buffer->buckets->nbuckets; i++)
    	{
			e1 = tx->mythreadID->buffer->buckets->bucket[i];
			while (e1 != NULL)
			{
				e2 = e1->next;
				eh.haddr = e1->val.bfaddr;
				eh.version = t;
				eh.size = e1->val.size;
				void* head_addr = reinterpret_cast<void*>(result);
//				printf("result is%d\n", result);
				memcpy(head_addr, &eh, sizeof(struct entry_head));
				ae = &(tx->mythreadID->addr_set.entries[tx->mythreadID->addr_set.nb_entries++]);
				ae->addr = reinterpret_cast<void*>(result + sizeof(struct entry_head));
				ae->key = e1->key;
				void *pptr = reinterpret_cast<void*>(result + sizeof(struct entry_head));
				size_t size = e1->val.size;
				void *qtr = e1->val.bfaddr;
				memcpy(pptr, qtr, size);
				result = result + e1->val.size + sizeof(struct entry_head);
				e1 = e2;
   			}
   		}
		return ptr;
	}
	return NULL;
}

bool write_to_buffer(Transaction* tx, void* addr, size_t size)
{
	l_entry* l;
	op_entry* o;
	uint64_t ret = reinterpret_cast<uintptr_t>(addr);
	sval_t *val = ht_contains(tx->nodeMemory->ht, ret);
	if(val != NULL){
		if(val->tx_lock != 0){
			for(l = tx->mythreadID->l_set.entries; l < tx->mythreadID->l_set.entries + tx->mythreadID->l_set.nb_entries; l++)		
			{
				if(l->node == addr){
					
					void *re = buffer_get(tx->mythreadID->buffer, ret);
					if(re != NULL)
 					{   
						return true;
					}else{
 						void* data = addr;
						bval_t v; 
						v.bfaddr = data;
					 	v.size = size;
						buffer_insert(tx->mythreadID->buffer, ret, v);
						return true;
 					}
				}
			}
			return false;
		}
		uint64_t version = val->version;
		if(version > tx->end){
			if(has_read(tx, addr) != NULL){  
				return false;			
			}
		}
//		printf("val->tx_lock is is is >>.......%d\n", val->tx_lock);
		if(!ATOMIC_CAS_FULL(&(val->tx_lock), 0, 1)){
			printf("lock failed\n");
			return false;
		}
		l = &(tx->mythreadID->l_set.entries[tx->mythreadID->l_set.nb_entries++]);
		l->node = addr;
		l->lock = &(val->tx_lock);
		void *re = buffer_get(tx->mythreadID->buffer, ret);
		if(re != NULL)
		{       
			return true;
		}else{
			void *data = addr;
			bval_t v;  
			v.bfaddr = data;
			v.size = size;
			buffer_insert(tx->mythreadID->buffer, ret, v);
		 	return true;
		}  
	}
	void *re = buffer_get(tx->mythreadID->buffer, ret);
	if(re != NULL)
	{       
		return true;
	}else{
		void *data = addr;
		bval_t v;  
		v.bfaddr = data;
		v.size = size;               
		buffer_insert(tx->mythreadID->buffer, ret, v);
	 	return true;
	}  
}

void* read_from_buffer(Transaction* tx, void* addr)
{
	uint64_t version;
	uint64_t key = reinterpret_cast<uint64_t>(addr);
	sval_t *val = ht_contains(tx->nodeMemory->ht, key);
	if(val !=  NULL){		
		if(val->tx_lock != 0){
			l_entry* l;
			for(l = tx->mythreadID->l_set.entries; l < tx->mythreadID->l_set.entries + tx->mythreadID->l_set.nb_entries; l++)	
			{
				if(l == NULL)
					return NULL;
				if(l->node == addr){
					void* value = addr;
					return value;
				}
			}
//			printf("val->tx_lock is is is >>.......%d\n", val->tx_lock);
			return NULL;
		}
		void* value = addr;
		if(val->tx_lock != 0){
			return NULL;
		}
		version = val->version;
		if(version > tx->end){ 
			if(!snapshot_extend(tx)){
				return NULL;
			}
	//成功扩展快照之后再检查一下锁，确保在扩展快照过程中没有其他事务提前获得锁并修改了值
			if(val->tx_lock != 0 || version > tx->end){
				return NULL;
			}
		}
		void* e = has_read(tx, addr);
		if(e != NULL)
		{
			return value;
		}else{
			r_entry *re = &(tx->mythreadID->r_set.entries[tx->mythreadID->r_set.nb_entries++]);
			re->node = addr;
			re->version = version;
			re->lock = &(val->tx_lock);  //验证读集的时候要用
		}
		//如果事务的快照上界要小于当前条目的版本号，那么就要扩展快照,在扩展快照的过程中要验证读集
		return value;
	}else{
		return addr;
	}
}

void* log_find(Transaction* tx, void* addr)
{
	uint64_t version;
	uint64_t key = reinterpret_cast<uint64_t>(addr);
	if(tx->nodeMemory == NULL)
		return NULL;
	sval_t *val = ht_contains(tx->nodeMemory->ht, key);
	if(val !=  NULL){		
		if(val->tx_lock != 0){
			l_entry* l;
			for(l = tx->mythreadID->l_set.entries; l < tx->mythreadID->l_set.entries + tx->mythreadID->l_set.nb_entries; l++)		
			{
				if(l->node == addr){
					void* value = reinterpret_cast<void*>(val->logaddr);
					return value;
				}
			}
			return NULL;
		}
		void* value = reinterpret_cast<void*>(val->logaddr);
		if(val->tx_lock != 0){
			return NULL;
		}
		version = val->version;
		if(version > tx->end){ 
			if(!snapshot_extend(tx)){
				return NULL;

			}
				//成功扩展快照之后再检查一下锁，确保在扩展快照过程中没有其他事务提前获得锁并修改了值
			if(val->tx_lock != 0 || version > tx->end){
				return NULL;
			}
		}

		void* e = has_read(tx, addr);
		if(e != NULL)
		{

			return value;
		}else{
			r_entry *re = &(tx->mythreadID->r_set.entries[tx->mythreadID->r_set.nb_entries++]);
			re->node = addr;
			re->version = version;
			re->lock = &(val->tx_lock);  //验证读集的时候要用
		}
			//如果事务的快照上界要小于当前条目的版本号，那么就要扩展快照,在扩展快照的过程中要验证读集
		return value;
	}else{
		return NULL;
	}
}

void new_ht_ops(Transaction* tx, uintptr_t t)
{
	op_entry* o;
	for(int i = 0; i < tx->mythreadID->addr_set.nb_entries; i++){
		if(ht_contains(tx->nodeMemory->ht, tx->mythreadID->addr_set.entries[i].key) != NULL){
			o = &(tx->mythreadID->op_set.entries[tx->mythreadID->op_set.nb_entries++]);
			o->op = "remove";
			o->args[0] = tx->mythreadID->addr_set.entries[i].key;

			o = &(tx->mythreadID->op_set.entries[tx->mythreadID->op_set.nb_entries++]);
			o->op = "add";
			o->args[0] = tx->mythreadID->addr_set.entries[i].key;
			o->args[1] = t;
			o->args[2] = reinterpret_cast<uint64_t>(tx->mythreadID->addr_set.entries[i].addr);
			o->args[3] = 0;
		}else{
			o = &(tx->mythreadID->op_set.entries[tx->mythreadID->op_set.nb_entries++]);
			o->op = "add";
			o->args[0] = tx->mythreadID->addr_set.entries[i].key;
			o->args[1] = t;
			o->args[2] = reinterpret_cast<uint64_t>(tx->mythreadID->addr_set.entries[i].addr);
			o->args[3] = 0;

		}
	}
}


void* find(Transaction* tx, void* addr)
{
	sval_t *val = ht_contains(tx->nodeMemory->ht, reinterpret_cast<uint64_t>(addr));
	
	if(val != NULL){
		void* value = reinterpret_cast<void*>(val->logaddr);
		return value;
	}
	return NULL;
}


