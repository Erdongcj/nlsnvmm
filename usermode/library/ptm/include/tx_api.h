#ifndef __TX_API_H__
#define __TX_API_H__

#include <stddef.h>
#include <typeinfo>
#include <stdint.h>

#include "buffer.h"
#include "log_handle.h"
//#include "gc_restore.h"

#define TX_BEGIN tx_begin();
#define TX_ADD(o, b) tx_add(o, b)
#define TX_END tx_end();
#define TX_RW(o) tx_rw(o)
#define TX_READ(o) tx_read(o)
#define TX_RO(o) tx_ro(o)

void init_thread(threadID_t* threadID, int flag);
void tx_begin();
bool tx_add(void* addr, size_t size);
void tx_end();
void* tx_rw(void *addr);
void* tx_read(void *addr);
void* tx_ro(void* addr);
//void* gc_concurrent();
///void* restore_concurrent();
#endif
