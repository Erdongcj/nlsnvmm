#ifndef __LOG_HANDLE_H__
#define __LOG_HANDLE_H__

#include <stdint.h>
//#include <wrapper.hh>
#include "buffer.h"
#include "tx.h"
/*
struct entry_head{
	void* haddr;
	int version;
    size_t size;
};
*/
int validate(Transaction *tx);
static inline int snapshot_extend (Transaction *tx);
void* has_read(Transaction* tx, void* addr);
void* log_append(Transaction* tx, size_t& len, uintptr_t t);
bool write_to_buffer(Transaction* tx, void* addr, size_t size);
void new_ht_ops(Transaction* tx, uintptr_t t);
void* read_from_buffer(Transaction* tx, void *addr);
void* log_find(Transaction* tx, void *addr);
//void* log_alloc(size_t size);
//void* find(void* addr);
void* find(Transaction* tx, void* addr);
#endif
