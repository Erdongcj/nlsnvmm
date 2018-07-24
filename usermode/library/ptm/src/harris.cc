
#include "harris.h"

#define likely(x)       __builtin_expect((x), 1)
#define unlikely(x)  __builtin_expect((x), 0)


static int count;
/*
 * The five following functions handle the low-order mark bit that indicates
 * whether a node is logically deleted (1) or not (0).
 *  - is_marked_ref returns whether it is marked, 
 *  - (un)set_marked changes the mark,
 *  - get_(un)marked_ref sets the mark before returning the node.
 */
inline int
is_marked_ref(long i) 
{
  /* return (int) (i & (LONG_MIN+1)); */
  return (int) (i & 0x1L);
}

inline long
unset_mark(long i)
{
  /* i &= LONG_MAX-1; */
  i &= ~0x1L;
  return i;
}

inline long
set_mark(long i) 
{
  /* i = unset_mark(i); */
  /* i += 1; */
  i |= 0x1L;
  return i;
}

inline long
get_unmarked_ref(long w) 
{
  /* return unset_mark(w); */
  return w & ~0x1L;
}

inline long
get_marked_ref(long w) 
{
  /* return set_mark(w); */
  return w | 0x1L;
}

/*
 * harris_search looks for value val, it
 *  - returns right_node owning val (if present) or its immediately higher 
 *    value present in the list (otherwise) and 
 *  - sets the left_node to the node owning the value immediately lower than val. 
 * Encountered nodes that are marked as logically deleted are physically removed
 * from the list, yet not garbage collected.
 */
node_t*
harris_search(intset_t *set, skey_t key, node_t **left_node) 
{
  node_t *left_node_next, *right_node;
  left_node_next = set->head;
	
  do
    {
      node_t *t = set->head;
      node_t *t_next = set->head->next;
		
      /* Find left_node and right_node */
      do 
	{
	  if (!is_marked_ref((long) t_next)) 
	    {
	      (*left_node) = t;
	      left_node_next = t_next;
	    }
	  t = (node_t *) get_unmarked_ref((long) t_next);
	  if (!t->next) 
	    {
	      break;
	    }
	  t_next = t->next;
	} 
      while (is_marked_ref((long) t_next) || (t->key < key));
      right_node = t;
		
      /* Check that nodes are adjacent */
      if (left_node_next == right_node) 
	{
	  if (right_node->next && is_marked_ref((long) right_node->next))
	    {
	      continue;
	    }
	  else
	    {
	      return right_node;
	    }
	}
		
      /* Remove one or more marked nodes */
      if (ATOMIC_CAS_MB(&(*left_node)->next, left_node_next, right_node)) 
	  {
		node_t *cur = left_node_next;
		do{
			node_t* node = cur;
			cur = (node_t*)get_unmarked_ref((long)cur->next);
			ssfree(node);
		}while(cur != right_node);

   	    if (!(right_node->next && is_marked_ref((long) right_node->next)))
	    {
	      return right_node;
	    }
	  } 
    } 
  while (1);
}

/*
 * harris_find returns whether there is a node in the list owning value val.
 */
sval_t*
harris_find(intset_t *set, skey_t key) 
{
  node_t *right_node, *left_node;
  left_node = set->head;
	
  right_node = harris_search(set, key, &left_node);
  if ((right_node->next == NULL) || right_node->key != key)
    {
      return 0;
    }
  else 
    {
      return &(right_node->val);
    }
}

/*
 * harris_find inserts a new node with the given value val in the list
 * (if the value was absent) or does nothing (if the value is already present).
 */
//int harris_insert(intset_t *set, skey_t key, const sval_t& val) 
int harris_insert(intset_t* set, skey_t key, uint64_t version, void* logaddr, int tx_lock)
{
  node_t *newnode = NULL, *right_node, *left_node;
  left_node = set->head;
//  printf("val.version in harris !!!!!!!!!!!!!%d\n", version);	
  do 
    {
      right_node = harris_search(set, key, &left_node);
      if (right_node->key == key)
	  {
	  	if(unlikely(newnode != NULL)){
			ssfree(newnode);
		}
	  	return 0;
	  }

      if (likely(newnode == NULL))
	{
//	  newnode = new_node(key, val, right_node);
		newnode = new_node(key, version, logaddr, tx_lock, right_node);
	}
      else
	{
	  newnode->next = right_node;
	}

      if (ATOMIC_CAS_MB(&left_node->next, right_node, newnode))
	return 1;
    } 
  while(1);
}

/*
 * harris_find deletes a node with the given value val (if the value is present) 
 * or does nothing (if the value is already present).
 * The deletion is logical and consists of setting the node mark bit to 1.
 */
sval_t*
harris_delete(intset_t *set, skey_t key)
{
  node_t *right_node, *right_node_next, *left_node;
  left_node = set->head;
  sval_t* ret = NULL;
	
  do 
    {
      right_node = harris_search(set, key, &left_node);
      if (right_node->key != key)
	{
	  return NULL;
	}
      right_node_next = right_node->next;
      if (!is_marked_ref((long) right_node_next))
	{
	  if (ATOMIC_CAS_MB(&right_node->next, right_node_next, get_marked_ref((long) right_node_next)))
	    {
	      ret = &(right_node->val);
	      break;
	    }
	}
    } 
  while(1);

  if (likely(ATOMIC_CAS_MB(&left_node->next, right_node, right_node_next)))
    {
		ssfree((void*)get_unmarked_ref((long)right_node));
      ;
    }

  return ret;
}

int
set_size(intset_t *set)
{
  int size = 0;
  node_t* node;

  /* We have at least 2 elements */
  node = (node_t*) get_unmarked_ref((long) set->head->next);
  while ((node_t*) get_unmarked_ref((long) node->next) != NULL)
    {
      if (!is_marked_ref((long) node->next)) size++;
      node = (node_t*) get_unmarked_ref((long) node->next);
    }
  return size;
}
