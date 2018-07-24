#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#define FLUSH_ALIGN ((uintptr_t)64)
#define M_PCM_CPUFREQ      1200
#define EXTRA_SCM_LATENCY 500 
#define NS2CYCLE(__ns)     ((__ns) * M_PCM_CPUFREQ / 1000)

#define _mm_clflush(addr)\
	asm volatile("clflush %0" : "+m" (*(volatile char *)(addr)))


static inline unsigned long long asm_rdtsc(void)
{
    unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

static inline void emulate_latency_ns(uint64_t ns)
{
	uint64_t cycles, start, stop;
	start = asm_rdtsc();
	cycles = NS2CYCLE(ns);

	do { 
		stop = asm_rdtsc();
	} while (stop - start < cycles);
}

static void
nvm_flush(const void *addr, size_t len)
{
	uintptr_t uptr;

	for (uptr = (uintptr_t)addr & ~(FLUSH_ALIGN - 1);
			uptr < (uintptr_t)addr + len; uptr += FLUSH_ALIGN){
		_mm_clflush((char *)uptr);
		emulate_latency_ns(EXTRA_SCM_LATENCY);
	}

}
