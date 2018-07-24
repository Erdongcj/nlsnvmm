#ifndef _GHEAP_HH
#define _GHEAP_HH

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

class threadHeap
{
public:
    threadHeap(SlabHeap_t* sheap)
        : slheap(sheap)
    { }
	void* pmalloc(size_t sz);
    void free(void* ptr);
    size_t getsize(void* ptr);
	void* get_first_ptr();
private:
    SlabHeap_t* slheap; 
};

class gHeap {
public:

    int initial();
    threadHeap* threadheap();

private:
    ExtentHeap_t* exheap_;
    SlabHeap_t* slheap_;
    size_t bigsize_;
};
#endif // _HEAP_HH
