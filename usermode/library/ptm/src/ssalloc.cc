#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <numa.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>

#include "ssalloc.h"

#define SSMEM_CACHE_LINE_SIZE 64

static __thread uintptr_t ssalloc_app_mem[SSALLOC_NUM_ALLOCATORS];
static __thread size_t alloc_next[SSALLOC_NUM_ALLOCATORS] = {0};
static __thread void* ssalloc_free_list[SSALLOC_NUM_ALLOCATORS][256] = {{0}};
static __thread uint8_t ssalloc_free_cur[SSALLOC_NUM_ALLOCATORS] = {0};
static __thread uint8_t ssalloc_free_num[SSALLOC_NUM_ALLOCATORS] = {0};

void
ssalloc_set(void* mem)
{
	ssalloc_app_mem[0] = (uintptr_t) mem;
}

void
ssalloc_init()
{
	int i;
//	printf("11111111111111111111111111111111111111111111111\n");
	for (i = 0; i < SSALLOC_NUM_ALLOCATORS; i++)
	{
//		ssalloc_app_mem[i] = (uintptr_t) memalign(SSMEM_CACHE_LINE_SIZE, SSALLOC_SIZE);
		ssalloc_app_mem[i] = (uintptr_t) numa_alloc_local(SSALLOC_SIZE);
//		printf("ssalloc_app_mem[i] is !!!!!!!!!!!!!!!!!!!!!!!!%lx\n", ssalloc_app_mem[i]);
		assert((void*) ssalloc_app_mem[i] != NULL);
	}
}

void
ssalloc_offset(size_t size)
{
	ssalloc_app_mem[0] += size;
}

void*
ssalloc_alloc(unsigned int allocator, size_t size)
{
  void* ret = NULL;
  if (ssalloc_free_num[allocator] > 2)
  {
  	uint8_t spot = ssalloc_free_cur[allocator] - ssalloc_free_num[allocator];
	ret = ssalloc_free_list[allocator][spot];
	ssalloc_free_num[allocator]--;
  }else{
  	ret = (void*) (ssalloc_app_mem[allocator] + alloc_next[allocator]);
	alloc_next[allocator] += size;
	if (alloc_next[allocator] > SSALLOC_SIZE)
	{
		fprintf(stderr, "*** warning: allocator %2d : out of bounds alloc\n", allocator);
	}

  
  }
  return ret;
}

void*
ssalloc(size_t size)
{
  return ssalloc_alloc(0, size);
}

void*
ssalloc_aligned_alloc(unsigned int allocator, size_t alignement, size_t size)
{
  	void* ret = NULL;
	ret = (void*) (ssalloc_app_mem[allocator] + alloc_next[allocator]);
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
	if(alloc_next[allocator] > SSALLOC_SIZE)
	{
		fprintf(stderr, "*** warning: allocator %2d : out of bounds alloc\n", allocator);
	}
	return ret;
}

void*
ssalloc_aligned(size_t alignment, size_t size)
{
  	return ssalloc_aligned_alloc(0, alignment, size);
}

void
ssfree_alloc(unsigned int allocator, void* ptr)
{
	ssalloc_free_num[allocator]++;
	ssalloc_free_list[allocator][ssalloc_free_cur[allocator]++] = ptr;
}

void
ssfree(void* ptr)
{
  ssfree_alloc(0, ptr);
}





















