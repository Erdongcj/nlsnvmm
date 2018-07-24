#ifndef _HEAP_HH
#define _HEAP_HH

#include "pointer.hh"
#include "slabheap_1.hh"
#include "extentheap.hh"
#include "extentmap.hh"
extern "C"
{
#include <mnemosyne.h>
}
class Context {
public:
    Context(bool _do_v = true, bool _do_nv = true)
        : do_v(_do_v),
          do_nv(_do_nv)
    {   
        //Transaction *tran;
        //struct tx* td;
        //Transaction* tx=tran->get_tx();
        //if (tx->status != TX_IDLE_) {
         // td = get_tx();
        //} else {
          //  td = NULL;
        //}
    }

    void load(uint8_t* src, uint8_t *dest, size_t size)
    {
        //if (td=NULL) {
            //_ITM_nl_load_bytes(src, dest, size);
        //} else {
            memcpy(dest, src, size);
        //}
    }

    void store(uint8_t* src, uint8_t *dest, size_t size)
    {
      //  if (td=NULL) {
            //_ITM_nl_store_bytes(src, dest, size);
        //} else {
            memcpy(dest, src, size);
        //}
    }

    bool do_v;
    bool do_nv;
    //Tx* td;
    //Transaction * td;
};

typedef SlabHeap<Context, TPtr, PPtr> SlabHeap_t;
typedef ExtentHeap<Context, TPtr, PPtr> ExtentHeap_t;

class ThreadHeap
{
public:
    ThreadHeap(SlabHeap_t* sheap)
        : slheap(sheap)
    { }
	void* pmalloc(size_t sz);
//    void* pmalloc(size_t sz, void* p);
    void free(void* ptr);
    size_t getsize(void* ptr);
private:
    SlabHeap_t* slheap; 
};

class Heap {
public:

    int init();
    ThreadHeap* threadheap();

private:
    ExtentHeap_t* exheap_;
    SlabHeap_t* slheap_;
    size_t bigsize_;
};
#endif // _HEAP_HH
