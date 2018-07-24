#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <tx_api.h>
#include <wrap.hh>

#include "btree_map.h"

#define BTREE_ORDER 8
#define BTREE_MIN ((BTREE_ORDER / 2) - 1)
static int count;
struct tree_map_node_item {
	uint64_t key;
	uint64_t value;
};

struct tree_map_node {
	int n; /* number of occupied slots */
	struct tree_map_node_item items[BTREE_ORDER - 1];
	struct tree_map_node *slots[BTREE_ORDER];
};

struct btree_map {
	struct tree_map_node* root;
};

static void
set_empty_item(struct tree_map_node_item *item)
{
	item->key = 0;
	item->value = 0;
}

struct btree_map* btree_map_create()
{
//	size_t sz = sizeof(struct tree_map_node) + (BTREE_ORDER-1) * sizeof(struct tree_map_node_item);
	struct btree_map *map = (struct btree_map*)malloc(sizeof(struct btree_map));
	TX_BEGIN{
		TX_ADD(map, sizeof(struct btree_map));
		((struct btree_map*)TX_RW(map))->root = NULL;
//		TX_ADD(((struct btree_map*)TX_RW(map))->root, sz);		
//		((struct tree_map_node*)TX_RW(((struct btree_map*)TX_RW(map))->root))->n = 0;				
	}TX_END
	printf("create success\n");
	return map;
}


static int btree_map_insert_item_at(struct tree_map_node* node, int pos, struct tree_map_node_item& item)
{
	struct tree_map_node* nw = (struct tree_map_node*)TX_RW(node);
//	struct tree_map_node* nd = (struct tree_map_node*)TX_READ(node);
//	printf("nw nw nw nw nw is ............!!!!!!!!!!!%lx\n", nw);
	if(nw != NULL){
		nw->items[pos] = item;
		nw->n += 1;	
//		printf("pos is ..%d, nw %lx, nw->n %d, item.key %d, item.value %d\n", pos, nw, nw->n, item.key, item.value);
//		printf("count is ................%d\n", count++);
		return 1;
	}else{
		return 0;
	}
}

static int btree_map_insert_empty(struct btree_map* map, struct tree_map_node_item& item)
{
	size_t sz = sizeof(struct tree_map_node) + (BTREE_ORDER-1) * sizeof(struct tree_map_node_item);
	struct btree_map* m = (struct btree_map*)TX_READ(map);
	if(m == NULL)
		return 0;
	m->root = (struct tree_map_node*)malloc(sz);
	if(TX_ADD(m->root, sz) == false)
		return 0;
	struct tree_map_node* nw = (struct tree_map_node*)TX_RW(m->root);
	if(nw == NULL)
		return 0;
	nw->items[0] = item;
	nw->n = 1;
	for(int i=1; i<7; i++){
		nw->items[i].key = 0;
		nw->items[i].value = 0;
	}
	for(int i=0; i<8; i++){
		nw->slots[i] = NULL;
	}
	TX_ADD(m, sizeof(struct btree_map));
	((struct btree_map*)TX_RW(m))->root = m->root;
	return 1;
}

static int btree_map_insert_node(struct tree_map_node* node, int p, struct tree_map_node_item& item,
				struct tree_map_node* left, struct tree_map_node* right)
{
	size_t sz = sizeof(struct tree_map_node) + (BTREE_ORDER-1) * sizeof(struct tree_map_node_item);
	if(TX_ADD(node, sz) == false)
		return 0;
	struct tree_map_node* nw = (struct tree_map_node*)TX_RW(node);
	struct tree_map_node* nd = (struct tree_map_node*)TX_READ(node);
	if(nw == NULL)
		return 0;
	if(nd == NULL)
	    return 0;

	if(nd->items[p].key != 0){
//		printf("nw->items[p].key is ..............%d\n", nw->items[p].key);
//		nw->items[p+1]=nd->items[p];
//		nw->slots[p+1]=nd->slots[p];
		memmove(&nw->items[p+1], &nw->items[p], sizeof(struct tree_map_node_item) * ((BTREE_ORDER - 1 - p)));
		memmove(&nw->slots[p+1], &nw->slots[p], sizeof(struct tree_map_node*) * ((BTREE_ORDER - 1 - p)));		
	}
//	printf("p is .................%d\n", p);
	nw->slots[p] = left;
	nw->slots[p+1] = right;
//	struct tree_map_node* nd = (struct tree_map_node*)TX_READ(node);
//	if(nd == NULL)
//		return 0;
	for(int i = 0; i<BTREE_ORDER; i++){
		if(i<p){
			nw->slots[i] = nd->slots[i];
		}
		if(i>p+1 && i<BTREE_ORDER){
			nw->slots[i] = nd->slots[i-1];
		}
	}
//	printf("nw->slots[2] is ............%lx\n", nw->slots[2]);
	nw->items[p] = item;
	nw->n += 1;
	return 1;
	
}

static struct tree_map_node* btree_map_create_split_node(struct tree_map_node* node, struct tree_map_node_item *m)
{
	size_t sz = sizeof(struct tree_map_node) + (BTREE_ORDER-1) * sizeof(struct tree_map_node_item);
	struct tree_map_node *right = (struct tree_map_node*)malloc(sz);
	
	if(TX_ADD(right, sz) == false)
		return NULL;
	
	if(TX_ADD(node, sz) == false)
		return NULL;
	int c = (BTREE_ORDER / 2);

	struct tree_map_node* nw = (struct tree_map_node*)TX_RW(node);
	if(nw == NULL)
		return NULL;
	*m = nw->items[c-1];
//	printf("m->key is ..............%d\n", m->key);
	struct tree_map_node* r = (struct tree_map_node*)TX_RW(right);
	if(r == NULL)
	    return NULL; 
	set_empty_item(&nw->items[c-1]);
	for(int i = c; i < BTREE_ORDER; ++i){
		if(i != BTREE_ORDER - 1){
			r->items[r->n++] = nw->items[i];
			set_empty_item(&nw->items[i]);
		}
		r->slots[i-c] = nw->slots[i];
		nw->slots[i] = NULL;
	}
	nw->n = c-1;	
	return right;
}

static struct tree_map_node* btree_map_find_dest_node(struct btree_map* map, struct tree_map_node *n,
                             struct tree_map_node *parent, uint64_t key, int* p)
{
//	printf("key is ,,,,,,,,,,,,,,,,,,,,,%d\n", key);
	struct tree_map_node *no = (struct tree_map_node*)TX_RW(n);
//	printf("no is ....................%lx\n", no);
	if(no == NULL)
		return NULL;

	if(no->n == BTREE_ORDER - 1){
		struct tree_map_node_item m;
		struct tree_map_node* right = btree_map_create_split_node(n, &m);
		if(right == NULL){
			return NULL;
		}
		if(parent != NULL){
			if(!btree_map_insert_node(parent, *p, m, n, right))
				return NULL;
			if(key > m.key)
				n = right;
		}else{
			size_t sz = sizeof(struct tree_map_node) + (BTREE_ORDER-1) * sizeof(struct tree_map_node_item);
			struct tree_map_node* up = (struct tree_map_node*)malloc(sz);
			if(TX_ADD(up, sz) == false)
				return NULL;
			struct tree_map_node* ne = (struct tree_map_node*)TX_RW(up);
			if(ne == NULL)
				return NULL;
			ne->n = 1;
			ne->items[0].key = m.key;
			ne->items[0].value = m.value;
			ne->slots[0] = n;
			ne->slots[1] = right;
			if(TX_ADD(map, sizeof(struct btree_map)) == false)
				return NULL;
			struct btree_map* m = (struct btree_map*)TX_RW(map);
			if(m == NULL)
				return NULL;
			m->root = up;
			n = up;
		}

	}
	int i;
	for(i = 0; i < BTREE_ORDER - 1; ++i){
		*p = i;
		struct tree_map_node* no = (struct tree_map_node*)TX_RW(n);
//		printf("no is !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %lx\n", no);
		if(no == NULL)
			return NULL;
		
		if(no->n == i || no->items[i].key > key){
//			printf("no->slots[%d] is is  is !!!!!!!!!%lx\n", i, no->slots[i]);
			struct tree_map_node* ret = no->slots[i] == NULL ? n : btree_map_find_dest_node(map, no->slots[i], n, key, p);
			
//			printf("no->slots[%d] is is  is !!!!!!!!!\n", i);
			return ret;
		}
		
	}
//	printf("2222222222222222222222222222222\n");
	struct tree_map_node *nd = (struct tree_map_node*)TX_RW(n);
	if(nd == NULL)
		return NULL;
	return btree_map_find_dest_node(map, nd->slots[i], n, key, p);

}



static int btree_map_insert_item(struct tree_map_node* node, int p, struct tree_map_node_item& item)
{
	size_t sz = sizeof(struct tree_map_node) + (BTREE_ORDER-1) * sizeof(struct tree_map_node_item);
	if(TX_ADD(node, sz) == false)
		return 0;
	struct tree_map_node* nw = (struct tree_map_node*)TX_RW(node);
	if(nw == NULL)
		return 0;
	if(nw->items[p].key != 0){
		memmove(&nw->items[p+1], &nw->items[p], sizeof(struct tree_map_node_item) * (BTREE_ORDER - 2 - p));
	}

	nw->items[p] = item;
	nw->n += 1;
//	printf("nw %lx, nw->n %d, item.key %d, item.value %d\n", nw, nw->n, item.key, item.value);
//	printf("count is ....................%d\n", count++);
	return 1;
		
}

int btree_map_insert(struct btree_map* map, uint64_t key, uint64_t value)
{
	struct tree_map_node_item item = {key, value};
//	struct tree_map_node* var = NULL;
	TX_BEGIN{
		struct btree_map* m = (struct btree_map*)TX_READ(map);
//		printf("m is ....................%lx\n", m);
		if(m == NULL)
			return 0;
//		printf("mmmmmmmmmmmmmmmmmmmmmmmmmmmm\n");
		if(m->root == NULL){
			btree_map_insert_empty(map, item);
//			printf("111111111111111111\n");
		}else{
			int p;
			struct tree_map_node* parent = NULL;
			struct tree_map_node* dest = btree_map_find_dest_node(map, m->root, parent, key, &p);
			if(dest == NULL)
				return 0;
			if(btree_map_insert_item(dest, p, item) == 0){
				return 0;	
			}
//			var = dest;
		}

	}TX_END
	return 1;
	
}

static int btree_map_get_in_node(struct tree_map_node* node, uint64_t key)
{
	struct tree_map_node* no = (struct tree_map_node*)TX_RO(node);
	if(no == NULL)
	{	
//		printf("no is null\n");
		return 0;
	}
//	printf("node is %lx, node->n %d, no->n is %d, no->items[4].key %d\n", node, node->n, no->n, no->items[4].key);
	for (int i = 0; i <= no->n; ++i) {
//		printf("node is %lx, node->n %d, no->n is %d, no->items[i].key %d\n", node, node->n, no->n, no->items[i].key);
		if (i != no->n && no->items[i].key == key)
		{	
			struct tree_map_node* nd = (struct tree_map_node*)TX_READ(node);
//			printf("node is %lx, node->n %d, nd->n is %d, nd->items[%d].key %d, nd->items[i].value %d\n", node, node->n, nd->n, i, nd->items[i].key, nd->items[i].value);
//			printf("nd->items[i].value is ............%d\n", nd->items[i].value);
			if(nd == NULL)
				return 0;
			return 1;
		}else if((i == no->n || no->items[i].key > key) && no->slots[i]){
//			printf("1111111111111111111111111111111\n");
//			printf("i is ................%d\n", i);
			return btree_map_get_in_node(no->slots[i], key);
		}
	}
//	printf("here here\n");
	return 0;
}


int btree_map_get(struct btree_map* map, uint64_t key)
{
	int ret;
//	return 1;
	TX_BEGIN{
		struct btree_map* m = (struct btree_map*)TX_RO(map);
		if(m == NULL)
			return 0;
		struct tree_map_node* ro = m->root;
		if(ro == NULL)
			return 0;
		ret = btree_map_get_in_node(ro, key);			
	}TX_END
	return ret;
}
































