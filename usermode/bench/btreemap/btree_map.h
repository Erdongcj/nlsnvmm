#ifndef __BTREE_H_
#define __BTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>


struct btree_map;

//int btree_map_check(struct btree_map* map);
//int btree_map_check(struct btree_map* map);
//int btree_map_create(struct btree_map *map);
///int btree_map_destroy(struct btree_map *map);
struct btree_map* btree_map_create();
int btree_map_insert(struct btree_map* map,
	uint64_t key, uint64_t value);
//int btree_map_insert_new(struct btree_map* map,
//		uint64_t key, size_t size, unsigned type_num,
//		void *arg);
//uint64_t btree_map_remove(struct btree_map* map,
//		uint64_t key);
//int btree_map_remove_free(struct btree_map* map,
//		uint64_t key);
//int btree_map_clear(struct btree_map* map);
int btree_map_get(struct btree_map* map, uint64_t key);
//int btree_map_lookup(struct btree_map* map, uint64_t key);
//int btree_map_foreach(struct btree_map* map);
int btree_map_is_empty(struct btree_map* map);

#endif /* __BTREE_H */


