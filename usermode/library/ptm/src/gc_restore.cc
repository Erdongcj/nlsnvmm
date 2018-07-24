#include <string.h>
#include <malloc.h>
#include <sys/time.h>
#include <wrapper_log.hh>
#include <sched.h>
#include <pthread.h>
//#include "log_handle.h"
#include "numals.h"
#include "gc_restore.h"

#define MAX_NUMBER 1024
#define THREAD_NUM 1
#define SB_SIZE 8192
static uint64_t MAX_SB_NUM = 2*1024*1024;
typedef struct{
    int version;
    void* haddr;
    void* logaddr;
}temp_entry;

typedef struct{
	int nb_entries;
    temp_entry* entries;
}temp_set_t;

typedef struct{
	s_nodeMemory* currMemory;
	op_set_t op_set;
}gc_thread;

typedef struct{
	s_nodeMemory* currMemory;
	op_set_t op_set;
	temp_set_t temp_set;
}restore_thread;

void init_gc_thread(gc_thread* G_thread)
{
	ssalloc_init();
	G_thread->op_set.entries = (op_entry*)malloc(MAX_NUMBER * sizeof(op_entry));
	G_thread->op_set.nb_entries = 0;
	int n = sched_getcpu();
	for(int i = 0; i<cpu_status[0]; i++){
	    if((n >= i*(cpu_status[1]/2) && n < (i+1)*(cpu_status[1]/2)) ||
	  		(n >= (i+cpu_status[0])*(cpu_status[1]/2) && n < (i+cpu_status[0]+1)*(cpu_status[1]/2))){
		    G_thread->currMemory = NODE_MEMORY + i;
			break;
		}
	}

}

void init_restore_thread(restore_thread* R_thread)
{
	ssalloc_init();
	R_thread->op_set.entries = (op_entry*)malloc(MAX_NUMBER * sizeof(op_entry));
	R_thread->op_set.nb_entries = 0;
	R_thread->temp_set.entries = (temp_entry*)malloc(MAX_NUMBER * sizeof(temp_entry));
	R_thread->temp_set.nb_entries = 0;
	int n = sched_getcpu();
	for(int i = 0; i<cpu_status[0]; i++){
		if((n >= i*(cpu_status[1]/2) && n < (i+1)*(cpu_status[1]/2)) ||
	       (n >= (i+cpu_status[0])*(cpu_status[1]/2) && n < (i+cpu_status[0]+1)*(cpu_status[1]/2))){
	        R_thread->currMemory = NODE_MEMORY + i;
	        break;
	    }
	}

}

typedef struct{
   gc_thread* G_thread;
   restore_thread* R_thread;
}thread_info;

pthread_mutex_t cj_mutex = PTHREAD_MUTEX_INITIALIZER; 
static size_t size;
static uint64_t res;
static uint64_t start;
static uint64_t ret;
//int* cpu_status;

static void move_entry(gc_thread* G_thread, uint64_t start, uint64_t& out)
{
	s_nodeMemory* nodeMemory = G_thread->currMemory;
	
//	uint64_t ret = res;
//	printf("ret is ...............%d\n", ret);
	struct entry_head* en;
	op_entry *o, *o1;
	do{
		en = reinterpret_cast<struct entry_head*>(ret);
		sval_t* value = ht_contains(nodeMemory->ht, reinterpret_cast<uint64_t>(en->haddr));
//		printf("value is ............%lx\n", value);
		if(value != NULL){
//			printf("en->version %d, value->version %d\n", en->version, value->version);
			if(en->version == value->version){
				void* new_addr = log_malloc(sizeof(struct entry_head) + en->size);
//				printf("new_addr is %d\n", new_addr);
				memcpy(new_addr, (void*)en, sizeof(struct entry_head) + en->size);
				o = &(G_thread->op_set.entries[G_thread->op_set.nb_entries++]);
				o->op = "remove";
				o->args[0] = reinterpret_cast<uint64_t>(en->haddr);
				o1 = &(G_thread->op_set.entries[G_thread->op_set.nb_entries++]);
				o1->op = "add";
				o1->args[0] = reinterpret_cast<uint64_t>(en->haddr);
				o1->args[1] = value->version;
				o1->args[2] = reinterpret_cast<uint64_t>(new_addr);
				o1->args[3] = 0;
//				printf("G_thread->op_set.nb_entries %d\n", G_thread->op_set.nb_entries);
//			printf("push to hashtable\n");

			}
		}
		ret = ret + sizeof(struct entry_head) + en->size;

//		printf("ret ret ret is ..................%d\n", ret);
//		printf("start + 8192 + sizeof(struct entry_head) + en->size is .............%d\n", start + 8192 + sizeof(struct entry_head) + en->size);
	}while(ret < start + SB_SIZE + sizeof(struct entry_head) + en->size);
	out = ret;
//	printf("combine concurrent\n");
	combine_concurrent(nodeMemory, G_thread->op_set);
	G_thread->op_set.nb_entries = 0;
	free_slab(reinterpret_cast<void*>(start));
//	printf("success\n");
}

//void* log_clean(void* args)
void* log_clean(gc_thread* G_thread)
{
//	pthread_mutex_lock(&cj_mutex);
	printf("enter to log_clean\n");
//	gc_thread* G_thread = ((thread_info*)args)->G_thread;
	init_gc_thread(G_thread);
    s_nodeMemory* nodeMemory = G_thread->currMemory;
	void* ptr = get_head();
	res = reinterpret_cast<uint64_t>(ptr);
	start = reinterpret_cast<uint64_t>(ptr);
	ret = start;
	struct entry_head* eh;
//	pthread_mutex_lock(&cj_mutex);
loop:	while (res < (reinterpret_cast<uint64_t>(ptr) + 5*SB_SIZE)){
		eh = reinterpret_cast<struct entry_head*>(res);
		if(eh->size == 0){
			printf("loop break\n");
			break;
		}
		if(res >= start + SB_SIZE + sizeof(struct entry_head) + eh->size){
			start = start + SB_SIZE;
			size = 0;
		}
		sval_t* val = ht_contains(nodeMemory->ht, reinterpret_cast<uint64_t>(eh->haddr));
		if(val == NULL || eh == NULL)
			return (void*)-1;
		if(val->version != eh->version){
			size = size + sizeof(struct entry_head) + eh->size;
			printf("size is ...............%d\n", size);
			if((double)size / SB_SIZE >= 0.1){
				
//				printf("start is %d\n", start);
				uint64_t out;
				move_entry(G_thread, start, out);
//				printf("continue\n");
				start = start + SB_SIZE;
				size = 0;
				res = out;
				goto loop;
			}
		}

		res = res + sizeof(struct entry_head) + eh->size;		
	}
//	pthread_mutex_unlock(&cj_mutex);
	return (void*)0;

}
void* log_clean_concurrent()
{
	gc_thread* G_thread = (gc_thread*)malloc(sizeof(gc_thread));
	if(G_thread == NULL)
		return (void*)-1;
	log_clean(G_thread);
	return (void*)0;
}
/*
void* log_clean_concurrent()
{
	cpu_set_t cpus;
	thread_info *data;
	pthread_t thread[THREAD_NUM];
	if((data = (thread_info*)malloc(THREAD_NUM * sizeof(thread_info))) == NULL){
		printf("error malloc thread_info\n");
		exit(1);
	}

	for(int i=0; i<THREAD_NUM; i++){
		printf("creating log clean threads\n");
		data[i].G_thread = (gc_thread*)malloc(sizeof(gc_thread));
		data[i].R_thread = NULL;
		if(pthread_create(&thread[i], NULL, log_clean, (void*)(&data[i])) == 0){
			CPU_ZERO(&cpus); 
			CPU_SET(i, &cpus); 
			if(pthread_setaffinity_np(thread[i], sizeof(cpu_set_t), &cpus) != 0) {
				return (void*)-1;
			}
		}else{
			printf("Error creating clean thread\n");
			exit(1);
		}
	}
	for(int i=0; i<THREAD_NUM; i++) { 
		pthread_detach(thread[i]);
//		if(pthread_join(thread[i], NULL) != 0) { 
//			return (void*)-1;
//		}
	}
}
*/
void* crash_restore(restore_thread* R_thread)
{
//	restore_thread* R_thread = ((thread_info*)args)->R_thread;
	init_restore_thread(R_thread);
	void* ptr = get_head();
	uint64_t res = reinterpret_cast<uint64_t>(ptr);
	uint64_t start = res;
	struct entry_head* eh;
	op_entry* o1;
	temp_entry* te;
	s_nodeMemory* nodeMemory = R_thread->currMemory;
//	printf("111111111111111111111\n");
	while (res < reinterpret_cast<uint64_t>(ptr)+ MAX_SB_NUM * SB_SIZE){
//		printf("111111111111111111111\n");
		eh = reinterpret_cast<struct entry_head*>(res);
		if(eh->size == 0){
			printf("size == 0\n");
			break;
		}
		for(te = R_thread->temp_set.entries; te < R_thread->temp_set.entries + R_thread->temp_set.nb_entries; te++){
			if(te->haddr == eh->haddr){
				printf("");
				te->version = eh->version;
				te->logaddr = reinterpret_cast<void*>(res + sizeof(struct entry_head));
				goto next;
//				break;
//			}else{
//				te = &(R_thread->temp_set.entries[R_thread->temp_set.nb_entries++]);
//				te->version = eh->version;
//				te->haddr = eh->haddr;
//				te->logaddr = reinterpret_cast<void*>(res + sizeof(struct entry_head));
			}
		}
		te = &(R_thread->temp_set.entries[R_thread->temp_set.nb_entries++]);
		te->version = eh->version;
		te->haddr = eh->haddr;
		te->logaddr = reinterpret_cast<void*>(res + sizeof(struct entry_head));

next:		res = res + sizeof(struct entry_head) + eh->size;
	}
	for(te = R_thread->temp_set.entries; te < R_thread->temp_set.entries+R_thread->temp_set.nb_entries; te++){
		o1 = &(R_thread->op_set.entries[R_thread->op_set.nb_entries++]);
		o1->op = "add";
		o1->args[0] = reinterpret_cast<uint64_t>(te->haddr);
		o1->args[1] = te->version;
		o1->args[2] = reinterpret_cast<uint64_t>(te->logaddr);
		o1->args[3] = 0;	
	}
	combine_concurrent(nodeMemory, R_thread->op_set);
}

void* crash_restore_concurrent()
{
	restore_thread* R_thread = (restore_thread*)malloc(sizeof(restore_thread));
	if(R_thread == NULL)
	    return (void*)-1;
//	init_numals((void*)&cpu_status);
	for (int i=0; i<cpu_status[0]; i++) {
		s_nodeMemory* currMemory = NODE_MEMORY + i;
	    ht_delete(currMemory->ht);
//		printf("delete success\n");
	}
	struct timeval begin1, end1;
	float diff1;
	gettimeofday(&begin1, NULL);
	crash_restore(R_thread);
	gettimeofday(&end1, NULL);
	diff1 = (end1.tv_sec - begin1.tv_sec)*1000000 + (end1.tv_usec - begin1.tv_usec);
	printf(" times diff........................................................................: %f\n", diff1);
	return (void*)0;
}
/*
void* crash_restore_concurrent()
{
    cpu_set_t cpus;
    thread_info *data;
    pthread_t thread[THREAD_NUM];
    if((data = (thread_info*)malloc(THREAD_NUM * sizeof(thread_info))) == NULL){
        printf("error malloc thread_info\n");
        exit(1);
    }
 
    for(int i=0; i<THREAD_NUM; i++){
        printf("creating crash restore threads\n");
        data[i].R_thread = (restore_thread*)malloc(sizeof(restore_thread));
        data[i].G_thread = NULL;
        if(pthread_create(&thread[i], NULL, crash_restore, (void*)(&data[i])) == 0){
            CPU_ZERO(&cpus); 
            CPU_SET(i, &cpus);
            if(pthread_setaffinity_np(thread[i], sizeof(cpu_set_t), &cpus) != 0) {
                return (void*)-1;
            }
        }else{
	        printf("Error creating restore thread\n");
            exit(1);
        }
    }
    for(int i=0; i<THREAD_NUM; i++) {
        if(pthread_join(thread[i], NULL) != 0) {
            printf("Error waiting for restore thread completion\n");
			return (void*)-1;
		}
	}
}	                
*/
