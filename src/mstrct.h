/**
 *  Copyright (C) 2026 @sadvadan (https://github.com/sadvadan)
 *
 *  memstruct v0.1.0
 *  "Zero Cost Abstraction"  -std=gnu99 & onws
 *
 **SUMMARY*******************************************
 *
 *   A robust, lightweight & fast single file library
 *  exposing a single, thin, convenience macro $(...)
 *  to ensure memory safety in the calling program.
 *
 *   100% compatible with existing code base ABI, and
 *  also with unguarded source at the same code site;
 *  Leverages both compile & runtime methods to check
 *  bounds/UAF/NULL-deref/double-free & memory-leaks.
 *
 *  with flag NMSTRCT checks can be disabled. there's
 *  also hardening flag MSTRCT_L to suit err handling
 *
 **BASIC*USAGE*************************************************
 *
 * NOTATION                    TYPE        EFFECT
 * --------                    -------     ------
 * $(name)                     L-value     meta data
 * $(name, index)              L-value     data[index]
 * $(type, name, range, addr)  declare     bind memory
 *
 **/

#ifndef MSTRCT_H
#define MSTRCT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MSTRCT_VER_MAJOR  0
#define MSTRCT_VER_MINOR  1
#define MSTRCT_VER_PATCH  0

#define MSTRCT_DEF1(a)          #a
#define MSTRCT_DEF2(a,b)        a##b
#define MSTRCT_CAT1(a)          MSTRCT_DEF1(a)
#define MSTRCT_CAT2(a,b)        MSTRCT_DEF2(a,b)
#define MSTRCT_CAT3(a,b,c)      MSTRCT_CAT2(a, MSTRCT_CAT2(b,c))
#define MSTRCT_CAT4(a,b,c,d)    MSTRCT_CAT2(MSTRCT_CAT2(a,b), MSTRCT_CAT2(c,d))
#define MSTRCT_CAT5(a,b,c,d,e)  MSTRCT_CAT2(a, MSTRCT_CAT4(b,c,d,e))

#define MSTRCT_MACR16(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,NAME,...) NAME

#define MSTRCT_ARG_COUNT(...) MSTRCT_MACR16(16 __VA_OPT__(,) ##__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define $(...) MSTRCT_CAT2(MSTRCT_$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#define MSTRCT_$10(...)       MSTRCT_GET_
#define MSTRCT_$9(...)        MSTRCT_GET_
#define MSTRCT_$8(...)        MSTRCT_GET_
#define MSTRCT_$7(...)        MSTRCT_GET_
#define MSTRCT_$6(...)        MSTRCT_GET_
#define MSTRCT_$5(...)        MSTRCT_GET_
#define MSTRCT_$0(...)        MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)
#define MSTRCT_GET_           MSTRCT_ASSERT(TOO_MANY_ARGS)

#define MSTRCT_$2(arg1, arg2) MSTRCT_CAT3(MSTRCT_$2$, MSTRCT_ARG_COUNT(arg1), MSTRCT_ARG_COUNT(arg2))(arg1, arg2)

#define MSTRCT_$2$00(arg1, arg2)  ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG); MSTRCT_WRONG_TYPE_OF_ARG;})

#define MSTRCT_$2$01(name, empty) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG); MSTRCT_WRONG_TYPE_OF_ARG;})

#define MSTRCT_$2$10(name, empty) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG); MSTRCT_WRONG_TYPE_OF_ARG;})

#define MSTRCT_$2$11(arg1, arg2) MSTRCT_GET(arg1, arg2)

#define MSTRCT_$3(typ, name, size)  \
MSTRCT_CAT4(MSTRCT_$3$, MSTRCT_ARG_COUNT(typ), MSTRCT_ARG_COUNT(name), MSTRCT_ARG_COUNT(size))(typ, name, size) 

#define MSTRCT_$3$000(typ, name, size)    ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$3$001(typ, name, size)    ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$3$100(typ, name, size)    ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$3$101(typ, name, size)    ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$3$010(typ, name, size)    ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$3$011(typ, name, size)    ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$3$111(typ, name, size)    ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})

#define MSTRCT_$4(typ, name, size, addr) MSTRCT_CAT5(  \
MSTRCT_$4$, MSTRCT_ARG_COUNT(typ), MSTRCT_ARG_COUNT(name), MSTRCT_ARG_COUNT(size), MSTRCT_ARG_COUNT(size))(typ,name,size,addr) 

#define MSTRCT_$4$0010(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$1000(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$0100(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$0001(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})

#define MSTRCT_$4$1010(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$1001(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$0110(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$0101(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})

#define MSTRCT_$4$0011(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$1100(typ, name, size, addr) MSTRCT_$3$110(typ, name, size)

#define MSTRCT_$4$1110(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$1011(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$1101(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})
#define MSTRCT_$4$0000(typ, name, size, addr) ({MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);})

#ifdef __clang__
  #define MSTRCT_X 1
#else
  #ifdef __GNUC__
    #define MSTRCT_X 0
  #endif
#endif

#ifdef NMSTRCT
  #define MSTRCT_C 0
#else
  #define MSTRCT_C 1
#endif

#ifdef MSTRCT_HARD
  #define MSTRCT_L 2                                  // print detailed err, exit program with mstrct_status code
#else
  #ifdef MSTRCT_STRICT
    #define MSTRCT_L 1                                // print detailed err, halt program with segfault at err site
  #else
    #define MSTRCT_L 0                                // print detailed err, continue with "start of the arr value"
  #endif
#endif

#ifdef __OPTIMIZE__
  #define MSTRCT_O 1
#else
  #define MSTRCT_O 0
#endif

const char* const mstrct_string = "mstrct_string";

typedef enum {
    MSTRCT_NULL                  = 1710,
    MSTRCT_USE_AFTER_FREE        = 1711,
    MSTRCT_BOUNDS_CHECK_FAIL     = 1712,
    MSTRCT_BAD_SYNTAX            = 1713,
    MSTRCT_BAD_CHECKSUM          = 1714,
    MSTRCT_WRONG_TYPE_OF_ARG     = 1715,
    MSTRCT_TOO_MANY_ARGS         = 1716,
    MSTRCT_ALLOC_FAIL            = 1717,
    MSTRCT_DE_ALLOC_FAIL         = 1718,
    MSTRCT_MULTI_DIM_BOUNDS_FAIL = 1719,
} mstrct_status;

static __thread uint64_t mstrct_u64 = UINT64_MAX;

static __thread char * mstrct_ptr  = (char *)1;

static const char * mstrct_abc;

__attribute__((constructor)) 
static void mstrct_shadow() {
  char* _abc;
  __asm__ __volatile__ (
    ".pushsection .bss.mstrct, \"aw\", @nobits \n\t"
    ".align 2097152\n\t"
    ".Lmstrct_asm:\n\t"
    ".zero 8\n\t"
    ".popsection\n\t"

    "leaq .Lmstrct_asm(%%rip), %[out]"
    : [out] "=r" (_abc)
    :
    : "memory"
  ); mstrct_abc = _abc; 
}

// memstruct
#define MSTRCT_T(type, card, range, line)  \
struct {  \
  union { \
    uint32_t id; \
    struct { \
      uint16_t _d; \
      uint16_t _s; \
      type typ[0] __attribute__((packed)); \
      struct {char a[line];} lin[0];   \
      MSTRCT_SIZ(range, type); \
      MSTRCT_FAC(card); \
      MSTRCT_CON(type); \
    };   \
  };  \
} __attribute__((aligned(4)))

#define MSTRCT_CON(type) MSTRCT_CAT2(MSTRCT_CON_, MSTRCT_O)(type)
#define MSTRCT_CON_0(type) struct {char a[0];} con[0]
#define MSTRCT_CON_1(type) struct {char a[(__builtin_strstr(__builtin_strrchr(#type, '*'), "const") != NULL)];} con[0]

#define MSTRCT_SIZ(range, type) MSTRCT_CAT2(MSTRCT_SIZ_, MSTRCT_O)(range, type)
#define MSTRCT_SIZ_0(range, type) struct {char a[0];} ran[0]
#define MSTRCT_SIZ_1(range, type) struct {   \
  char a[(__builtin_strstr(__builtin_strrchr(#type, '*'), "const") != NULL) * (__builtin_constant_p(range)) * (range)];} ran[0]

#define MSTRCT_FAC(card) MSTRCT_CAT2(MSTRCT_FAC_, MSTRCT_X)(card)
#define MSTRCT_FAC_0(card) struct {char a[card];} car[0] // gcc
#define MSTRCT_FAC_1(card) struct {char a[((__builtin_constant_p(card) == 0) || (card + 1)) - 1];} car[0] // clang

typedef struct {
  union {
    uint32_t id;
    struct {
      uint16_t _d; // low
      uint16_t _s; // high
    };
  };
} mstrct_proto;

typedef struct  {
  uint64_t addr;  // ptr addr
  const uint64_t  size; // range value
  const uint64_t  base; // base addr
} mstrct_meta;

#define MSTRCT_DEF_META(counter, size) { \
__asm__ (  \
  ".pushsection .bss.mstrct, \"aw\", @nobits \n\t" \
  ".ifndef .Lmstrct_label_%c0\n\t" \
  ".align 8\n\t"  \
  ".Lmstrct_label_%c0:\n\t" \
  ".zero %c1\n\t" \
  ".endif \n\t"   \
  ".popsection"   \
  : : "i" (counter), "i" (size));   \
}

#define MSTRCT_OFF(counter) ({uint64_t _diff; __asm__ (  \
  "movq $(.Lmstrct_label_%c[suffix] - .Lmstrct_asm), %0\n\t" : "=r" (_diff) : [suffix] "i" (counter)); _diff;})

__attribute__((const, always_inline))
static inline uint64_t mstrct_meta_addr1(int counter) {uint64_t _addr;
  __asm__ ("leaq .Lmstrct_label_%c[suffix](%%rip), %0" : "=r" (_addr) : [suffix] "i" (counter)); return _addr;
}

__attribute__((const))
static uint64_t mstrct_meta_addr2(uint64_t runtime_off) {uint64_t _addr;
  __asm__ ("leaq (%2,%1,8), %0\n\t" : "=r" (_addr) : "r" (runtime_off), "r" (mstrct_abc)); return _addr;
}

__attribute__((pure, always_inline))
static inline uint64_t mstrct_get10(int counter) {uint64_t _val;
  __asm__ ("movq .Lmstrct_label_%c[suffix](%%rip), %0" : "=r" (_val) : [suffix] "i" (counter)); return _val;
}

__attribute__((const, always_inline))
static inline uint64_t mstrct_get11(int counter) {uint64_t _val;
  __asm__ ("movq .Lmstrct_label_%c[suffix]+8(%%rip), %0" : "=r" (_val) : [suffix] "i" (counter)); return _val;
}

__attribute__((const, always_inline))
static inline uint64_t mstrct_get12(int counter) {uint64_t _val;
  __asm__ ("movq .Lmstrct_label_%c[suffix]+16(%%rip), %0" : "=r" (_val) : [suffix] "i" (counter)); return _val;
}

__attribute__((pure))
static uint64_t mstrct_get20(uint64_t runtime_off) {uint64_t _val;
  __asm__ ("movq (%2,%1,8), %0" : "=r" (_val) : "r" (runtime_off), "r" (mstrct_abc)); return _val;
}

__attribute__((const))
static uint64_t mstrct_get21(uint64_t runtime_off) {uint64_t _val;
  __asm__ ("movq 8(%2,%1,8), %0" : "=r" (_val) : "r" (runtime_off), "r" (mstrct_abc)); return _val;
}

__attribute__((const))
static uint64_t mstrct_get22(uint64_t runtime_off) {uint64_t _val;
  __asm__ ("movq 16(%2,%1,8), %0" : "=r" (_val) : "r" (runtime_off), "r" (mstrct_abc)); return _val;
}

__attribute__((const))
static uint64_t mstrct_get23(uint64_t runtime_off) {uint64_t _val;
  __asm__ ("movq 24(%2,%1,8), %0" : "=r" (_val) : "r" (runtime_off), "r" (mstrct_abc)); return _val;
}

__attribute__((const))
static uint64_t mstrct_get30(uint64_t runtime_off, uint64_t f_off) {
  __asm__ ("addq (%2,%1,8), %0" : "+r" (f_off) : "r" (runtime_off), "r" (mstrct_abc) : "cc"); return f_off;
}

#define MSTRCT_SET(value, runtime_off, local_off) __asm__ __volatile__ (   \
  "movq %[val], " MSTRCT_CAT1(local_off) "(%[base], %[off], 8)"   \
  : [dest] "=m" (*((char *)mstrct_abc + (runtime_off * 8) + (local_off)))  \
  : [val]  "r"  (value),   \
    [base] "r"  (mstrct_abc), \
    [off]  "r"  ((uint64_t)runtime_off)   \
)

// force return
#define MSTRCT_RET() __asm__ __volatile__ ("movl $0, %%eax\n\t" "leave\n\t" "ret" : : : "eax")

static void mstrct_error(const char *ops, const int err_no, const int line, const char *file) {
  printf("MSTRCT ERR: %s; originated at line: %d, file: %s; err status: %d\n", ops, line, file, err_no);
  if (MSTRCT_L == 2) {exit(err_no);} else if (MSTRCT_L == 1) {MSTRCT_RET();}
}

#define MSTRCT_ASSERT(ops) MSTRCT_CAT2(MSTRCT__, ops)()

typedef void (*mstrct_func)(void);

static void MSTRCT__NULL(void) {(void)0;};
__attribute__((weak, warning("MSTRCT ERR: use after free (UAF) or NULL dereference!")))
void MSTRCT__USE_AFTER_FREE(void);
__attribute__((weak, warning("MSTRCT ERR: array bounds check FAIL!")))
void MSTRCT__BOUNDS_CHECK_FAIL(void);
__attribute__((weak, warning("MSTRCT ERR: mstrct.h doesn't support multidim VLAs in clang (try gcc)!!")))
void MSTRCT__BAD_SYNTAX(void);
__attribute__((weak, warning("MSTRCT ERR: checksum (type_size)x(name_cardinality)x(range) = total_syze_in_bytes, SEEMS OFF!")))
void MSTRCT__BAD_CHECKSUM(void);
__attribute__((weak, warning("MSTRCT ERR: invalid type or INCOMPATIBLE arg(s)!")))
void MSTRCT__WRONG_TYPE_OF_ARG(void);
__attribute__((weak, warning("MSTRCT ERR: too MANY args!")))
void MSTRCT__TOO_MANY_ARGS(void);
__attribute__((weak, warning("MSTRCT ERR: name cardinality (either multi or single) CAN'T exceed UINT16_MAX!!")))
void MSTRCT__ALLOC_FAIL(void);

static mstrct_func mstrct_err_tab[] = { // err vtable
  MSTRCT__NULL,
  MSTRCT__USE_AFTER_FREE,
  MSTRCT__BOUNDS_CHECK_FAIL,
  MSTRCT__BAD_SYNTAX,
  MSTRCT__BAD_CHECKSUM,
  MSTRCT__WRONG_TYPE_OF_ARG,
  MSTRCT__TOO_MANY_ARGS,
  MSTRCT__ALLOC_FAIL
};

__attribute__((always_inline))
static inline void mstrct_warn(int err_cond, int err_code, const char *err_msg, int line, const char *file) {
  switch (__builtin_constant_p(err_cond)) {
      case 0: if (err_cond) {mstrct_error(err_msg, err_code, line, file);}; return;
      case 1: mstrct_err_tab[err_cond * (err_code - MSTRCT_NULL)](); return;
      default: __builtin_unreachable();
  }
}


static void mstrct_leak(int status, void *ptr) {
  if (mstrct_get20((uint16_t)(uint64_t)ptr) != 0) {
    const char *file = (const char *)((uint64_t)mstrct_string + (((uint64_t)ptr)>> 32));
    printf("MSTRCT ERR: MEMORY_LEAK; for memory originated at line: %d, file: %s; exit status: %d\n",
    (int)(((uint32_t)(uint64_t)ptr) >> 16), file, status);
  }
}

__attribute__((always_inline))
static inline void mstrct_cleanup(uint16_t *ptr) {
  if (*ptr != UINT16_MAX) {
    uintptr_t rsp; __asm__("mov %%rsp, %0" : "=r"(rsp));
    if ((int64_t)((char *)rsp - (char *)ptr) < 0) { // only cater to stack vars
      MSTRCT_SET((uint64_t)0, (uint16_t)(*ptr), 0); // scope end: make ptr addr NULL
    }
  }
}

__attribute__((unused))
static void mstrct_user_free(mstrct_proto *arg, int flag) {
  switch (flag) {
    case 0: {
      mstrct_proto name = *arg;
      void *addr = (void *)mstrct_get20(name._d); 
      if (addr != NULL) {(free)((void *)mstrct_get22(name._d));}
      MSTRCT_SET((uint64_t)0, name._d, 0); break;
    }
    case 1: {
      void *addr = *(void **)(arg);
      if (addr != NULL) {
        (free)(addr); addr = NULL;
      } break;
    }
    default: __builtin_unreachable();
  }
}

// overload free()
#define free(arg) MSTRCT_FREE(arg)

#define MSTRCT_FREE(arg) mstrct_user_free(((mstrct_proto *)&arg), (__builtin_classify_type(arg) == 5))

extern int munmap(void *addr, size_t len); // fwd declare munmap

__attribute__((unused))
static uint64_t mstrct_munmap_1(mstrct_proto *arg, int line, const char *file) {
  mstrct_proto name = *arg; int temp = 0;
  void *addr = (void *)mstrct_get20(name._d); 
  if (addr != NULL) {
    if ((munmap)((void *)mstrct_get22(name._d), mstrct_get21(name._d)) == 0) {
      MSTRCT_SET((uint64_t)0, name._d, 0); MSTRCT_SET((uint64_t)0, name._d, 8); return temp;
    } else {
       temp = 1; mstrct_error("de-allocation failed!!", MSTRCT_DE_ALLOC_FAIL, line, file); return temp;
    }
  } else return temp;
}

__attribute__((unused))
static uint64_t mstrct_munmap_2(void **arg, uint64_t size, int line, const char *file) {
  int temp = 0; void *addr = *(void **)(arg);
  if (addr != NULL) {
    if ((munmap)(addr, size) == 0) {addr = NULL; return temp;}
    else {temp = 1; mstrct_error("de-allocation failed!!", MSTRCT_DE_ALLOC_FAIL, line, file); return temp;}
  } else return temp;
}

// overload munmap
#define munmap(...)  MSTRCT_CAT2(MSTRCT_MUNMAP_, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#define MSTRCT_MUNMAP_1(arg) mstrct_munmap_1(((mstrct_proto *)&arg), __LINE__, __FILE__)
#define MSTRCT_MUNMAP_2(arg, size) mstrct_munmap_2((void **)&arg, size, __LINE__, __FILE__)
#define MSTRCT_MUNMAP_3(arg1, arg2, arg3) MSTRCT_ASSERT(TOO_MANY_ARGS)
#define MSTRCT_MUNMAP_4(arg1, arg2, arg3, arg4) MSTRCT_ASSERT(TOO_MANY_ARGS)
#define MSTRCT_MUNMAP_0() MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)

__attribute__((always_inline))
static inline uint64_t mstrct_meta_addr(uint64_t id_s, uint64_t id_d, uint64_t f_size, int line, const char *file) {
  switch (__builtin_constant_p(id_s)) {
    case 1: return mstrct_meta_addr1(id_s);
    case 0:
      switch (f_size) {
        case 0: return 0; // clang multidim VLA
        case 1: return mstrct_meta_addr2(id_d); // cardinality = 1
        default:
          switch (id_s) {
            case 0: return mstrct_meta_addr2(id_d); // start of multidim arr
            default: {mstrct_error("No unique metadata exists for individual elements of a multidim arr (hint: input w/o indexes)",
                      MSTRCT_BAD_SYNTAX, line, file); return 0;}
          }
      }
    default: __builtin_unreachable();
  }
}

__attribute__((always_inline))
static inline uint64_t mstrct_addr(uint64_t con, const uint64_t id_s, const uint64_t id_d, const uint64_t f_size) {
  switch (__builtin_constant_p(id_s)) {
    case 1:
      switch (con) {
        case 0:  return mstrct_get10(id_s); // mutable addr
        default: return mstrct_get12(id_s); // const addr
      }
    case 0:
      switch (f_size) {
        case 0: return 0; // clang multidim VLA
        case 1: 
          switch (con) { // cardinalty = 1
            case 0:  return mstrct_get20(id_d); // mutable addr
            default: return mstrct_get22(id_d); // const addr
          }
        default: {return mstrct_get30(id_d, (f_size * id_s));} // name factory
      }
    default: __builtin_unreachable();
  }
}

static void mstrct_bounds_error(uint64_t ptr, int line, const char *file) {
  if (ptr == 0) {mstrct_error("USE_AFTER_FREE", MSTRCT_USE_AFTER_FREE, line, file);}
  else {mstrct_error("BOUNDS_CHECK_FAIL", MSTRCT_BOUNDS_CHECK_FAIL, line, file);};
}

__attribute__((const, always_inline))  
static inline int mstrct_check_dyna_cal(uint64_t addr, uint64_t size, uint64_t base) {
  return __builtin_expect(((uint64_t)(addr - base) <= size), 1);
}

__attribute__((always_inline)) static inline uint64_t
mstrct_check_dyna(uint64_t addr, uint64_t size, uint64_t base, uint16_t id_d, int line, const char *file) {
  if (mstrct_check_dyna_cal(addr, size, base)) {return addr;}
  else {mstrct_bounds_error(mstrct_get20(id_d), line, file); if (MSTRCT_L == 0) return (base + size);}
}

__attribute__((always_inline)) static inline uint64_t
mstrct_dyna(uint16_t id_s, uint16_t id_d, uint64_t addr, int line, const char *file) {
  switch (__builtin_constant_p(id_s)) {
      case 0: return mstrct_check_dyna(addr, mstrct_get21(id_d), mstrct_get22(id_d), id_d, line, file);
      case 1: return mstrct_check_dyna(addr, mstrct_get11(id_s), mstrct_get12(id_s), id_d, line, file);
      default: __builtin_unreachable();
  }
}

__attribute__((always_inline))
static inline uint64_t mstrct_check_stat(uint64_t n, uint64_t N, uint64_t addr, uint16_t _d, int line, const char *file) {
  if (n < N) {return addr;}
  else {
    addr = (mstrct_get21(_d) + mstrct_get22(_d)); // default
    mstrct_warn((n >= N), MSTRCT_BOUNDS_CHECK_FAIL, "bounds check failed!!", line, file);
    if (MSTRCT_L == 0) return addr;
  }
}

__attribute__((always_inline)) static inline uint64_t
mstrct_fact_oob(uint64_t type_line, uint16_t _d, int line, const char *file, uint64_t addr) {
  if (mstrct_get23(_d) != type_line) {
    addr = (mstrct_get21(_d) + mstrct_get22(_d)); // default
    mstrct_error("multi-dim-arr index failed bounds check!!", MSTRCT_MULTI_DIM_BOUNDS_FAIL, line, file);
    if (MSTRCT_L == 0) return addr;
  } else return addr;
}

__attribute__((always_inline)) static inline uint64_t mstrct_check(uint64_t type_range, uint64_t f_range, uint64_t index,
  int line, const char *file, uint64_t type_line, uint16_t _s, uint16_t _d, uint64_t addr_in) {uint64_t addr = addr_in;
  if (f_range > 1) {addr = mstrct_fact_oob(type_line, _d, line, file, addr);}
  if ((type_range > 0) && (__builtin_constant_p(_s))) {addr = mstrct_check_stat(index, type_range, addr, _d, line, file);}
  else {addr = mstrct_dyna(_s, _d, addr, line, file);};
  return addr;
}


#define MSTRCT_$1(name) \
  (*(struct {typeof(name.typ[0]) addr; const uint64_t size; typeof(({(typeof(name.typ[0]))0;})) const base;} *)   \
  mstrct_meta_addr((uint64_t)name._s, (uint64_t)name._d, (sizeof(name.car[0])), __LINE__, __FILE__))

#define MSTRCT_GET(name, index) MSTRCT_CAT2(MSTRCT_GET__, MSTRCT_C)(name, index)

#define MSTRCT_GET__0(name, index) (*((typeof(name.typ[0])) \
   (mstrct_addr(sizeof(name.con[0]), (uint64_t)name._s, (uint64_t)name._d, (sizeof(name.car[0])))) + (index)))


#define MSTRCT_GET__1(name, index) (*((typeof(name.typ[0])) (  \
  mstrct_check(sizeof(name.ran[0]), sizeof(name.car[0]), index, __LINE__, __FILE__, sizeof(name.lin[0]), name._s, name._d, \
  (uint64_t)((typeof(name.typ[0]))(mstrct_addr(sizeof(name.con[0]), name._s, name._d, (sizeof(name.car[0])))) + index + 1)) \
) - 1))

#define MSTRCT_$3$110(typ, name, empty) MSTRCT_T(typ, sizeof(struct {char name;}), 0, __LINE__) name


#define MSTRCT_$4$1111(typ, name, range, addr) MSTRCT_CAT2(MSTRCT_SET_, MSTRCT_C)(typ, name, range, addr)

#define MSTRCT_SET_0(typ, name, range, addr) \
  mstrct_ptr = (char *)(addr);  \
  MSTRCT_T(typ, sizeof(struct {char name;}), range, __LINE__) name = {0};  \
  MSTRCT_LET(name, range, sizeof(addr))

#define MSTRCT_SET_1(typ, name, range, addr)  \
  mstrct_ptr = (char *)(addr);  \
  MSTRCT_T(typ, sizeof(struct {char name;}), range, __LINE__) name = {0};  \
  __attribute__((cleanup(mstrct_cleanup))) uint16_t MSTRCT_CAT2(mstrct_s_,__LINE__) = UINT16_MAX; \
  MSTRCT_LET(name, range, sizeof(addr))


#define MSTRCT_$4$0111(empty, name, range, addr) MSTRCT_CAT2(MSTRCT_RET_, MSTRCT_C)(empty, name, range, addr)

#define MSTRCT_RET_0(empty, name, range, addr)  \
  mstrct_ptr = (char *)(addr); /* allocation; next: tackle realloc types */   \
  if ((mstrct_get22(name._d) == 0) || (mstrct_get22(name._d) == (uint64_t)mstrct_ptr)) {MSTRCT_SET((uint64_t)0, name._d, 0);} \
  MSTRCT_LET(name, range, sizeof(addr))

#define MSTRCT_RET_1(empty, name, range, addr)  \
  mstrct_ptr = (char *)(addr); /* allocation; next: tackle realloc types */   \
  if ((mstrct_get22(name._d) == 0) || (mstrct_get22(name._d) == (uint64_t)mstrct_ptr)) {MSTRCT_SET((uint64_t)0, name._d, 0);} \
  __attribute__((cleanup(mstrct_cleanup))) uint16_t MSTRCT_CAT2(mstrct_s_, __LINE__) = 0;   \
  MSTRCT_LET(name, range, sizeof(addr))

#define MSTRCT_CLEANUP(line, off) MSTRCT_CAT2(MSTRCT_CLEANUP_, MSTRCT_C)(line, off)
#define MSTRCT_CLEANUP_0(line, off) (void)0
#define MSTRCT_CLEANUP_1(line, off) MSTRCT_CAT2(mstrct_s_, line) = off

// suppress false Warray-bounds diagnostics
#define MSTRCT_PRAG_ON  _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
#define MSTRCT_PRAG_OFF _Pragma("GCC diagnostic pop")

#define MSTRCT_LET(name, range, allo_size) if (mstrct_ptr != (char *)1 && mstrct_ptr != NULL) {   \
  enum {enm = __COUNTER__, card = sizeof(name.car[0])}; uint16_t off = (MSTRCT_OFF(enm))/8; char a;  \
  MSTRCT_PRAG_ON if (card == 1) {name._s = enm; MSTRCT_DEF_META(enm, 24);} else {MSTRCT_DEF_META(enm, 32);} MSTRCT_PRAG_OFF   \
  if (((int64_t)(&a - mstrct_ptr)) < 0) {MSTRCT_CLEANUP(__LINE__, off);} \
  mstrct_u64 = MSTRCT_BASE(name, card); uint64_t size = (sizeof(*(name.typ[0])) * card * range); \
  mstrct_checksum(size, allo_size, __LINE__, __FILE__); mstrct_let((char *)&(name), size, off, __LINE__, __FILE__, card, enm);  \
} else if (mstrct_ptr == NULL) { \
  mstrct_ptr = (char *)1;  \
  mstrct_error("allocation failed!!", MSTRCT_ALLOC_FAIL, __LINE__, __FILE__); \
}

static void mstrct_leak_1(mstrct_proto * name, int line, const char *file) {
  uintptr_t rsp; __asm__("mov %%rsp, %0" : "=r"(rsp));
  if ((int64_t)((char *)rsp - mstrct_ptr) > 0) /* not on stack */ {
    mstrct_proto temp; temp._d = name->_d; temp._s = (uint16_t)line;
    uint64_t diff = (uint64_t)file - (uint64_t)mstrct_string;
    uint64_t packed = (temp.id) | (diff << 32);
    on_exit(mstrct_leak, (void *)(packed));
  } 
}

__attribute__((unused)) static void mstrct_leak_0(__attribute__((unused)) mstrct_proto * name,
__attribute__((unused)) int line, __attribute__((unused)) const char *file) {(void)0;}

static void mstrct_factory(mstrct_proto * name, uint64_t card) {
  uint16_t offset = name->_d;
  for (uint64_t i = 0; i < (card); i++) {
    (name + i)->_s = i; // carries local_offset
    (name + i)->_d = offset; // store metadata offset
  }
}

#define MSTRCT_BASE(name, card) ({  \
    struct {char name;} *dummy = NULL; \
    ((uint64_t)((char *)(&(dummy->name)) - (char *)dummy));  \
})

__attribute__((always_inline))
inline static void mstrct_checksum(uint64_t size, uint64_t allosize, int line, const char *file) {
  ssize_t obj_size = (ssize_t)__builtin_dynamic_object_size(mstrct_ptr, 0);
  if (obj_size > 0) {
    mstrct_warn(((uint64_t)obj_size != size), MSTRCT_BAD_CHECKSUM,
    "checksum: (type_size) x (name_cardinality) x (range) = total_syze_in_bytes, SEEMS OFF!", line, file);
  } else if (allosize != sizeof(void *)) {
    mstrct_warn((allosize != size), MSTRCT_BAD_CHECKSUM,
    "checksum: (type_size) x (name_cardinality) x (range) = total_syze_in_bytes, SEEMS OFF!", line, file);
  } else if (allosize < sizeof(void *)) {
    mstrct_warn((allosize < sizeof(void *)), MSTRCT_ALLOC_FAIL,
    "allocation failed because of invalid addr input (must be a 64-bit valid address)!", line, file);
  }
}

static void mstrct_let( // helper func
  char *oob, uint64_t size, uint16_t off, int line, const char *file, uint64_t card, int enm) {

  mstrct_warn((enm > UINT16_MAX), MSTRCT_ALLOC_FAIL, "name cardinality __COUNT__ can't exceed UINT16_MAX!!", line, file);
  mstrct_warn((card > UINT16_MAX), MSTRCT_ALLOC_FAIL, "multi-dim name cardinality CAN'T exceed UINT16_MAX!", line, file);
  mstrct_warn((card == 0), MSTRCT_BAD_SYNTAX, "mstrct.h doesn't support multidim VLAs in clang (try gcc)!!", line, file); 

  mstrct_proto *name = (mstrct_proto *)oob - mstrct_u64;
  name->_d = off;
  MSTRCT_SET((uint64_t)mstrct_ptr, name->_d, 0);
  MSTRCT_SET(size, name->_d, 8);
  MSTRCT_SET((uint64_t)mstrct_ptr, name->_d, 16);
  MSTRCT_CAT2(mstrct_leak_, MSTRCT_C)(name, line, file); // register global leak check
  if (card > 1) {
    MSTRCT_SET((uint64_t)(uint16_t)line, name->_d, 24);
    mstrct_factory(name, card);
  }
  mstrct_u64 = UINT64_MAX; mstrct_ptr = (char *)1;
}


#endif
