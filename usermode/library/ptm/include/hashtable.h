
#include "intset.h"


/* Hashtable length (# of buckets) */
static unsigned int maxhtlength = 1024;
#define TLS
/* Hashtable seed */
#ifdef TLS
extern __thread unsigned int *rng_seed;
#else /* ! TLS */
extern pthread_key_t rng_seed_key;
#endif /* ! TLS */

typedef struct ht_intset 
{
  intset_t* buckets;
  uint8_t padding[CACHE_LINE_SIZE - 16 - sizeof(node_t*)];
} ht_intset_t;

void ht_delete(ht_intset_t *set);
int ht_size(ht_intset_t *set);
int floor_log_2(unsigned int n);
ht_intset_t *ht_new();
sval_t* ht_contains(ht_intset_t* set, skey_t key);
//int ht_add(ht_intset_t* set, skey_t key, const sval_t& val);
int ht_add(ht_intset_t *set, skey_t key, uint64_t version, void* logaddr, int tx_lock);
sval_t* ht_remove(ht_intset_t* set, skey_t key);

