#include <stdlib.h>
#include <string.h>

#include <mutex>

#include "gheap.hh"

thread_local threadHeap* threadheap = NULL;
static gHeap* heap;
extern std::mutex heapmtx;
inline static gHeap * getHeap (void) 
{
    heapmtx.lock();
    if (heap) {
        heapmtx.unlock();
        return heap;
    }
    heap = new gHeap();
    heap->initial();
    heapmtx.unlock();
    return heap;
}

inline static threadHeap* getThreadHeap (void)
{
    if (threadheap) {
        return threadheap;
    }
    gHeap* heap = getHeap();
    return heap->threadheap();
}
extern "C"
void* log_malloc(size_t sz)
{
	threadHeap *heap = getThreadHeap();
	void* addr = heap->pmalloc(sz);
    return addr;
}
extern "C"
void free_slab(void* ptr)
{
	threadHeap *heap = getThreadHeap();
	heap->free(ptr);
}

void* get_head()
{
	threadHeap *heap = getThreadHeap();
	return heap->get_first_ptr();
}

