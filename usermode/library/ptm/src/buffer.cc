#include <assert.h>
#include <stdio.h>
#include "buffer.h"

static int num_buckets;

//uint64_t totalSize;

struct buffer* create_buffer(int flag)
{
	if(flag == 1){
		num_buckets = 1000000;
	}else{
		num_buckets = 64;
	}
	size_t len = num_buckets;
	size_t sz = sizeof(struct bfbuckets) + len * sizeof(struct bfentry);
	struct buffer* buffer = (struct buffer*)bfalloc(sizeof(struct buffer));
	
	assert(buffer != NULL);
	buffer->count = 0;
	buffer->buckets = (struct bfbuckets*)bfalloc(sz);
	assert(buffer->buckets != NULL);
	buffer->buckets->nbuckets = len;
	for(int i = 0; i < len; i++){
		buffer->buckets->bucket[i] = NULL;
	}
	return buffer;
}

static uint64_t hash(bkey_t key)
{
	size_t len = num_buckets;
	return key % len;
}

void buffer_insert(struct buffer* buffer, bkey_t key, const bval_t& val)
{
	struct bfbuckets* buckets = buffer->buckets;
	uint64_t h = hash(key);
	struct bfentry* e = (struct bfentry*)bfalloc(sizeof(struct bfentry));
	assert(e != NULL);
	e->key = key;
	e->val.bfaddr = val.bfaddr;
	e->val.size = val.size;
	e->next = buckets->bucket[h];
	buckets->bucket[h] = e;
	buffer->count++;
	buffer->totalSize += val.size;
}

void* buffer_get(struct buffer* buffer, bkey_t key)
{
	struct bfbuckets* buckets = buffer->buckets;
	uint64_t h = hash(key);
	struct bfentry* var;
	for(var = buckets->bucket[h]; var != NULL; var = var->next){
		if(var->key == key)
			
			return var->val.bfaddr;
	}
	return NULL;
}

void delete_buffer(struct buffer* buffer)
{
	struct bfentry *var, *next;
  	int i;
  
  	for (i=0; i < num_buckets; i++) 
    {
      var = buffer->buckets->bucket[i];
      while (var != NULL) 
	  {
	  	next = var->next;
	    bffree((void*) var);
	    var = next;
	  }
    }
	bffree(buffer->buckets);
	bffree(buffer);
}

void clear_buffer(struct buffer* buffer)
{
	struct bfentry *var, *next;
	int i;
	for (i=0; i < num_buckets; i++)
	{
		var = buffer->buckets->bucket[i];
		while (var != NULL)
		{
			next = var->next;
			bffree((void*) var);
			var = next;
		}
		buffer->buckets->bucket[i] = NULL;
	}
	buffer->count = 0;
}
