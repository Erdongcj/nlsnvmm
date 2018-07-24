#include <stdlib.h>
#include <malloc.h>
#include "hashtable.h"

void ht_delete(ht_intset_t *set) 
{
  node_t *node, *next;
  int i;
  
  for (i=0; i < maxhtlength; i++) 
  {
      node = set->buckets[i].head;
      while (node != NULL) 
   	  {
	  	  next = node->next;
	  	  ssfree((void*) node);
//		  node = NULL;
	  	  node = next;
	  }
  }
  printf("delete success\n");
///  ssfree(set->buckets);
//  set->buckets = NULL;
//  ssfree(set);
//  set = NULL;
}

int
ht_size(ht_intset_t *set) 
{
  int size = 0;
  int i;
	
  for (i = 0; i < maxhtlength; i++) 
    {
      size += set_size(&set->buckets[i]);
    }
  return size;
}

int floor_log_2(unsigned int n) {
  int pos = 0;
  printf("n result = %d\n", n);
  if (n >= 1<<16) { n >>= 16; pos += 16; }
  if (n >= 1<< 8) { n >>=  8; pos +=  8; }
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }
  if (n >= 1<< 1) {           pos +=  1; }
  printf("floor result = %d\n", pos);
  return ((n == 0) ? (-1) : pos);
}

ht_intset_t* 
ht_new() 
{
  ht_intset_t *set;
  int i;
	
  if ((set = (ht_intset_t *)ssalloc_aligned_alloc(1, CACHE_LINE_SIZE, sizeof(ht_intset_t))) == NULL)
    {
      perror("malloc");
      printf("allocate failed\n");
	  exit(1);
    }  


  size_t bs = (maxhtlength + 1) * sizeof(intset_t);
  bs += CACHE_LINE_SIZE - (bs & CACHE_LINE_SIZE);
  if ((set->buckets = (intset_t*)ssalloc_alloc(1, bs)) == NULL)
    {
      perror("malloc");
      printf("allocate bucket failed\n");
	  exit(1);
    }  

  for (i = 0; i < maxhtlength; i++) 
    {
      bucket_set_init(&set->buckets[i]);
    }
  return set;
}

sval_t* ht_contains(ht_intset_t *set, skey_t key)
{
  int addr = key % maxhtlength;
  return set_contains(&set->buckets[addr], key);
}

//int ht_add(ht_intset_t *set, skey_t key, const sval_t& val)
int ht_add(ht_intset_t *set, skey_t key, uint64_t version, void* logaddr, int tx_lock)
{
	int addr = key % maxhtlength;
///	sval_t val;
//	val.version = version;
//	val.logaddr = logaddr;
//	val.tx_lock = tx_lock;
//	printf("val.version in ht %d\n", version);
	return set_add(&set->buckets[addr], key, version, logaddr, tx_lock);
}

sval_t* ht_remove(ht_intset_t *set, skey_t key)
{
	int addr = key % maxhtlength;
	return set_remove(&set->buckets[addr], key);
}
