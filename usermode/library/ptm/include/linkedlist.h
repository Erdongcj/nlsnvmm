
#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#include <atomic_ops.h>
#include "ssalloc.h"

#define ALIGNED(N) __attribute__ ((aligned (N)))

//#define ALIGNED(N)
#define CACHE_LINE_SIZE 64
#define CAS_U64(a,b,c) __sync_val_compare_and_swap(a,b,c)
#define CAS_U64_bool(addr, old, new) (old == CAS_U64(addr, old, new))
#define FAI_U32(a) __sync_fetch_and_add(a,1)
#define ATOMIC_CAS_MB(a, e, v)          CAS_U64_bool((volatile AO_t *) (a),(AO_t) (e), (AO_t) (v))
#define ATOMIC_FETCH_AND_INC_FULL(a)    FAI_U32(a)

#define KEY_MIN                         INT_MIN
#define KEY_MAX                         (INT_MAX - 2)


typedef uintptr_t skey_t;


typedef struct{
	void* logaddr;
	uintptr_t tx_lock;
	uint64_t version;
} sval_t;

typedef struct node 
{
  skey_t key;
  sval_t val;
  struct node* next;
  uint8_t padding32[8];
} node_t;

typedef ALIGNED(CACHE_LINE_SIZE) struct intset 
{
  node_t *head;
} intset_t;
node_t* new_node(skey_t key, uint64_t version, void* logaddr, int tx_lock, node_t* next);
//node_t *new_node(skey_t key, const sval_t& val, node_t *next);
void bucket_set_init(intset_t* set);
void set_delete(intset_t *set);
