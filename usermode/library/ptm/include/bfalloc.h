#ifndef BFALLOC_H_
#define BFALLOC_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sched.h>
#include <inttypes.h>

#define BFALLOC_NUM_ALLOCATORS 2

#define BFALLOC_SIZE 4*512*512*1024LL

void bfalloc_init();
void bfalloc_offset(size_t size);
void* bfalloc_alloc(unsigned int allocator, size_t size);
void* bfalloc_aligned_alloc(unsigned int allocator, size_t alignment, size_t size);
void bffree_alloc(unsigned int allocator, void* ptr);
void* bfalloc(size_t size);
void* bfalloc_aligned(size_t alignment, size_t size);
void bffree(void* ptr);

#endif

