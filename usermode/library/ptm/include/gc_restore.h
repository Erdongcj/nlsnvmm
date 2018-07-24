#ifndef GC_RESTORE_H
#define GC_RESTORE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tx.h"
//#include <wrapper.hh>
//#include "log_handle.h"
void* log_clean_concurrent();
void* crash_restore_concurrent();

#endif
