#ifndef __THREAD_INIT_H__
#define __THREAD_INIT_H__

#include <stddef.h>
#include <typeinfo>
#include <stdint.h>
#include <pthread.h>
#include <numa.h>
#include "buffer.h"
#include "numals.h"
typedef struct r_entry_t {
  uintptr_t version;          /* Version read */
  void *node;            /* Pointer to entry */
  uint64_t *lock;
} r_entry;

typedef struct r_set_s{
	r_entry *entries;
	int nb_entries;
}r_set_t;

typedef struct l_entry_t {
  void *node;            /* Pointer to entry */
  uint64_t* lock;	
} l_entry;

typedef struct l_set_s{
	l_entry *entries;
	int nb_entries;
}l_set_t;

typedef struct addr_entry_t{
	void* addr;
	uint64_t key;
}addr_entry;

typedef struct addr_set_s {  
	addr_entry *entries;
	int  nb_entries;         /* Number of entries */
} addr_set_t;

typedef struct{
	struct buffer *buffer;
	int mycpuID;
	addr_set_t addr_set;
	r_set_t r_set;
	l_set_t l_set;
	op_set_t op_set;
	s_nodeMemory* myMemory;
}threadID_t;


#endif
