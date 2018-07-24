
#include "intset.h"

sval_t*
set_contains(intset_t *set, skey_t key)
{
  sval_t* result;				
  result = harris_find(set, key);
  return result;
}

//int set_add(intset_t *set, skey_t key, const sval_t& val)
int set_add(intset_t *set, skey_t key, uint64_t version, void* logaddr, int tx_lock)
{
  int result;

  result = harris_insert(set, key, version, logaddr, tx_lock);

  return result;
}

sval_t*
set_remove(intset_t *set, skey_t key)
{
  sval_t* result = NULL;
  result = harris_delete(set, key);	
  return result;
}

