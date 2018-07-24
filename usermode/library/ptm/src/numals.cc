#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <numa.h>
#include "numals.h"
#include "ssalloc.h"

#define NUM_ENTRIES_IN_LOG	1024*1024*20		/* The size of the shared log*/
#define MAX_OP_SIZE		8

s_sharedMemory SHARED_MEMORY;
s_nodeMemory* NODE_MEMORY;
int* cpu_status;

s_entries* initSharedLog()
{
        int i;
        s_entries* firstEntry = NULL;
        s_entries* lastEntry = NULL;
        for( i = 0; i< NUM_ENTRIES_IN_LOG; i++){
                s_entries* newentry = (s_entries*)malloc(sizeof(s_entries));
                newentry->index = i;
                newentry->rounds = false;
                newentry->filled = false;
                newentry->op = (char*)malloc(MAX_OP_SIZE * sizeof(char));
                if(lastEntry != NULL){
                        lastEntry->next = newentry;
                }else{
                        firstEntry = newentry;
                }
                lastEntry = newentry;
        }
        lastEntry->next = firstEntry;
        return firstEntry;

}

void* create_ht_concurrent(void* arg){	
	int pthreadReturnValue;
    s_nodeMemory* nodeMemory;
    s_threadId* thId = (s_threadId*)arg;
    if ((pthreadReturnValue = pthread_mutex_lock(&SHARED_MEMORY.globalMtx)) != 0) {
    	return (void*)-1;
    }
    nodeMemory = thId->nodeMemory;
    if ((nodeMemory->myCPUID = sched_getcpu()) == -1) {
		return (void*)-1;
    }
    nodeMemory->myThreadID = pthread_self();
	if (nodeMemory->replicaInitialized == 0) {
		ssalloc_init();
		nodeMemory->ht = ht_new();
        printf("nodeMemory->ht is .................%lx\n", nodeMemory->ht);
		nodeMemory->replicaInitialized = 1;
    }
    thId->ready = 1;
    if((pthreadReturnValue = pthread_cond_signal(&SHARED_MEMORY.globalCond)) != 0){
    	return (void*)-1;
    }
    if((pthreadReturnValue = pthread_mutex_unlock(&SHARED_MEMORY.globalMtx)) != 0) {
    	return (void*)-1;
    }
    pthread_exit(0);
}

void *init_numals(void* args)
{
	cpu_status = (int*) args;
	int i,j;
	int pthreadReturnValue;
	pthread_t thread[cpu_status[0]];
	cpu_set_t cpus;
	s_nodeMemory* loopNextNode = NULL;
	s_threadId threadDetails;

	if ((pthreadReturnValue = pthread_cond_init(&SHARED_MEMORY.globalCond, NULL)) != 0) { 
		return (void*)-1;
	}
	if ((pthreadReturnValue = pthread_mutex_init(&SHARED_MEMORY.globalMtx, NULL)) != 0) { 
		return (void*)-1;
	}

	SHARED_MEMORY.maxBatch = cpu_status[1];				
	SHARED_MEMORY.logMinLowMark = NUM_ENTRIES_IN_LOG - LOG_MIN_SAFE_ZONE - SHARED_MEMORY.maxBatch;
	SHARED_MEMORY.logMinUpdating = 0;	
	SHARED_MEMORY.logRounds = true;					
	SHARED_MEMORY.sharedLog = initSharedLog();			
	SHARED_MEMORY.logMin = SHARED_MEMORY.sharedLog;			
	SHARED_MEMORY.logTail = SHARED_MEMORY.sharedLog;		
	SHARED_MEMORY.completedTail = SHARED_MEMORY.sharedLog;		

	NODE_MEMORY = (s_nodeMemory*)malloc(cpu_status[0] * sizeof(s_nodeMemory));
	for (i=0; i<cpu_status[0]; i++) { /* Foreach node */
		((NODE_MEMORY + i)->ht) = NULL;				
		((NODE_MEMORY + i)->localTail) = SHARED_MEMORY.sharedLog;	
		((NODE_MEMORY + i)->replicaInitialized) = 0;					
		((NODE_MEMORY + i)->nextNode) = loopNextNode;		
		loopNextNode = (NODE_MEMORY + i);
	}
	SHARED_MEMORY.firstNode = loopNextNode; /* Pointer to one of the nodes (for looping purposes) */
	
	for (i=0; i<cpu_status[0]; i++) { /* Foreach node */
			threadDetails.ready = 0;
			threadDetails.nodeMemory = NODE_MEMORY + i;
			threadDetails.tailEntry = NULL;
			if (pthread_create(&thread[i], NULL, create_ht_concurrent, (void*)&threadDetails) == 0) {  
				CPU_ZERO(&cpus); /* Clears set, so that it contains no CPUs. */
				CPU_SET(i*(cpu_status[1]/2), &cpus); /* Add CPU cpu to set. */ 
				if ((pthreadReturnValue = pthread_setaffinity_np(thread[i], sizeof(cpu_set_t), &cpus)) != 0) {
					return (void*)-1;
				}
				while (!threadDetails.ready) {/* Wait on the condition until the ready flag is set */
					if ((pthreadReturnValue = pthread_cond_wait(&SHARED_MEMORY.globalCond, &SHARED_MEMORY.globalMtx)) != 0) {  
						return (void*)-1;
					}
				}
		
			} else {	
				return (void*)-1;
			}
	}
	
	for (i=0; i<cpu_status[0]; i++) { /* Foreach node */
		if ((pthreadReturnValue = pthread_join(thread[i], NULL)) != 0) { 
			return (void*)-1;
		}
	}
	if ((pthreadReturnValue = pthread_mutex_destroy(&SHARED_MEMORY.globalMtx)) != 0) { 
		return (void*)-1;
	}
	if ((pthreadReturnValue = pthread_cond_destroy(&SHARED_MEMORY.globalCond)) != 0) {
		return (void*)-1;
	}
	

	return (void*)0;
}

void destroySharedLog()
{
	int i;
	s_entries* currEntry = SHARED_MEMORY.sharedLog;
	s_entries* nextEntry = NULL;
	for (i=0;i<NUM_ENTRIES_IN_LOG;i++) {
		nextEntry = currEntry->next;
		free(currEntry->op);
		free(currEntry);
		currEntry = nextEntry;
	}
}

bool compareEntries(s_entries* a, s_entries* b) { 
	assert(b != NULL);
	if (a->rounds == b->rounds) { 
		
		return (a->index < b->index);
	} else if (SHARED_MEMORY.logRounds == a->rounds) { 

		return false;
	} else {
		return true;
	}
}

bool Execute(ht_intset_t* ht, char* op, uint64_t* args){
	if(strncmp(op, "add", 3) == 0){
//		sval_t v;
//		v.logaddr = reinterpret_cast<void*>(args[2]);
//		v.version = args[1];
//		v.tx_lock = args[3];
//     	printf("args[1] !!!!!!!!!!!is ..........%ld\n", args[1]);
//		printf("v.version !!!!!!!!!!!is ..........%ld\n", v.version);
//		if(ht_add(ht, args[0], v)){
//			return true;
//		}
		printf("ht is ................%lx\n", ht);		
		if(ht_add(ht, args[0], args[1], reinterpret_cast<void*>(args[2]), args[3])){
			return true;
		}
	}else if(strncmp(op, "remove", 6) == 0){
		if(ht_remove(ht, args[0]) != NULL)
			return true;
	}
	return false;
}
void updateFromLog(s_nodeMemory* nodeMemory, s_entries* to){
	s_entries* loopEntry = nodeMemory->localTail;
	while(compareEntries(loopEntry, to))
	{	
		while(loopEntry->filled == false){
			usleep(50);
		}
		Execute(nodeMemory->ht, loopEntry->op, loopEntry->args);
		loopEntry = loopEntry->next;
	}
}

s_entries* reserveLogEntries(int n)
{
	int i;
	s_entries* val = SHARED_MEMORY.logTail;
	s_entries* newval = val;
	for(i = 0; i < n; i++){
		newval = newval->next;
	}
	while (!__atomic_compare_exchange(&SHARED_MEMORY.logTail, &val, &newval, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {}
	return val;
}


s_entries* appendToLog(op_set_t& op_set)
{
	int i, j;
	const int zero = 0;
	const int one = 1;
	int n = op_set.nb_entries;
//	printf("n is ...................%d\n", n);
	s_entries* start = reserveLogEntries(n);
	s_entries* loopEntry = start;
	s_entries* originalLogMin = NULL;
	s_nodeMemory* loopNextNode = NULL;
	for(i = 0; i < n; i++){
		if(loopEntry->index == 0){
			SHARED_MEMORY.logRounds = !SHARED_MEMORY.logRounds;

		}
		loopEntry->rounds = SHARED_MEMORY.logRounds;
		loopEntry->filled = true;
		strcpy(loopEntry->op, op_set.entries[i].op);
		loopEntry->args[0] = op_set.entries[i].args[0];
		loopEntry->args[1] = op_set.entries[i].args[1];
		loopEntry->args[2] = op_set.entries[i].args[2];
		loopEntry->args[3] = op_set.entries[i].args[3];
		loopEntry = loopEntry->next;
		if (loopEntry->index == SHARED_MEMORY.logMinLowMark) { /* When a thread reaches a low mark in the log, which is max batch entries before logMin */
			printf("huibuhuijinruzhegehanshu\n");
			while (!__atomic_compare_exchange(&SHARED_MEMORY.logMinUpdating, &zero, &one, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {}
			originalLogMin = SHARED_MEMORY.logMin; /* Save the pointer to the original log min */
			SHARED_MEMORY.logMin = SHARED_MEMORY.firstNode->localTail;
			loopNextNode = SHARED_MEMORY.firstNode->nextNode; /* Start from node #2 */
			while (loopNextNode != NULL) { /* Foreach node */
				if (compareEntries(loopNextNode->localTail, SHARED_MEMORY.logMin)) {  /* Compare relation between two entries at the log, return true if a<b and false o.w */
					SHARED_MEMORY.logMin = loopNextNode->localTail; /* Updates logMin to the smallest localTail of all nodes */
				}
				loopNextNode = loopNextNode->nextNode; /* Move to the next core */
			}
			
			while (originalLogMin->index != SHARED_MEMORY.logMin->index) {
				originalLogMin->rounds = false;		/* Bit that indicate when the shared log wrap itself */
				originalLogMin->filled = false;		/* Reserved bit in the slot used to marks that the slot is filled */
				originalLogMin->op = NULL;		/* The operation */
				//originalLogMin->args = NULL;		/* The operation arguments */
				originalLogMin = originalLogMin->next;
			}
			SHARED_MEMORY.logMinLowMark = SHARED_MEMORY.logMin->index - LOG_MIN_SAFE_ZONE - SHARED_MEMORY.maxBatch;
			if (SHARED_MEMORY.logMinLowMark < 0) {
				SHARED_MEMORY.logMinLowMark += NUM_ENTRIES_IN_LOG;
			}
			while (!__atomic_compare_exchange(&SHARED_MEMORY.logMinUpdating, &one, &zero, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {}
		} else { /* Meanwhile, other threads wait for logMin to change */
			
			while (!__atomic_compare_exchange(&SHARED_MEMORY.logMinUpdating, &zero, &zero, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {}
		}
	}

	return start;
}


void combine_concurrent(s_nodeMemory* nodeMemory, op_set_t& op_set)
{
	struct s_entries* startEntry = NULL;
	struct s_entries* endEntry = NULL;
	startEntry = appendToLog(op_set);
	updateFromLog(nodeMemory, startEntry);
	endEntry = startEntry;
	for(int i = 0; i < op_set.nb_entries; i++){
		endEntry = endEntry->next;
    }
	printf("op_set.nb_entries is .............%d\n", op_set.nb_entries);
	nodeMemory->localTail = endEntry;
	for(int i = 0; i<op_set.nb_entries; i++){
		Execute(nodeMemory->ht, op_set.entries[i].op, op_set.entries[i].args);
	}
	for (int i=0; i<cpu_status[0]; i++) {
		s_nodeMemory* currMemory = NODE_MEMORY + i;
		if(currMemory != nodeMemory){
			updateFromLog(currMemory, endEntry);
			currMemory->localTail = endEntry;
		}
	}
//	printf("11111111111111111111111111\n");
}
