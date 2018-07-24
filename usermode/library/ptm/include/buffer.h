#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include "bfalloc.h"

typedef uintptr_t bkey_t;
typedef struct{
	void* bfaddr;
	size_t size;
}bval_t;

struct bfentry{
	bkey_t key;
	bval_t val;
	struct bfentry* next;
};

struct bfbuckets{
	size_t nbuckets;
	struct bfentry* bucket[];
};

struct buffer{
	struct bfbuckets* buckets;
	uint64_t count;
	size_t totalSize;
};

struct buffer* create_buffer(int flag);
void* buffer_get(struct buffer* buffer, bkey_t key);
void buffer_insert(struct buffer* buffer, bkey_t key, const bval_t& val);
void delete_buffer(struct buffer* buffer);
void clear_buffer(struct buffer* buffer);
#endif
