
#include "linkedlist.h"

/* ################################################################### *
 * HARRIS' LINKED LIST
 * ################################################################### */

inline int is_marked_ref(long i);
inline long unset_mark(long i);
inline long set_mark(long i);
inline long get_unmarked_ref(long w);
inline long get_marked_ref(long w);

node_t* harris_search(intset_t *set, skey_t key, node_t** left_node);
sval_t* harris_find(intset_t *set, skey_t key);
//int harris_insert(intset_t *set, skey_t key, const sval_t& val);
int harris_insert(intset_t* set, skey_t key, uint64_t version, void* logaddr, int tx_lock);
sval_t* harris_delete(intset_t *set, skey_t key);
int set_size(intset_t *set);
