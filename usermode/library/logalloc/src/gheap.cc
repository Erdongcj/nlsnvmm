#include "gheap.hh"

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <mnemosyne.h>
#include <mutex>

__attribute__ ((section("PERSISTENT"))) void* PREGION_BASE = 0;

int gHeap::initial()
{
    Context ctx;
    unsigned long long n_gb = 20;
    unsigned long long region_size = 1024*1024*1024;
    region_size *= n_gb; 
    size_t block_log2size = 13;
    size_t slabsize = 1 << block_log2size;
    
    bigsize_ = slabsize;
    if (PREGION_BASE == 0) {
		PREGION_BASE = (void*) m_pmap((void *) PREGION_BASE, region_size, PROT_READ|PROT_WRITE, 0);
        void* region = PREGION_BASE;
        exheap_ = ExtentHeap_t::make(region, region_size, block_log2size);
    } else {
        void* region = PREGION_BASE;
        exheap_ = ExtentHeap_t::load(region);
    }

    slheap_ = new SlabHeap_t(slabsize, NULL, exheap_);
    slheap_->init(ctx);
}

threadHeap* gHeap::threadheap()
{

    threadHeap* thp = new threadHeap(slheap_);
    return thp;
}
/*
void* ThreadHeap::pmalloc(size_t sz)
{
    Context ctx(true, true);
    void* ptr;
    ptr = slheap->malloc(ctx, sz);
//	printf("ptrin the pmalloc is ....%d\n", ptr);
	return ptr;
}
*/

void* threadHeap::pmalloc(size_t sz)
{
	Context ctx(true, true);
	TPtr<void> ptr;
	ErrorCode rc = slheap->malloc(ctx, sz, &ptr);
	if (rc != kErrorCodeOk) {
		return NULL;
	}
	return ptr.get();
}


void threadHeap::free(void* ptr) 
{
//    Context ctx(false, true);
    printf("ptr is free%d\n", ptr);
    slheap->free_slab(ptr);
}

void* threadHeap::get_first_ptr()
{
	return slheap->get_first_ptr();
}
/*
size_t ThreadHeap::getsize(void* ptr)
{
    Context ctx(true, true);

    return slheap->getsize(ptr);
}
*/

