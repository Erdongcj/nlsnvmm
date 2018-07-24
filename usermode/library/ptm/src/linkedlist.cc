#include <numa.h>
#include "linkedlist.h"

//node_t* new_node(skey_t key, const sval_t& val, node_t *next)
node_t* new_node(skey_t key, uint64_t version, void* logaddr, int tx_lock, node_t* next)
{
  node_t *node;
  
  node = (node_t *) ssalloc(sizeof(node_t));
  if (node == NULL) 
    {
      perror("malloc @ new_node");
      printf("malloc new node failed\n");
	  exit(1);
    }

  node->key = key;
  node->val.version = version;
  node->val.logaddr = logaddr;
  node->val.tx_lock = tx_lock;
//  printf("val.version in new_node %d\n", node->val.version);
//  printf("version is .................%d\n", version);
  node->next = next;
  return (node_t*) node;
}

void
bucket_set_init(intset_t* set)
{
  node_t *min, *max;
  sval_t v_max, v_min;
  v_max.logaddr = 0;
  v_max.tx_lock = 0;
  v_max.version = 0;
  v_min.logaddr = 0;
  v_min.tx_lock = 0;
  v_min.version = 0;
  max = new_node(KEY_MAX, 0, 0, 0, NULL);
  min = new_node(KEY_MIN, 0, 0, 0, max);
  set->head = min;
}

void set_delete(intset_t *set)
{
  node_t *node, *next;

  node = set->head;
  while (node != NULL) {
    next = node->next;
    ssfree((void*) node);
    node = next;
  }
  ssfree(set);
}

