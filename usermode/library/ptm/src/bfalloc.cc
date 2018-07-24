#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <numa.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>

#include "bfalloc.h"

#define BF_CACHE_LINE_SIZE 64

static __thread uintptr_t bfalloc_app_mem[BFALLOC_NUM_ALLOCATORS];
static __thread size_t alloc_next[BFALLOC_NUM_ALLOCATORS] = {0};
static __thread void* bfalloc_free_list[BFALLOC_NUM_ALLOCATORS][256] = {{0}};
static __thread uint8_t bfalloc_free_cur[BFALLOC_NUM_ALLOCATORS] = {0};
static __thread uint8_t bfalloc_free_num[BFALLOC_NUM_ALLOCATORS] = {0};

void
bfalloc_set(void* mem)
{
	bfalloc_app_mem[0] = (uintptr_t) mem;
}

void
bfalloc_init()
{
	int i;
//	printf("11111111111111111111111111111111111111111111111\n");
	for (i = 0; i < BFALLOC_NUM_ALLOCATORS; i++)
	{
//		bfalloc_app_mem[i] = (uintptr_t) memalign(BF_CACHE_LINE_SIZE, BFALLOC_SIZE);
		bfalloc_app_mem[i] = (uintptr_t) numa_alloc_local(BFALLOC_SIZE);
//		printf("ssalloc_app_mem[i] is !!!!!!!!!!!!!!!!!!!!!!!!%lx\n", bfalloc_app_mem[i]);
		assert((void*) bfalloc_app_mem[i] != NULL);
	}
}

void
bfalloc_offset(size_t size)
{
	bfalloc_app_mem[0] += size;
}

void*
bfalloc_alloc(unsigned int allocator, size_t size)
{
  void* ret = NULL;
  if (bfalloc_free_num[allocator] > 2)
  {
  	uint8_t spot = bfalloc_free_cur[allocator] - bfalloc_free_num[allocator];
	ret = bfalloc_free_list[allocator][spot];
	bfalloc_free_num[allocator]--;
  }else{
  	ret = (void*) (bfalloc_app_mem[allocator] + alloc_next[allocator]);
	alloc_next[allocator] += size;
	if (alloc_next[allocator] > BFALLOC_SIZE)
	{
//		printf("error !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!error\n");
		fprintf(stderr, "*** warning: allocator %2d : out of bounds alloc\n", allocator);
	}

  
  }
//  printf("ret is ........................%p\n", ret);
  return ret;
}

void*
bfalloc(size_t size)
{
	
  return bfalloc_alloc(0, size);
}

void*
bfalloc_aligned_alloc(unsigned int allocator, size_t alignement, size_t size)
{
  	void* ret = NULL;
	ret = (void*) (bfalloc_app_mem[allocator] + alloc_next[allocator]);
	uintptr_t retu = (uintptr_t) ret;
	if ((retu & (alignement - 1)) != 0)
	{
		size_t offset = alignement - (retu & (alignement - 1));
		retu += offset;
		alloc_next[allocator] += offset;
		ret = (void*) retu;
	}
	alloc_next[allocator] += size;
	assert((((uintptr_t) ret) & (alignement-1)) == 0);
	if(alloc_next[allocator] > BFALLOC_SIZE)
	{
		fprintf(stderr, "*** warning: allocator %2d : out of bounds alloc\n", allocator);
	}
	return ret;
}

void*
bfalloc_aligned(size_t alignment, size_t size)
{
  	return bfalloc_aligned_alloc(0, alignment, size);
}

void
bffree_alloc(unsigned int allocator, void* ptr)
{
	bfalloc_free_num[allocator]++;
	bfalloc_free_list[allocator][bfalloc_free_cur[allocator]++] = ptr;
}

void
bffree(void* ptr)
{
  bffree_alloc(0, ptr);
}





















