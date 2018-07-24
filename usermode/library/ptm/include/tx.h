#ifndef __TX_CONCURRENT_H__
#define __TX_CONCURRENT_H__

#include <stddef.h>
#include <typeinfo>
#include <stdint.h>
#include <pthread.h>
#include "thread_init.h"
#include "numals.h"

extern uint64_t init_clock;
#define CLOCK (init_clock)
#define GET_CLOCK                       (ATOMIC_LOAD_ACQ(&CLOCK))
#define FETCH_INC_CLOCK                 (ATOMIC_FETCH_INC_FULL(&CLOCK))
enum {
	TX_ACTIVE,
	TX_COMMITTED,
	TX_ABORTED,
};

struct entry_head{
   void* haddr;
   int version;
   size_t size;
};


typedef struct{
	int status;
	threadID_t* mythreadID;
	uintptr_t start;
	uintptr_t end;
	int mycpuID;
	struct s_nodeMemory* nodeMemory;
}Transaction;
#endif
