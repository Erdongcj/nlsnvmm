#ifndef SSALLOC_H
#define SSALLOC_H

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

#define SSALLOC_NUM_ALLOCATORS 2

#define SSALLOC_SIZE (4 * 1024 * 1024 * 1024LL)

void ssalloc_set(void* mem);
void ssalloc_init();
void ssalloc_offset(size_t size);
void* ssalloc_alloc(unsigned int allocator, size_t size);
void* ssalloc_aligned_alloc(unsigned int allocator, size_t alignment, size_t size);
void ssfree_alloc(unsigned int allocator, void* ptr);
void* ssalloc(size_t size);
void* ssalloc_aligned(size_t alignment, size_t size);

void ssfree(void* ptr);

#endif

