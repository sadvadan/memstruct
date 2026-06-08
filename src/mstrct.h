/**
 *  Copyright(C) 2026 @sadvadan (github.com/sadvadan)
 *
 *  memstruct v1.0.0
 *  "Zero Cost Abstraction"         -std=gnu99 & onws
 *
 * SUMMARY ******************************************
 *
 *  A lightweight & fast single header library with a
 *  m-macro API for memory safety in calling program.
 *
 *  Leverages compile-runtime methods & also hoisting
 *  to check OOB/UAF/NULL-deref/double-free and leaks
 *
 * mstrct_errno *************************************
 *
 *  USE_AFTER_FREE ................................ 1
 *  BOUNDS_CHECK_FAIL ............................. 2
 *  WRONG_TYPE_OF_ARG ............................. 3
 *  TOO_MANY_ARGS ................................. 4
 *  ALLOC_FAIL .................................... 5
 *  DE_ALLOC_FAIL ................................. 6
 *
 * runtime err_strings ******************************
 *
 *  UAF                  use after free OR null deref
 *  OOB                                 out of bounds
 *  ALLOC_FAIL                     allocation failure
 *  DEALLOC_FAIL                de-allocation failure
 *  OVF                      metadata memory overflow
 *
 * flags ********************************************
 *
 *  NAMSTRCT                   disable spatial safety
 *  NBMSTRCT             disable heap temporal safety
 *  NCMSTRCT            disable stack temporal safety
 *  MSTRCT_SOFT                print err_site line_no
 *  MSTRCT_HARD                segfault at error site
 *  MSTRCT_16                        MCU: 16 or 8 bit
 *  MSTRCT_32                             MCU: 32 bit
 *  MSTRCT_64                     make foo.i uint64_t
 *
 * language extension APIs **************************
 *
 *  MSTRCT_PRINT(...)                 MCU print macro
 *  MSTRCT_ALLOC(...)    MCU metadata allocator macro
 *  mstrct_realloc(...)    custom re-allocator func 1
 *  mstrct_mremap(...)     custom re-allocator func 2
 *
 * literals *****************************************
 *
 *  MSTRCT_SIZE         reference metadata block size
 *
 **/

#pragma GCC diagnostic warning "-Wstrict-aliasing=2"
#pragma GCC diagnostic warning "-Warray-bounds"

#ifndef MSTRCT_H
#define MSTRCT_H

#define MSTRCT_VER_MAJOR                  1
#define MSTRCT_VER_MINOR                  0
#define MSTRCT_VER_PATCH                  0

#define MSTRCT_DEF1(a)                    #a
#define MSTRCT_DEF2(a,b)                  a##b
#define MSTRCT_CAT1(a)                    MSTRCT_DEF1(a)
#define MSTRCT_CAT2(a,b)                  MSTRCT_DEF2(a,b)
#define MSTRCT_CAT3(a,b,c)                MSTRCT_CAT2(a, MSTRCT_CAT2(b,c))

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

#define MSTRCT_$14(foo,i,...)             MSTRCT_ASSERT(MSTRCT_TOO_MANY_ARGS)
#define MSTRCT_$13(foo,i,...)             MSTRCT_ASSERT(MSTRCT_TOO_MANY_ARGS)
#define MSTRCT_$12(foo,i,...)             MSTRCT_GET(foo, i, MSTRCT_IDX10(__VA_ARGS__))
#define MSTRCT_$11(foo,i,...)             MSTRCT_GET(foo, i, MSTRCT_IDX9(__VA_ARGS__))
#define MSTRCT_$10(foo,i,...)             MSTRCT_GET(foo, i, MSTRCT_IDX8(__VA_ARGS__))
#define MSTRCT_$9(foo,i,...)              MSTRCT_GET(foo, i, MSTRCT_IDX7(__VA_ARGS__))
#define MSTRCT_$8(foo,i,...)              MSTRCT_GET(foo, i, MSTRCT_IDX6(__VA_ARGS__))
#define MSTRCT_$7(foo,i,...)              MSTRCT_GET(foo, i, MSTRCT_IDX5(__VA_ARGS__))
#define MSTRCT_$6(foo,i,...)              MSTRCT_GET(foo, i, MSTRCT_IDX4(__VA_ARGS__))
#define MSTRCT_$5(foo,i,...)              MSTRCT_GET(foo, i, MSTRCT_IDX3(__VA_ARGS__))
#define MSTRCT_$4(foo,i,...)              MSTRCT_GET(foo, i, MSTRCT_IDX2(__VA_ARGS__))
#define MSTRCT_$3(foo,i,...)              MSTRCT_GET(foo, i, MSTRCT_IDX1(__VA_ARGS__))
#define MSTRCT_$2(foo,i,...)              MSTRCT_CAT2(MSTRCT_M2, MSTRCT_ARG_COUNT(i))(foo, i)
#define MSTRCT_$1(foo)                    MSTRCT_CAT2(MSTRCT_M1, MSTRCT_META(foo))(foo)
#define MSTRCT_$0()                       MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)

#define MSTRCT_$$15(typ,foo,empty,...)    MSTRCT_ASSERT(TOO_MANY_ARGS)
#define MSTRCT_$$14(typ,foo,empty,...)    MSTRCT_ASSERT(TOO_MANY_ARGS)
#define MSTRCT_$$13(typ,foo,empty,...)    MSTRCT_LET(typ,foo,empty,MSTRCT_IDX10(__VA_ARGS__))
#define MSTRCT_$$12(typ,foo,empty,...)    MSTRCT_LET(typ,foo,empty, MSTRCT_IDX9(__VA_ARGS__))
#define MSTRCT_$$11(typ,foo,empty,...)    MSTRCT_LET(typ,foo,empty, MSTRCT_IDX8(__VA_ARGS__))
#define MSTRCT_$$10(typ,foo,empty,...)    MSTRCT_LET(typ,foo,empty, MSTRCT_IDX7(__VA_ARGS__))
#define MSTRCT_$$9(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX6(__VA_ARGS__))
#define MSTRCT_$$8(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX5(__VA_ARGS__))
#define MSTRCT_$$7(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX4(__VA_ARGS__))
#define MSTRCT_$$6(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX3(__VA_ARGS__))
#define MSTRCT_$$5(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX2(__VA_ARGS__))
#define MSTRCT_$$4(typ,foo,empty,...)     MSTRCT_LET(typ,foo,empty, MSTRCT_IDX1(__VA_ARGS__))
#define MSTRCT_$$3(arg1,arg2,arg3)        MSTRCT_CAT3(MSTRCT_M3, MSTRCT_STORE(arg1), MSTRCT_ARG_COUNT(arg3))(arg1,arg2,arg3)
#define MSTRCT_$$2(de_store,foo)          MSTRCT_LET2(de_store, foo)
#define MSTRCT_$$1(foo)                   MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)
#define MSTRCT_$$0()                      MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)

#define MSTRCT_STORE(arg)                 MSTRCT_HAS_COMMA(MSTRCT_CAT2(MSTRCT_, arg)) /* 2=reallocation, 1=store, 0=no_store */
#define MSTRCT_AUTO(arg)                  MSTRCT_HAS_COMMA(MSTRCT_CAT2(_MSTRCT_, arg)) /* 1=auto, 0=not_auto */
#define MSTRCT_META(arg)                  MSTRCT_HAS_COMMA(MSTRCT_CAT2(MSTRCT__, arg)) /* 3=size, 2=base, 1=span, 0=no_key */

#define MSTRCT_M10(arg)                   MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)
#define MSTRCT_M11(span_foo)              MSTRCT_GET1(MSTRCT_LET1(span_foo))
#define MSTRCT_M12(base_foo)              MSTRCT_GET2(MSTRCT_LET1(base_foo))
#define MSTRCT_M13(size_foo)              MSTRCT_GET3(MSTRCT_LET1(size_foo))
#define MSTRCT_LET1(arg)                  MSTRCT_CAT2(__MSTRCT__, arg)

#define MSTRCT_M20(foo, i)                MSTRCT_LET0(foo)
#define MSTRCT_M21(foo, i)                MSTRCT_GET(foo, i, [0])

#define MSTRCT_M301(memory, foo, idx)     MSTRCT_LET3(memory, foo, idx)
#define MSTRCT_M311(store, foo, idx)      MSTRCT_PUT(store, foo, idx)
#define MSTRCT_M300(typ, foo, empty)      MSTRCT_LET(typ, foo, empty, [1])
#define MSTRCT_M310(store, foo, empty)    MSTRCT_PUT(store,foo, 1)
#define MSTRCT_M320(rememory, foo, empty) MSTRCT_ASSERT(WRONG_TYPE_OF_ARG)
#define MSTRCT_M321(rememory, foo, idx)   MSTRCT_LET4(rememory, foo, idx)

#define MSTRCT_ARG5(_1, _2, _3, _4, _5,...)  _5
#define MSTRCT_HAS_COMMA(...)             MSTRCT_ARG5(__VA_ARGS__, 3, 2, 1, 0)

// API
#define m(...)                            MSTRCT_CAT2(MSTRCT_$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define M(...)                            MSTRCT_CAT2(MSTRCT_$$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#define MSTRCT_SIZE                       (1ULL * 1024 * 1024 * 1024) // 1 GiB
#define MSTRCT_CHK1                       MSTRCT_ARG_COUNT(NAMSTRCT)
#define MSTRCT_CHK2                       MSTRCT_ARG_COUNT(NBMSTRCT)
#define MSTRCT_CHK3                       MSTRCT_ARG_COUNT(NCMSTRCT)
#define MSTRCT_LINE(name)                 MSTRCT_CAT3(MSTRCT_L, MSTRCT_ARG_COUNT(MSTRCT_SOFT), MSTRCT_ARG_COUNT(MSTRCT_HARD))(name)
#define MSTRCT_L01(name)                  __LINE__
#define MSTRCT_L11(name)                  ((mstrct_uint32)sizeof(name.lin[0]))
#define MSTRCT_L10(name)                  0

#define MSTRCT_ARG_COUNT(...)             MSTRCT_MACR16(10 __VA_OPT__(,) ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define MSTRCT_MACR16(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,NAME,...) NAME

#define MSTRCT_auto ~,1
#define MSTRCT_static ~,1
#define MSTRCT___thread ~,1
#define MSTRCT_realloc ~,~,2
#define MSTRCT_mstrct_realloc ~,~,2
#define MSTRCT_mstrct_mremap ~,~,2
#define MSTRCT_mremap ~,~,2
#define _MSTRCT_auto  ~,1
#define _MSTRCT___thread
#define _MSTRCT_static

#define __MSTRCT__span
#define __MSTRCT__base
#define __MSTRCT__size
#define MSTRCT__span ~,1
#define MSTRCT__base ~,~,2
#define MSTRCT__size ~,~,~,3

typedef unsigned long long mstrct_uint64; typedef signed long long mstrct_int64;

#if !defined(MSTRCT_16)
  typedef unsigned int mstrct_uint32; typedef signed int mstrct_int32;
  #if !defined(MSTRCT_16) && !defined(MSTRCT_32) 
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/mman.h>
    #define MSTRCT_PRINT(...) printf(__VA_ARGS__)
    #define MSTRCT_ALLOC(line) mstrct_alloc(line)
  #endif
#else
  typedef unsigned long mstrct_uint32; typedef signed long mstrct_int32;
#endif

extern char  **environ;
static __thread char *mstrct_ptr  = (char *)1;
static __thread char mstrct_errno = 0;

__attribute__((weak)) void* mstrct_start;
__attribute__((weak)) volatile mstrct_uint64 mstrct_offset;

typedef struct {union {void *ptr; struct {mstrct_uint32 _d; /*low*/ mstrct_uint32 _s; /*high*/};};} mstrct_pack;

#if !defined(MSTRCT_16) && !defined(MSTRCT_32) /* arena allocation for metadata (mstrct) */
static inline mstrct_uint64 mstrct_alloc(const mstrct_int32 line) {
  mstrct_uint64 increment = 2;

  if (__builtin_expect(mstrct_start == NULL, 0)) { // cold path taken only once at start
    void* space = mmap(NULL, MSTRCT_SIZE, 0x3, 0x4021, -1, 0); //PROT_READ | PROT_WRITE,MAP_ANONYMOUS | MAP_SHARED | MAP_NORESERVE
    if (space == ((void *)-1)) {return (mstrct_uint64)-1;}
    mstrct_start = space;
    mstrct_offset = 0;
  }

  increment = __atomic_fetch_add(&mstrct_offset, increment, __ATOMIC_RELAXED); // increment becomes the old val of mstrct_offset

  if (__builtin_expect(increment + 2 > (MSTRCT_SIZE / 8), 0)) { // BOUNDS CHECK
    MSTRCT_PRINT("M_%s/%s/%d\n", "OVF", __BASE_FILE__, line);
    return (mstrct_uint64)-1;
  }

  return increment;
}
#endif

__attribute__((const, __may_alias__)) static inline void*
mstrct_base(mstrct_uint32 offset) {
  return *(void **)((mstrct_uint64 *)mstrct_start + offset);
}

static inline char*
mstrct_base_addr(mstrct_uint32 offset) {
  return *(char **)((mstrct_uint64 *)mstrct_start + offset);
}
__attribute__((const)) static inline mstrct_uint64
mstrct_size(mstrct_uint32 offset) {
  return *((mstrct_uint64 *)mstrct_start + offset + 1);
}

// memstruct; see doc
#define MSTRCT_T(type, index, line) struct {  \
  MSTRCT_CAT2(MSTRCT_TYP_, MSTRCT_ARG_COUNT(MSTRCT_64))(type) i; \
  mstrct_uint32 id;   \
  /* typ[0] */ typeof(type) typ[0] __attribute__((packed)); \
  /* con[0] */ struct {char a[((MSTRCT_CON(type)) ? ((__builtin_constant_p(sizeof(char index))) ? 1 : 0) : 0)];} con[0];   \
  /* lin[0] */ struct {char a[line];} lin[0];   \
  /* dim[0] */ struct {char a[] index [1];} dim[0];   \
}

#define MSTRCT_CON(type) __builtin_types_compatible_p(typeof(type) const *, type *)
#define MSTRCT_TYP_0(type) typeof((MSTRCT_CON(type))? (mstrct_int64 const)0 : (mstrct_int64)0)
#define MSTRCT_TYP_1(type) typeof((MSTRCT_CON(type))? (mstrct_int32 const)0 : (mstrct_int32)0)

static inline void
mstrct_set(mstrct_uint32 *ptr) {
  __asm__ __volatile__ (" " : "+r"  (*ptr) : : );
  *((mstrct_uint64 *)mstrct_start + *ptr + 1) = 0;
}

__attribute__((noreturn)) static inline void
mstrct_sigsegv(void) {
  __asm__ __volatile__ (" " : "+r" (*mstrct_ptr) : :);
  *mstrct_ptr = 0; __builtin_unreachable();
}

__attribute__((cold)) static inline void
mstrct_error(const char *ops, const char err_no, const mstrct_int32 line) {
  MSTRCT_PRINT("M_%s/%s/%d\n", ops, __BASE_FILE__, line);
  if (MSTRCT_ARG_COUNT(MSTRCT_HARD) == 0) {mstrct_sigsegv();} else {mstrct_errno = err_no;}
}

#define MSTRCT_ASSERT(ops) MSTRCT_CAT2(MSTRCT_, ops)()
__attribute__((weak, warning("MSTRCT ERR: INCOMPATIBLE arg(s)!")))
void MSTRCT_WRONG_TYPE_OF_ARG(void);
__attribute__((weak, warning("MSTRCT ERR: too MANY args!")))
void MSTRCT_TOO_MANY_ARGS(void);
__attribute__((weak, warning("MSTRCT ERR: M(...) third argument must be EMPTY (it belongs to runtime index)!!")))
void MSTRCT_NON_EMPTY_THIRD_ARG(void);

static inline void
mstrct_leak(__attribute__((unused)) int status, void *ptr) {
  if (mstrct_size((mstrct_uint32)(mstrct_uint64)ptr) != 0) {
    MSTRCT_PRINT("M_%s/%s/%d\n", "LEAK", __BASE_FILE__, (int)(((mstrct_uint64)ptr) >> 32));
  }
}

static inline void
mstrct_cleanup(void *ptr) {mstrct_set((mstrct_uint32 *)ptr);}

__attribute__((cold)) static inline mstrct_int64
mstrct_bounds_error(mstrct_int32 _d, mstrct_int32 line) {
  if (mstrct_size(_d) == 0) {mstrct_error("UAF", 1, line);}
  else {mstrct_error("OOB", 2, line);};
  return 0;
}

__attribute__((const)) static inline mstrct_uint64
mstrct_limit(mstrct_uint64 unit_size, mstrct_uint32 _d) {
  return mstrct_size(_d) / unit_size;
}

__attribute__((hot)) static inline mstrct_int64
mstrct_check(mstrct_uint32 id, mstrct_uint64 type_size, mstrct_int32 line, mstrct_int64 index) {
  if (mstrct_limit(type_size, id) > (mstrct_uint64)index) {return index;}
  else {return mstrct_bounds_error(id, line);}
}

// utility
#define MSTRCT_TSIZ(name) sizeof(*((typeof(name.typ[0]))0))
#define MSTRCT_DSIZ(name) sizeof(*(name.dim[0].a))
#define MSTRCT_FLAT(name, index, idx) ((mstrct_int64)(&(*(typeof(name.dim[0].a) *)0) index idx [0]) + name.i)

// get
#define MSTRCT_GET1(name) ((mstrct_int64)mstrct_limit(MSTRCT_TSIZ(name), name.id))
#define MSTRCT_GET2(name) (mstrct_base_addr(name.id))
#define MSTRCT_GET3(name) (mstrct_size(name.id))

#define MSTRCT_GET(name, i, index) MSTRCT_CAT3(MSTRCT_GET_, MSTRCT_ARG_COUNT(i), MSTRCT_CHK1)(name, i, index)

#define MSTRCT_GET_10(name, i, index) (*((typeof(name.typ[0]))(mstrct_base(name.id)) + MSTRCT_FLAT(name, [i], index)))

#define MSTRCT_GET_11(name, i, index) (*((typeof(name.typ[0]))(mstrct_base(name.id)) +   \
  mstrct_check(name.id, MSTRCT_TSIZ(name), MSTRCT_LINE(name), MSTRCT_FLAT(name, [i], index))))

#define MSTRCT_GET_00(name, i, index) MSTRCT_GET_10(name, [0], index)

#define MSTRCT_GET_01(name, i, index) (*((sizeof(name.con[0]) && __builtin_constant_p(sizeof(char index))) ?   \
  ((typeof(name.typ[0]))(mstrct_base(name.id)) + MSTRCT_FLAT(name, [0] index)) : ((typeof(name.typ[0])) \
  (mstrct_base(name.id)) + mstrct_check(name.id, MSTRCT_TSIZ(name), MSTRCT_LINE(name), MSTRCT_FLAT(name, [0], index)))))

// static/on-stack array initializer list handler                                                         
#define MSTRCT_ERR__RANGE_MUST_NOT_BE_IN_PARENTHESES(a,b,...) /* deliberate fail for single input (a) */
#define MSTRCT_PAR(a,b,...) {(a, b) __VA_OPT__(,) ##__VA_ARGS__}
#define MSTRCT_FULL(...) __VA_ARGS__
#define MSTRCT_PUT(store, name, range) \
MSTRCT_CAT2(MSTRCT_PUT_, MSTRCT_ARG_COUNT(MSTRCT_ERR__RANGE_MUST_NOT_BE_IN_PARENTHESES range))(store, name, range)

// put
#define MSTRCT_PUT_0(store, name, range) store char MSTRCT_CLEAN(__LINE__, store)  \
MSTRCT_CAT2(mstrct_arr_, __LINE__) [(mstrct_uint64)sizeof(*(name.typ[0])) * MSTRCT_DSIZ(name)] \
__attribute__((aligned(__alignof__(*(name.typ[0]))), unused)) = MSTRCT_PAR(mstrct_ptr = (char *)2, MSTRCT_FULL(range)) ;   \
if (mstrct_ptr == (char *)2) {   \
  name.id = MSTRCT_ALLOC(__LINE__);   \
  *(void **)((mstrct_uint64 *)mstrct_start + name.id) = (void *)MSTRCT_CAT2(mstrct_arr_, __LINE__); \
  *((mstrct_uint64 *)mstrct_start + name.id + 1) = ((mstrct_uint64)sizeof(*(name.typ[0])) * MSTRCT_DSIZ(name));   \
  mstrct_ptr = (char *)1; *(int *)&(name.i) = 0; \
  if (MSTRCT_CHK3 && MSTRCT_AUTO(store)) {*(mstrct_uint32 *)MSTRCT_CAT2(mstrct_arr_, __LINE__) = name.id;}  \
}

#define MSTRCT_PUT_1(store, name, range) store char MSTRCT_CLEAN(__LINE__, store)  \
MSTRCT_CAT2(mstrct_arr_,__LINE__) [(range) * (mstrct_uint64)sizeof(*(name.typ[0])) * MSTRCT_DSIZ(name)] \
__attribute__((aligned(__alignof__(*(name.typ[0]))), unused)) = MSTRCT_PAR(mstrct_ptr = (char *)2, 0) ;   \
if (mstrct_ptr == (char *)2) {   \
  name.id = MSTRCT_ALLOC(__LINE__);   \
  *(void **)((mstrct_uint64 *)mstrct_start + name.id) = (void *)MSTRCT_CAT2(mstrct_arr_, __LINE__); \
  *((mstrct_uint64 *)mstrct_start + name.id + 1) = ((mstrct_uint64)sizeof(*(name.typ[0])) * (range) * MSTRCT_DSIZ(name));   \
  mstrct_ptr = (char *)1; *(int *)&(name.i) = 0;  \
  if (MSTRCT_CHK3 && MSTRCT_AUTO(store)) {*(mstrct_uint32 *)MSTRCT_CAT2(mstrct_clean_, __LINE__) = name.id;}  \
}                                                        

#define MSTRCT_CLEAN(line, store) MSTRCT_CAT3(MSTRCT_CLEAN_, MSTRCT_CHK3, MSTRCT_AUTO(store))(line)
#define MSTRCT_CLEAN_00(line)
#define MSTRCT_CLEAN_10(line)
#define MSTRCT_CLEAN_11(line) MSTRCT_CAT2(mstrct_clean_, line)[4] __attribute__((cleanup(mstrct_cleanup), aligned(4), unused)),
#define MSTRCT_CLEAN_01(line)

#define MSTRCT_LET(typ, name, empty, index)   \
  MSTRCT_CAT2(MSTRCT_LET_, MSTRCT_ARG_COUNT(empty))(typ, name, index)
#define MSTRCT_LET_0(typ, name, index) MSTRCT_T(typ, index, __LINE__) name
#define MSTRCT_LET_1(typ, name, index) MSTRCT_ASSERT(NON_EMPTY_THIRD_ARG)

// bind
#define MSTRCT_LET4(rememory, name, range) do {   \
  mstrct_ptr = (char *)(rememory);  \
  *(void **)((mstrct_uint64 *)mstrct_start + name.id) = (void *)mstrct_ptr; \
  *((mstrct_uint64 *)mstrct_start + name.id + 1) = ((mstrct_uint64)sizeof(*(name.typ[0])) * (range) * MSTRCT_DSIZ(name));   \
  mstrct_leak_0(0, __LINE__); __asm__ __volatile__ (" " : : : "memory"); *(int *)&(name.i) = 0; \
} while(0)

#define MSTRCT_LET3(memory, name, range) do {   \
  mstrct_ptr = (char *)(memory); name.id = MSTRCT_ALLOC(__LINE__); *(int *)&(name.i) = 0;  \
  *(void **)((mstrct_uint64 *)mstrct_start + name.id) = (void *)mstrct_ptr; \
  *((mstrct_uint64 *)mstrct_start + name.id + 1) = ((mstrct_uint64)sizeof(*(name.typ[0])) * (range) * MSTRCT_DSIZ(name));   \
  MSTRCT_CAT2(mstrct_leak_, MSTRCT_CHK2)(name.id, __LINE__); /* leak check */  \
} while(0)

#define MSTRCT_LET2(de_store, name) do {   \
  __builtin_choose_expr((sizeof(de_store) == 1), (mstrct_dealloc_0(de_store, (name.id))),  \
    (mstrct_dealloc_1(de_store, (name.id), __LINE__)) \
  );  \
  mstrct_set(&(name.id));  \
} while(0)

// prototypes for free() & munmap()                                                                                   
typedef void (*mstrct_free_proto)(void *);
typedef mstrct_int32 (*mstrct_munmap_proto)(void *, mstrct_uint64);

static inline void
mstrct_dealloc_0(void *fun, mstrct_uint32 id) {
  if (mstrct_size(id) != 0) {
    ((mstrct_free_proto)fun)(mstrct_base(id));
  }
}

static inline void
mstrct_dealloc_1(void *fun, mstrct_uint32 id, mstrct_uint32 line) {
  if (mstrct_size(id) != 0) {
    if (((mstrct_munmap_proto)fun)(mstrct_base(id), mstrct_size(id)) == -1) {mstrct_error("DEALLOC_FAIL", 6, line);}
  }
}

static inline void 
mstrct_leak_1(mstrct_uint32 id, mstrct_int32 line) {char a;
  if (mstrct_ptr == NULL) {mstrct_error("ALLOC_FAIL", 5, line);}
  if ((mstrct_int64)((char *)&a - mstrct_ptr) > 0 || (mstrct_ptr - (char *)environ) > 0) /* not on regular stack */ {
    mstrct_pack temp; temp._d = id; temp._s = (mstrct_uint32)line;
    on_exit(mstrct_leak, temp.ptr);
  } else {mstrct_error("ALLOC_FAIL", 5, line);}
  mstrct_ptr = (char *)1;
}

static inline void
mstrct_leak_0(__attribute__((unused)) mstrct_uint32 id, __attribute__((unused)) mstrct_int32 line) {
  if (mstrct_ptr == NULL || mstrct_ptr == ((void *) -1)) {mstrct_error("ALLOC_FAIL", 5, line);}
  mstrct_ptr = (char *)1;
}

#endif
