#ifndef _MNEMOSYNE_WRAP_H
#define _MNEMOSYNE_WRAP_H

#include <stdlib.h>

#if __cplusplus
extern "C" {
#endif

#define hmalloc(x) mtm_pmalloc(x)
#define hfree(x) mtm_pfree(x)
//#define pcalloc _ITM_pcalloc
//#define prealloc _ITM_prealloc

void* mtm_pmalloc(size_t sz);
void mtm_pfree(void* ptr);

#if __cplusplus
}
#endif



#endif


