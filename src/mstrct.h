/**
 *  Copyright(C) 2026 @sadvadan (github.com/sadvadan)
 *
 *  memstruct v1.0.0
 *  "Zero Cost Abstraction"         -std=gnu99 & onws
 *
 **SUMMARY*******************************************
 *
 *  A lightweight & fast single header library with a
 *  m-macro API for memory safety in calling program.
 *
 *  Leverages compile & runtime methods, and hoisting
 *  to check OOB/UAF/NULL-deref/double-free and leaks
 *
 *  with flag NMSTRCT checks can be disabled. there's
 *  also hardening flag MSTRCT_L to suit err handling
 *
 **/

#ifndef MSTRCT_H
#define MSTRCT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MSTRCT_VER_MAJOR                  1
#define MSTRCT_VER_MINOR                  0
#define MSTRCT_VER_PATCH                  0

#define MSTRCT_DEF1(a)                    #a
#define MSTRCT_DEF2(a,b)                  a##b
#define MSTRCT_CAT1(a)                    MSTRCT_DEF1(a)
#define MSTRCT_CAT2(a,b)                  MSTRCT_DEF2(a,b)
#define MSTRCT_CAT3(a,b,c)                MSTRCT_CAT2(a, MSTRCT_CAT2(b,c))
#define MSTRCT_CAT4(a,b,c,d)              MSTRCT_CAT2(MSTRCT_CAT2(a,b), MSTRCT_CAT2(c,d))
#define MSTRCT_CAT5(a,b,c,d,e)            MSTRCT_CAT2(a, MSTRCT_CAT4(b,c,d,e))

#define MSTRCT_IDX1(i)                    [i]
#define MSTRCT_IDX2(i,j)                  [i][j]
#define MSTRCT_IDX3(i,j,k)                [i][j][k]
#define MSTRCT_IDX4(i,j,k,l)              [i][j][k][l]
#define MSTRCT_IDX5(i,j,k,l,m)            [i][j][k][l][m]
#define MSTRCT_IDX6(i,j,k,l,m,n)          [i][j][k][l][m][n]
#define MSTRCT_IDX7(i,j,k,l,m,n,o)        [i][j][k][l][m][n][o]
#define MSTRCT_IDX8(i,j,k,l,m,n,o,p)      [i][j][k][l][m][n][o][p]
#define MSTRCT_IDX9(i,j,k,l,m,n,o,p,q)    [i][j][k][l][m][n][o][p][q]
#define MSTRCT_IDX10(i,j,k,l,m,n,o,p,q,r) [i][j][k][l][m][n][o][p][q][r]

#define MSTRCT_$13(foo,...)               MSTRCT_ASSERT(MSTRCT_TOO_MANY_ARGS)
#define MSTRCT_$12(foo,...)               MSTRCT_ASSERT(MSTRCT_TOO_MANY_ARGS)
#define MSTRCT_$11(foo,...)               MSTRCT_GET(foo, MSTRCT_IDX10(__VA_ARGS__))
#define MSTRCT_$10(foo,...)               MSTRCT_GET(foo, MSTRCT_IDX9(__VA_ARGS__))
#define MSTRCT_$9(foo,...)                MSTRCT_GET(foo, MSTRCT_IDX8(__VA_ARGS__))
#define MSTRCT_$8(foo,...)                MSTRCT_GET(foo, MSTRCT_IDX7(__VA_ARGS__))
#define MSTRCT_$7(foo,...)                MSTRCT_GET(foo, MSTRCT_IDX6(__VA_ARGS__))
#define MSTRCT_$6(foo,...)                MSTRCT_GET(foo, MSTRCT_IDX5(__VA_ARGS__))
#define MSTRCT_$5(foo,...)                MSTRCT_GET(foo, MSTRCT_IDX4(__VA_ARGS__))
#define MSTRCT_$4(foo,...)                MSTRCT_GET(foo, MSTRCT_IDX3(__VA_ARGS__))
#define MSTRCT_$3(foo,...)                MSTRCT_GET(foo, MSTRCT_IDX2(__VA_ARGS__))
#define MSTRCT_$2(foo,...)                MSTRCT_GET(foo, MSTRCT_IDX1(__VA_ARGS__)[0])
#define MSTRCT_$1(foo)                    MSTRCT_GET0(foo)
#define MSTRCT_$0()                       MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)

#define MSTRCT_$$15(typ,foo,empty,...)    MSTRCT_ASSERT(MSTRCT_TOO_MANY_ARGS)
#define MSTRCT_$$14(typ,foo,empty,...)    MSTRCT_ASSERT(MSTRCT_TOO_MANY_ARGS)
#define MSTRCT_$$13(typ,foo,empty,...)    MSTRCT_LET(typ,foo,empty, MSTRCT_IDX10(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$12(typ,foo,empty,...)    MSTRCT_LET(typ,foo,empty, MSTRCT_IDX9(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$11(typ,foo,empty,...)    MSTRCT_LET(typ,foo,empty, MSTRCT_IDX8(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$10(typ,foo,empty,...)    MSTRCT_LET(typ,foo,empty, MSTRCT_IDX7(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$9(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX6(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$8(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX5(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$7(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX4(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$6(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX3(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$5(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX2(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$4(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX1(__VA_ARGS__), __COUNTER__)
#define MSTRCT_$$3(arg1,arg2,arg3)        MSTRCT_CAT3(MSTRCT_M3, MSTRCT_STORE(arg1), MSTRCT_ARG_COUNT(arg3))(arg1,arg2,arg3)
#define MSTRCT_$$2(arg1,arg2)             MSTRCT_CAT2(MSTRCT_M2, MSTRCT_STORE(arg1))(arg1,arg2)
#define MSTRCT_$$1(foo)                   MSTRCT_CAT2(MSTRCT_M1, MSTRCT_STORE(foo))(foo)
#define MSTRCT_$$0()                      MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)

#define MSTRCT_$$$3(store,foo,idx)        MSTRCT_PUT(store,foo,idx,__COUNTER__)
#define MSTRCT_$$$2(store,foo)            MSTRCT_PUT(store, foo, 1, __COUNTER__)
#define MSTRCT_$$$1(store)                MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)

#define MSTRCT_STORE(arg)                 MSTRCT_HAS_COMMA(MSTRCT_CAT2(MSTRCT_, arg)) /* 1=storage, 0=no_storage */
#define MSTRCT_AUTO(arg)                  MSTRCT_HAS_COMMA(MSTRCT_CAT2(_MSTRCT_, arg)) /* 1=auto, 0=not_auto */

#define MSTRCT_auto ~,1
#define MSTRCT_static ~,1
#define MSTRCT___thread ~,1
#define _MSTRCT___thread
#define _MSTRCT_static
#define _MSTRCT_auto ~,1

#define MSTRCT_ARG3(_1, _2, _3, ...) _3
#define MSTRCT_HAS_COMMA(...) MSTRCT_ARG3(__VA_ARGS__, 1, 0)

#define MSTRCT_MACR16(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...) NAME
#define MSTRCT_ARG_COUNT(...) MSTRCT_MACR16(10 __VA_OPT__(,) ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define MSTRCT_M10(foo) MSTRCT_LET1(foo)
#define MSTRCT_M11(store) MSTRCT_$$$1(store)

#define MSTRCT_M20(id, foo) MSTRCT_LET2(id, foo)
#define MSTRCT_M21(store, foo) MSTRCT_PUT(store, foo, 1, __COUNTER__)

#define MSTRCT_M301(memory, foo, idx) MSTRCT_LET3(memory, foo, idx, __COUNTER__)
#define MSTRCT_M311(store, foo, idx) MSTRCT_$$$3(store, foo, idx)
#define MSTRCT_M300(typ, foo, empty) MSTRCT_LET(typ, foo, empty, [1], __COUNTER__)
#define MSTRCT_M310(store, foo, empty) MSTRCT_$$$2(store,foo)

// API
#define m(...) MSTRCT_CAT2(MSTRCT_$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define M(...) MSTRCT_CAT2(MSTRCT_$$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#ifdef NMSTRCT
  #define MSTRCT_C 0
#else
  #define MSTRCT_C 1
#endif

#ifdef MSTRCT_HARD
  #define MSTRCT_L 2       // print detailed err, exit program with mstrct_status code
#else
  #ifdef MSTRCT_STRICT
    #define MSTRCT_L 1     // print detailed err, halt program with segfault at err site
  #else
    #define MSTRCT_L 0     // print detailed err, continue with "start of the arr value"
  #endif
#endif

#ifdef __OPTIMIZE__
  #define MSTRCT_O 1
#else
  #define MSTRCT_O 0
#endif

typedef enum {
  MSTRCT_NULL                  = 1700,
  MSTRCT_USE_AFTER_FREE        = 1711,
  MSTRCT_BOUNDS_CHECK_FAIL     = 1712,
  MSTRCT_WRONG_TYPE_OF_ARG     = 1713,
  MSTRCT_TOO_MANY_ARGS         = 1714,
  MSTRCT_ALLOC_FAIL            = 1715,
  MSTRCT_DE_ALLOC_FAIL         = 1716,
  MSTRCT_NON_EMPTY_THIRD_ARG   = 1717,
} mstrct_status;

extern char  **environ;
static __thread char * mstrct_ptr  = (char *)1;
static __thread char* mstrct_string = __FILE__;
static __thread char* mstrct_refstr = __FILE__;
static const char * mstrct_abc;

__attribute__((constructor)) static void
mstrct_shadow() {
  char* _abc = (char *)__COUNTER__;
  __asm__ __volatile__ (
    ".pushsection .bss.mstrct, \"aw\", @nobits \n\t"
    ".align 4096\n\t"
    ".Lmstrct_asm:\n\t"
    ".zero 8\n\t"
    ".popsection\n\t"
    ".pushsection .bss.mstrct.mstrct, \"aw\", @nobits \n\t"
    ".align 4096\n\t"
    ".Lmstrct_asm_asm:\n\t"
    ".zero 8\n\t"
    ".popsection\n\t"
    "leaq .Lmstrct_asm(%%rip), %[out]"
    : [out] "=r" (_abc)
    :
    : "memory"
  ); mstrct_abc = _abc; 

  __asm__ (
    ".macro MSTRCT.0 addr, counter\n\t"
      "leaq .Lmstrct.\\counter(%rip), \\addr\n\t"
    ".endm\n\t"
  );

  __asm__ (
    ".macro MSTRCT.1 offset, counter, addr, size\n\t"
      ".ifndef .Lmstrct.meta.\\counter\n\t"
        ".pushsection .bss.mstrct, \"aw\", @nobits \n\t"
          ".align 8\n\t"
          ".Lmstrct.meta.\\counter:\n\t"
          ".zero 16\n\t"
        ".popsection\n\t"
      ".endif \n\t"
      "movq \\addr, .Lmstrct.meta.\\counter(%rip)\n\t"
      "movq \\size, 8+.Lmstrct.meta.\\counter(%rip)\n\t"
      "movq $(.Lmstrct.meta.\\counter - .Lmstrct_asm), \\offset\n\t"
    ".endm\n\t"
  );

  __asm__ (
    ".macro MSTRCT.2 value, base, id\n\t"
      "movq \\value, 8(\\base, \\id, 8)\n\t"
    ".endm\n\t"
  );

  __asm__ (
    ".macro MSTRCT.3 counter\n\t"
      ".ifndef .Lmstrct.\\counter\n\t"
        ".pushsection .bss.mstrct.mstrct, \"aw\", @nobits \n\t"
          ".align 8\n\t"
          ".Lmstrct.\\counter:\n\t"
          ".zero 8\n\t"
        ".popsection\n\t"
      ".endif \n\t"
    ".endm\n\t"
  );

}

// memstruct; if dim=1: index=[i][j][k]...; elseif dim=0: index=[1]
#define MSTRCT_T(type, index, enm)  \
struct {  \
  uint16_t id; \
  /* ref[0] */ struct {char a[enm];} ref[0];     \
  /* typ[0] */ typeof(type) typ[0] __attribute__((packed)); \
  /* dim[0] */ char (*dim[0])[]index __attribute__((packed)); \
} __attribute__((aligned(2)))

typedef struct  {uint16_t id;} mstrct_proto;
typedef struct  {void *addr; uint64_t size;} mstrct_meta;

typedef struct {
  union {
    uint32_t id;
    struct {
      uint16_t _d; // low
      uint16_t _s; // high
    };
  };
} mstrct_pack;

#define MSTRCT_DEF_META(counter, addr, size) ({uint64_t offset;  \
__asm__ __volatile__ ("MSTRCT.1 %0, %c1, %2, %3" : "=r" (offset) : "i" (counter), "r" (addr), "r" (size)); offset / 8;})

#define MSTRCT_ADDR(counter) MSTRCT_CAT2(mstrct_addr, MSTRCT_O)(counter)

#define mstrct_addr0(counter) ({uint64_t a; __asm__ ("MSTRCT.0 %0, %c1" : "=r" (a) : "i" (counter)); a;})

__attribute__((const, always_inline)) static inline uint64_t
mstrct_addr1(int counter) {uint64_t addr;
  __asm__ ("MSTRCT.0 %0, %c1" : "=r" (addr) : "i" (counter)); return addr;
}

__attribute__((const)) static inline uint64_t
mstrct_addr2(uint64_t runtime_off) {uint64_t _addr;
  __asm__ ("leaq (%2,%1,8), %0\n\t" : "=r" (_addr) : "r" (runtime_off), "r" (mstrct_abc)); return _addr;
}

#define mstrct_addrx(counter) __asm__ __volatile__ ("MSTRCT.3 %c0" : : "i" (counter))

__attribute__((const)) static inline uint64_t
mstrct_get0(uint64_t runtime_off) {uint64_t _val;
  __asm__ ("movq (%2,%1,8), %0" : "=r" (_val) : "r" (runtime_off), "r" (mstrct_abc)); return _val;
}

__attribute__((const)) static inline uint64_t
mstrct_get1(uint64_t runtime_off) {uint64_t _val;
  __asm__ ("movq 8(%2,%1,8), %0" : "=r" (_val) : "r" (runtime_off), "r" (mstrct_abc)); return _val;
}

#define MSTRCT_SET(value, id, local_off) __asm__ __volatile__ ("MSTRCT.2 %1, %2, %3"   \
  : "=m" (*((char *)mstrct_abc + (id * 8) + (local_off))) : "r"  (value), "r"  (mstrct_abc), "r"  ((uint64_t)id))

// force return
#define MSTRCT_RET() __asm__ __volatile__ ("movl $0, %%eax\n\t" "leave\n\t" "ret" : : : "eax")

__attribute__((cold)) static void
mstrct_error(const char *ops, const int err_no, const int line) {
  printf("MSTRCT ERR: %s; originated at line: %d, file: %s; err status: %d\n", ops, line, mstrct_string, err_no);
  if (MSTRCT_L == 2) {exit(err_no);} else if (MSTRCT_L == 1) {MSTRCT_RET();}
}

#define MSTRCT_ASSERT(ops) MSTRCT_CAT2(MSTRCT__, ops)()

typedef void (*mstrct_func)(void);

static void MSTRCT__NULL(void) {(void)0;};
__attribute__((weak, warning("MSTRCT ERR: use after free (UAF) or NULL dereference!")))
void MSTRCT__USE_AFTER_FREE(void);
__attribute__((weak, warning("MSTRCT ERR: array bounds check FAIL!")))
void MSTRCT__BOUNDS_CHECK_FAIL(void);
__attribute__((weak, warning("MSTRCT ERR: INCOMPATIBLE arg(s)!")))
void MSTRCT__WRONG_TYPE_OF_ARG(void);
__attribute__((weak, warning("MSTRCT ERR: too MANY args!")))
void MSTRCT__TOO_MANY_ARGS(void);
__attribute__((weak, warning("MSTRCT ERR: namellocation FAILED!!")))
void MSTRCT__ALLOC_FAIL(void);
__attribute__((weak, warning("MSTRCT ERR: third argument must be EMPTY (it belongs to actual memory)!!")))
void MSTRCT__NON_EMPTY_THIRD_ARG(void);

static mstrct_func mstrct_err_tab[] = { // err vtable
  MSTRCT__NULL,
  MSTRCT__USE_AFTER_FREE,
  MSTRCT__BOUNDS_CHECK_FAIL,
  MSTRCT__WRONG_TYPE_OF_ARG,
  MSTRCT__TOO_MANY_ARGS,
  MSTRCT__ALLOC_FAIL,
  MSTRCT__NON_EMPTY_THIRD_ARG
};

__attribute__((hot)) static inline void 
mstrct_warn(int err_cond, int err_code, const char *err_msg, int line) {
  switch (__builtin_constant_p(err_cond)) {
      case 0: if (err_cond) {mstrct_error(err_msg, err_code, line);}; return;
      case 1: mstrct_err_tab[err_cond * (err_code - MSTRCT_NULL)](); return;
      default: __builtin_unreachable();
  }
}

static inline void
mstrct_leak(int status, void *ptr) {
  if (mstrct_get1((uint16_t)(uint64_t)ptr) != 0) {
    const char *file = (const char *)((uint64_t)mstrct_refstr + (((uint64_t)ptr)>> 32));
    printf("MSTRCT ERR: MEMORY_LEAK; for memory originated at line: %d, file: %s; exit status: %d\n",
    (int)(((uint32_t)(uint64_t)ptr) >> 16), file, status);
  }
}

static inline void
mstrct_cleanup(uint16_t *ptr) {
  MSTRCT_SET((uint64_t)0, (uint16_t)(*ptr), 8);
}

static inline void
mstrct_user_free(mstrct_proto *arg, int flag) {
  switch (flag) {
    case 0: {
      mstrct_proto name = *arg;
      if (mstrct_get1(name.id) != 0) {(free)((void *)mstrct_get0(name.id));}
      MSTRCT_SET((uint64_t)0, name.id, 8); break;
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

extern int (munmap)(void *addr, size_t len); // fwd declare munmap

static inline uint64_t
mstrct_munmap_1(mstrct_proto *arg, int line) {
  mstrct_proto name = *arg; int temp = 0;
  uint64_t size = mstrct_get1(name.id); 
  if (size != 0) {
    if ((munmap)((void *)mstrct_get0(name.id), size) == 0) {
      MSTRCT_SET((uint64_t)0, name.id, 8); return temp;
    } else {
       temp = 1; mstrct_error("de-allocation failed!!", MSTRCT_DE_ALLOC_FAIL, line); return temp;
    }
  } else return temp;
}

static inline uint64_t
mstrct_munmap_2(void **arg, uint64_t size, int line) {
  int temp = 0; void *addr = *(void **)(arg);
  if (addr != NULL) {
    if ((munmap)(addr, size) == 0) {addr = NULL; return temp;}
    else {temp = 1; mstrct_error("de-allocation failed!!", MSTRCT_DE_ALLOC_FAIL, line); return temp;}
  } else return temp;
}

// overload munmap
#define munmap(...)  MSTRCT_CAT2(MSTRCT_MUNMAP_, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#define MSTRCT_MUNMAP_1(arg) mstrct_munmap_1(((mstrct_proto *)&arg), __LINE__)
#define MSTRCT_MUNMAP_2(arg, size) mstrct_munmap_2((void **)&arg, size, __LINE__)
#define MSTRCT_MUNMAP_3(arg1, arg2, arg3) MSTRCT_ASSERT(TOO_MANY_ARGS)
#define MSTRCT_MUNMAP_4(arg1, arg2, arg3, arg4) MSTRCT_ASSERT(TOO_MANY_ARGS)
#define MSTRCT_MUNMAP_0() MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)

__attribute__((cold)) static inline char *
mstrct_bounds_error(uint16_t _d, uint64_t offset, int line) {
  uint64_t size = mstrct_get1(_d);
  if (size == 0) {mstrct_error("USE_AFTER_FREE", MSTRCT_USE_AFTER_FREE, line);}
  else {mstrct_error("BOUNDS_CHECK_FAIL", MSTRCT_BOUNDS_CHECK_FAIL, line);};
  if (MSTRCT_L == 0) {return (char *)(mstrct_get0(_d) - offset);} else {return 0;}
}

__attribute__((const)) static inline uint64_t
mstrct_oob_up(uint64_t unit_size, char *addr, uint16_t _d) {
  char *base = (char *)mstrct_get0(_d); uint64_t size = mstrct_get1(_d);
  int64_t limit = (base + size - addr)/ unit_size;
  if (limit < 0) {return 0;} else {return (uint64_t)limit;}
}

__attribute__((const)) static inline uint64_t
mstrct_oob_lo(uint64_t unit_size, char *addr, uint16_t _d) {
  char *base = (char *)mstrct_get0(_d);
  int64_t limit = (base - addr)/ unit_size;
  if (limit >= 0) {return UINT64_MAX;} else {return (uint64_t)limit;}
}

__attribute__((hot)) static inline char *
mstrct_check(uint16_t id, char *addr, uint64_t type_size, int line, uint64_t index) {
  if (mstrct_oob_up(type_size, addr, id) > (uint64_t)index || mstrct_oob_lo(type_size, addr, id) < (uint64_t)index) {
    return addr;
  } else {return mstrct_bounds_error(id, (index * type_size), line);}
}

// addr L-val
#define MSTRCT_GET0(name) (*((typeof(name.typ[0]) *) MSTRCT_ADDR(sizeof(name.ref[0]))))

#define MSTRCT_GETX(name) (char *)(*((typeof(name.typ[0]) *) MSTRCT_ADDR(sizeof(name.ref[0]))))
#define MSTRCT_TSIZ(name) sizeof(*((typeof(name.typ[0]))0))
#define MSTRCT_DSIZ(name) sizeof((*name.dim[0])[0])
#define MSTRCT_FLAT(name, multi_index) ((uint64_t)(&(*(typeof(name.dim[0]))0) multi_index))

#define MSTRCT_LET1(id) ((mstrct_meta *)(mstrct_addr2(id)))

// get
#define MSTRCT_GET(name, index) MSTRCT_CAT2(MSTRCT_GET_, MSTRCT_C)(name, index)

#define MSTRCT_GET_0(name, index) (*(MSTRCT_GET0(name) + MSTRCT_FLAT(name, index)))

#define MSTRCT_GET_1(name, index) (*((typeof(name.typ[0])) (mstrct_check(name.id, MSTRCT_GETX(name), \
  MSTRCT_TSIZ(name), __LINE__, MSTRCT_FLAT(name, index))) + MSTRCT_FLAT(name, index)))

// put
#define MSTRCT_PUT(store, name, range, counter) \
store typeof(*(name.typ[0])) MSTRCT_CAT2(mstrct_arr_, __LINE__) [(mstrct_ptr = (char *)2, (range) * MSTRCT_DSIZ(name))]; \
name.id = MSTRCT_DEF_META( \
  counter, (uint64_t)&MSTRCT_CAT2(mstrct_arr_, __LINE__), ((uint64_t)sizeof(*(name.typ[0])) * (range) * MSTRCT_DSIZ(name))); \
MSTRCT_CLEAN(__LINE__, name.id, store) \
if (mstrct_ptr == (char *)2) {   \
  mstrct_addrx(sizeof(name.ref[0])); mstrct_string = __FILE__; \
  MSTRCT_GET0(name) = (void *)&MSTRCT_CAT2(mstrct_arr_, __LINE__); mstrct_ptr = (char *)1; \
}

#define MSTRCT_CLEAN(line, off, store) MSTRCT_CAT3(MSTRCT_CLEAN_, MSTRCT_C, MSTRCT_AUTO(store))(line, off)
#define MSTRCT_CLEAN_00(line, off)
#define MSTRCT_CLEAN_10(line, off)
#define MSTRCT_CLEAN_11(line, off) __attribute__((cleanup(mstrct_cleanup))) uint16_t MSTRCT_CAT2(mstrct_s_, line) = off;
#define MSTRCT_CLEAN_01(line, off)

#define MSTRCT_LET(typ, name, empty, index, counter) MSTRCT_CAT2(MSTRCT_LET_, MSTRCT_ARG_COUNT(empty))(typ,name,index,counter)
#define MSTRCT_LET_0(typ, name, index, counter) MSTRCT_T(typ, index, counter) name
#define MSTRCT_LET_1(typ, name, index, counter) MSTRCT_ASSERT(NON_EMPTY_THIRD_ARG)

// bind
#define MSTRCT_LET3(memory, name, range, counter) do {   \
  mstrct_addrx(sizeof(name.ref[0])); mstrct_string = __FILE__; \
  mstrct_ptr = (char *)(memory); *((char **) MSTRCT_ADDR(sizeof(name.ref[0]))) = mstrct_ptr;   \
  mstrct_checksum((__builtin_strstr(#memory, "realloc") != NULL),   \
  (__builtin_strstr(#memory, "mremap") != NULL), (mstrct_proto *)&(name), __LINE__);  \
  name.id = MSTRCT_DEF_META(counter,(uint64_t)mstrct_ptr,((uint64_t)sizeof(*(name.typ[0]))* (range) * MSTRCT_DSIZ(name))); \
  MSTRCT_CAT2(mstrct_leak_, MSTRCT_C)((mstrct_proto *)&(name), __LINE__); /* leak check */  \
} while(0)

#define MSTRCT_LET2(_id, name) do {   \
  if (sizeof(typeof(_id)) != 2) {MSTRCT_ASSERT(WRONG_TYPE_OF_ARG);}   \
  name.id = _id; mstrct_addrx(sizeof(name.ref[0])); mstrct_string = __FILE__; \
  *((char **) MSTRCT_ADDR(sizeof(name.ref[0]))) = (char *)mstrct_get0(_id); \
} while(0)

static inline void 
mstrct_leak_1(mstrct_proto * name, int line) {char a;
  if ((int64_t)((char *)&a - mstrct_ptr) > 0 || (mstrct_ptr - (char *)environ) > 0) /* not on regular stack */ {
    mstrct_pack temp; temp._d = name->id; temp._s = (uint16_t)line;
    uint64_t diff = (uint64_t)mstrct_string - (uint64_t)mstrct_refstr;
    uint64_t packed = (temp.id) | (diff << 32);
    on_exit(mstrct_leak, (void *)(packed));
  } else {mstrct_error("wrong allocator!! use M(storage_class, foo, range) for on-stackk arrs!!", MSTRCT_ALLOC_FAIL, line);} 
  mstrct_ptr = (char *)1;
}

static inline void
mstrct_leak_0(__attribute__((unused)) mstrct_proto * name, __attribute__((unused)) int line) {mstrct_ptr = (char *)1;}

static inline void 
mstrct_checksum(int a, int b, mstrct_proto *name, int line) {
  if (a || b) {MSTRCT_SET((uint64_t)0, name->id, 8);}
  if (mstrct_ptr == NULL) {mstrct_error("allocation failed!!", MSTRCT_ALLOC_FAIL, line);}
}

#endif
