#ifndef _ATOMIC_H_
# define _ATOMIC_H_

# include <atomic_ops.h>

#  define ATOMIC_CAS_FULL(a, e, v)      (AO_compare_and_swap_full((volatile AO_t *)(a), (AO_t)(e), (AO_t)(v)))
#  define ATOMIC_FETCH_INC_FULL(a)      (AO_fetch_and_add1_full((volatile AO_t *)(a)))
#  define ATOMIC_FETCH_DEC_FULL(a)      (AO_fetch_and_sub1_full((volatile AO_t *)(a)))
#  define ATOMIC_FETCH_ADD_FULL(a, v)   (AO_fetch_and_add_full((volatile AO_t *)(a), (AO_t)(v)))
#  ifdef SAFE
#   define ATOMIC_LOAD_ACQ(a)           (AO_load_full((volatile AO_t *)(a)))
#   define ATOMIC_LOAD(a)               (AO_load_full((volatile AO_t *)(a)))
#   define ATOMIC_STORE_REL(a, v)       (AO_store_full((volatile AO_t *)(a), (AO_t)(v)))
#   define ATOMIC_STORE(a, v)           (AO_store_full((volatile AO_t *)(a), (AO_t)(v)))
#   define ATOMIC_MB_READ               AO_nop_full()
#   define ATOMIC_MB_WRITE              AO_nop_full()
#   define ATOMIC_MB_FULL               AO_nop_full()
#  else /* ! SAFE */
#   define ATOMIC_LOAD_ACQ(a)           (AO_load_acquire_read((volatile AO_t *)(a)))
#   define ATOMIC_LOAD(a)               (*((volatile AO_t *)(a)))
#   define ATOMIC_STORE_REL(a, v)       (AO_store_release((volatile AO_t *)(a), (AO_t)(v)))
#   define ATOMIC_STORE(a, v)           (*((volatile AO_t *)(a)) = (AO_t)(v))
#   define ATOMIC_MB_READ               AO_nop_read()
#   define ATOMIC_MB_WRITE              AO_nop_write()
#   define ATOMIC_MB_FULL               AO_nop_full()
#  endif /* ! SAFE */

// 1
#endif /* _ATOMIC_H_ */
