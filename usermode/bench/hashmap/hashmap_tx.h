#ifndef HASHMAP_TX_H
#define HASHMAP_TX_H

#include <stddef.h>
#include <stdint.h>

	struct hashmap_tx;
	
	struct hashmap_tx* hm_tx_create();
	int hm_tx_insert(struct hashmap_tx* hashmap, uint64_t key, uint64_t value);
	int hm_tx_remove(struct hashmap_tx* hashmap, uint64_t key);
	int hm_tx_get(struct hashmap_tx* hashmap, uint64_t key);
#endif
