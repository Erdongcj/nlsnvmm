#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <numa.h>
#include <pthread.h>
#include <malloc.h>
#include "alloc_init.h"

static __thread void* alloc_addr;
static __thread uint64_t total_size;
void alloc_init()
{
    void* base_addr = numa_alloc_local(ALLOC_SIZE);
//	printf("base_addr is .....................%d\n", base_addr);
	alloc_addr = base_addr;
}
	   
void* alloc_fast(size_t size)
{
	void* ptr = alloc_addr;
	uint64_t temp;
	temp = reinterpret_cast<uint64_t>(alloc_addr) + size;
	alloc_addr = reinterpret_cast<void*>(temp);
	total_size += size;
	if(total_size >= ALLOC_SIZE){
		printf("alloc again\n");
		alloc_init();
		total_size = 0;
		return alloc_addr;
	}
	return ptr;
}
