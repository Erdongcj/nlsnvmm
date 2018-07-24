#ifndef __NUMALS_H_
#define __NUMALS_H_
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include "hashtable.h"

#define LOG_MIN_SAFE_ZONE	24
#define MAX_CORES		512	

typedef struct op_entry_t{
    char* op;
	uint64_t args[4];
}op_entry;
     
typedef struct op_set_s{
	op_entry* entries;
	int nb_entries;
} op_set_t;


struct s_entries{
	int index;
	bool rounds;
	bool filled;
	char* op;
	uint64_t args[4];
	struct s_entries* next;
};

struct s_nodeMemory{
	int replicaInitialized;
	pid_t		myThreadID;	
	int myCPUID;
	ht_intset_t* ht;
	s_entries* localTail;
	struct s_nodeMemory* nextNode;
};

struct s_sharedMemory{
	int		maxBatch;		/* The maximum size of the*  batch array */
	int		logMinLowMark;		/* The index where to re-calculate log min */
	int		logMinUpdating;
	bool		logRounds;		
	struct s_entries*	logMin;
	struct s_entries* sharedLog;
	struct s_entries* logTail;
	struct s_entries* completedTail;
	struct s_nodeMemory* firstNode;
	pthread_cond_t	globalCond;		/* Global lock shared by all threads, Use only to init & destroy of the threads */
	pthread_mutex_t	globalMtx;
};

struct s_threadId {
	int		ready;		
	struct s_nodeMemory*	nodeMemory;		/* Pointer to the shared memory of the core parent node */
	struct s_entries* tailEntry;
};

extern s_sharedMemory SHARED_MEMORY;
extern s_nodeMemory* NODE_MEMORY;
extern int* cpu_status;
void* init_numals(void* args);
void combine_concurrent(s_nodeMemory* nodeMemory, op_set_t& op_set);
s_entries* appendToLog(op_set_t& op_set);
void updateFromLog(s_nodeMemory* nodeMemory, s_entries* to);
void create_hashtable(void* args);
#endif
