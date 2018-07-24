#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

#define FLUSH_ALIGN ((uintptr_t)64)
#define HAS_SCM_LATENCY
#define EXTRA_SCM_LATENCY 600

#define _mm_clflush(addr)\
    asm volatile("clflush %0" : "+m" (*(volatile char *)(addr)))

static __inline void full_fence() {
    __asm__ __volatile__ ("mfence" ::: "memory");
}


static inline void NanosecondSleep(uint64_t sleeptime){
	struct timespec T0,T1;
	uint64_t temp = 0;
	clock_gettime(CLOCK_MONOTONIC, &T0);
		while(temp < sleeptime){
			clock_gettime(CLOCK_MONOTONIC,&T1);
			temp = (T1.tv_sec - T0.tv_sec) * 1000000000 + T1.tv_nsec - T0.tv_nsec;
		}
}

void nvm_flush(const void *addr, size_t len)
{
	uintptr_t uptr;

	for (uptr = (uintptr_t)addr & ~(FLUSH_ALIGN - 1); uptr < (uintptr_t)addr + len; uptr += FLUSH_ALIGN) {
		_mm_clflush((char *)uptr);
#ifdef HAS_SCM_LATENCY
	NanosecondSleep(EXTRA_SCM_LATENCY);
#endif
	}
}
