#include "harris.h"

sval_t* set_contains(intset_t *set, skey_t key);
//int set_add(intset_t *set, skey_t key, const sval_t& val);
int set_add(intset_t *set, skey_t key, uint64_t version, void* logaddr, int tx_lock);
sval_t* set_remove(intset_t *set, skey_t key);
