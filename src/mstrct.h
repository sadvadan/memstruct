/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *  Copyright(C) 2026 @sadvadan (github.com/sadvadan)
 *
 *  memstruct v1.0.0
 *  "Zero Cost Abstraction"         -std=gnu99 & onws
 *
 * SUMMARY ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  Single header library for 8-64 bit CPUs, covering
 *  memory & thread safety through m()/M() macro API.
 *  Uses compile & runtime optimizations and hoisting
 *  to check OOB/UAF/NULL-deref/double-free and leaks
 *
 * errcodes & errnos ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  "BAD" (ILLEGAL_ACCESS) ........................ 1
 *  "OOB" (BOUNDS_CHECK_FAIL) ..................... 2
 *  "ALLOC_FAIL" .................................. 3
 *  "DE_ALLOC_FAIL" ............................... 4
 *  "META_OVF" (METADATA_OVERFLOW)................. 5
 *  "TID_OVF" (THREAD_ID_OVERFLOW)................. 6
 *
 * flags ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  MSTRCTM              enable locks/atomics support
 *  NMSTRCTH             disable heap temporal safety
 *  NMSTRCT                    disable spatial safety
 *  MSTRCT_SOFT                print err_site line_no
 *  MSTRCT_HARD                segfault at error site
 *  MSTRCT_MCU                        enable MCU mode
 *
 * MCU APIs ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  MSTRCT_PRINT                          print macro
 *  MSTRCT_PRINT_FMT              print format string
 *  MSTRCT_ALLOC                block allocator macro
 *
 * literals ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  MSTRCT_BLOCK  reference metadata size (KiB units)
 *  MSTRCT_TNO (>=0)   no of multithreads (over main)
 *
 * limitations ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *
 *  currently not supported: harvard arch, big endian
 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/


#ifndef MSTRCT_H
#define MSTRCT_H

#define MSTRCT_VER_MAJOR                  1
#define MSTRCT_VER_MINOR                  00
#define MSTRCT_VER_PATCH                  000

#define MSTRCT_DUMMY(...)
#define MSTRCT_PARSE_0                    1
#define MSTRCT_PARSE_1                    0
#define _MSTRCT_auto                      1,1
#define _MSTRCT_do                        1,1,2
#define _MSTRCT__                         1,1,1,3
#define MSTRCT_EXPAND(...)                __VA_ARGS__

#define MSTRCT_DEF2(a,b)                  a##b
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

#define m(...)                            MSTRCT_CAT2(MSTRCT_$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define M(...)                            MSTRCT_CAT2(MSTRCT_$$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define MSTRCT_INDEX(...)                 MSTRCT_CAT2(MSTRCT_IDX, MSTRCT_ARG_COUNT(__VA_ARGS__)) (__VA_ARGS__)
#define MSTRCT_PAREN(i)                   MSTRCT_CAT2(MSTRCT_PARSE_, MSTRCT_ARG_COUNT(MSTRCT_DUMMY i))
#define MSTRCT_SUB(i)                     MSTRCT_CAT2(MSTRCT_SUB, MSTRCT_PAREN(i))(i)
#define MSTRCT_SUB0(i)                    [i]
#define MSTRCT_SUB1(i)                    MSTRCT_INDEX(MSTRCT_EXPAND i)

#define MSTRCT_QUAL(arg)                  MSTRCT_CAT2(MSTRCT_QUAL, MSTRCT_ARG_COUNT(MSTRCT_DUMMY arg))(arg)
#define MSTRCT_QUAL1(arg)                 MSTRCT_DO(_MSTRCT_##arg)
#define MSTRCT_QUAL0(arg)                 0

#define MSTRCT_ARG_COUNT(...)             MSTRCT(10 __VA_OPT__(,) ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define MSTRCT_DO(...)                    MSTRCT_ARG6(__VA_ARGS__, 4, 3, 2, 1, 0)
#define MSTRCT_ARG6(a,b,c,d,e,f,...)      f
#define MSTRCT(a,b,c,d,e,f,g,h,i,j,k,...) k

#define MSTRCT_PRAG0                      _Pragma("GCC diagnostic pop")
#define MSTRCT_PRAG1                      _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
#define MSTRCT_PRAG2                      _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wattributes\"")   \
                                          _Pragma("GCC diagnostic ignored \"-Wunused-local-typedefs\"")  \
                                          _Pragma("GCC diagnostic ignored \"-Woverride-init\"")

#define MSTRCT_CHK1                       MSTRCT_ARG_COUNT(NMSTRCT)
#define MSTRCT_CHK2                       MSTRCT_ARG_COUNT(NMSTRCTH)
#define MSTRCT_MULT                       MSTRCT_ARG_COUNT(MSTRCTM)

// util
#define MSTRCT_TID                        ((mstrct_uhalf)__builtin_choose_expr(sizeof(mstrct_tid), mstrct_tid, 0))

#define MSTRCT_LIN(name)                  MSTRCT_CAT3(MSTRCT_L,MSTRCT_ARG_COUNT(MSTRCT_SOFT),MSTRCT_ARG_COUNT(MSTRCT_HARD))(name)
#define MSTRCT_L01(name)                  __LINE__
#define MSTRCT_L11(name)                  ((mstrct_unit)sizeof(name.lin[0]))
#define MSTRCT_L10(name)                  0

#define MSTRCT_IS_ID(id)                  MSTRCT_CAT2(MSTRCT_ISID, MSTRCT_ARG_COUNT(MSTRCT_HARD))(id)
#define MSTRCT_ISID0(id)                  0
#define MSTRCT_ISID1(id)                  (id)

#define MSTRCT_STEP                       (MSTRCT_CHK2 ? (1 + sizeof(mstrct_utwice) / sizeof(mstrct_usize)) : 2)
#define MSTRCT_SHIFT                      (8*(sizeof(mstrct_utwice) - sizeof(mstrct_unit) - sizeof(mstrct_uhalf)  \
                                          * MSTRCT_ARG_COUNT(MSTRCT_MCU)))

#define MSTRCT_UNIT                       mstrct_unit
#define MSTRCT_SIZE(word)                 ((((mstrct_utwice)(word)) << MSTRCT_SHIFT) >> MSTRCT_SHIFT)
#define MSTRCT_TSIZ(name)                 ((mstrct_unit)sizeof(*(name.typ[0])))
#define MSTRCT_ELEM(type, i)              (*(typeof(type) *)0) MSTRCT_SUB(i) [0]
#define MSTRCT_FLAT(typ, i)               ((mstrct_size)((mstrct_usize)&(MSTRCT_ELEM(typ,i)) / sizeof(MSTRCT_ELEM(typ,i))))

#if defined(MSTRCT_MCU)
  #define MSTRCT_BLOCK                    1 // 1 KiB
  typedef unsigned int mstrct_usize;      typedef signed int mstrct_size; // 8,16,32b :: 2,2,4 B
#else
  #include <stdio.h>
  #include <stdlib.h>
  #define MSTRCT_PRINT                    printf
  #define MSTRCT_ALLOC                    malloc
  #define MSTRCT_BLOCK                    (10 * 1024) // 10 MiB
  typedef unsigned long mstrct_usize;     typedef signed long mstrct_size; // 64b :: 8B
#endif

#ifndef __clang__
#pragma GCC diagnostic ignored            "-Wstringop-overflow" // _/\_
#else
#pragma clang diagnostic ignored          "-Wtypedef-redefinition"
#endif

#ifndef MSTRCT_PRINT_FMT
  #define MSTRCT_PRINT_FMT                "M_%s/%s/%d\n"
#endif

#ifndef MSTRCT_TNO
  #define MSTRCT_TNO                      0
#endif

// containers
typedef unsigned int mstrct_unit; // 8,16,32,64b :: 2,2,4,4 B
typedef typeof(__builtin_choose_expr(sizeof(mstrct_unit) > 2, (unsigned short)0, (unsigned char)0)) mstrct_uhalf;
typedef typeof(__builtin_choose_expr(sizeof(mstrct_unit) > 2, (unsigned long long)0, (unsigned long)0)) mstrct_utwice;

__attribute__((common)) char mstrcterrno[MSTRCT_TNO + 1];
__attribute__((common)) mstrct_unit mstrctbox[MSTRCT_TNO + 1];
__attribute__((common)) mstrct_unit mstrctx[MSTRCT_TNO + 1], mstrcty[MSTRCT_TNO + 1];
__attribute__((common)) mstrct_usize *mstrctfixed[MSTRCT_TNO + 1]; static mstrct_usize **restrict mstrct_fixed = mstrctfixed;

typedef union {mstrct_uhalf id; mstrct_uhalf tid;} mstrct_pass;
typedef struct __attribute__((packed)) {mstrct_uhalf tid; mstrct_unit id;} mstrct_pack;

struct mstrct_arc {}; typedef struct mstrct_arc mstrct_arc; static struct {} mstrct_tid; 
#define MSTRCT_ARC (!__builtin_types_compatible_p(typeof((mstrct_arc *)0), typeof((struct mstrct_arc *)0)))

_Static_assert(sizeof(void(*)(void)) == sizeof(void*), "M_ERR: code & data ptrs must be same size!"); // no harvard

// memstruct

#define MSTRCT_T0(type, index, line, key) union {typeof(__builtin_choose_expr(key, (mstrct_utwice)0, (mstrct_unit)0)) _ID; \
struct {mstrct_unit _id;   \
  typeof(__builtin_choose_expr(key, (MSTRCT_UNIT)0, (struct {}){})) i;  \
  /* typ[0] */ typeof(type) * const typ[0] __attribute__((packed)); \
  /* lin[0] */ struct {char a[line];} lin[0];   \
  /* dim[0] */ struct {char b; typeof(type) a[] index [1];} dim[0];   \
};}

#define MSTRCT_T1(type, index, line, key) union {typeof(__builtin_choose_expr(key, (mstrct_utwice)0, (mstrct_unit)0)) _ID; \
struct {mstrct_unit _id;   \
  typeof(__builtin_choose_expr(key, (MSTRCT_UNIT)0, (struct {}){})) i;   \
  /* typ[0] */ typeof(type) * typ[0] __attribute__((packed)); \
  /* lin[0] */ struct {char a[line];} lin[0];   \
  /* dim[0] */ struct {typeof(type) a index [1];} dim[1];  \
};}

__attribute__((alloc_size(1), noinline, unused, const)) static char*
mstrct_base(mstrct_unit siz, mstrct_unit offset, char var, mstrct_uhalf tid) {
  (void)siz; asm volatile (" " : "+m" (var)); return (char *)(*((mstrct_fixed[tid]) + offset));
}

static inline char*
mstrct_addr(mstrct_unit offset, mstrct_uhalf tid) {return (char *)(*((mstrct_fixed[tid]) + offset));}

static inline mstrct_usize
mstrct_byte(mstrct_unit offset, mstrct_uhalf tid) {
  if (offset >= mstrcty[tid]) {return *((mstrct_fixed[tid]) + offset + 1);}
  else {return MSTRCT_SIZE(*(mstrct_utwice *)((mstrct_fixed[tid]) + offset + 1));}
}

__attribute__((noinline, unused, const)) static mstrct_size
mstrct_span(mstrct_unit tsiz, mstrct_unit offset, char var, mstrct_uhalf tid) {
  asm volatile (" " : "+m" (var)); return mstrct_byte(offset, tid) / tsiz;
}

__attribute__((always_inline)) static inline char
mstrct_reset(mstrct_unit offset, mstrct_uhalf tid) {return (char)(*((mstrct_fixed[tid]) + offset + 1));}

__attribute__((cold)) static inline void
mstrct_error(const char *ops, const char err_no, const mstrct_unit line, mstrct_uhalf tid) {
  MSTRCT_PRINT(MSTRCT_PRINT_FMT, ops, __BASE_FILE__, line);
  if (MSTRCT_ARG_COUNT(MSTRCT_HARD) == 0) {__builtin_trap();} else {mstrcterrno[tid] = err_no;}
}

__attribute__((cold)) static inline mstrct_size
mstrct_bounds_error(mstrct_unit _d, mstrct_unit line, mstrct_uhalf tid) {
  if (mstrct_byte(_d, tid) == 0) {mstrct_error("BAD", 1, line, tid);} else {mstrct_error("OOB", 2, line, tid);}; return 0;
}

__attribute__((hot)) static inline mstrct_size
mstrct_check(mstrct_unit id, mstrct_unit type_size, mstrct_unit line, mstrct_size index, mstrct_uhalf tid) {
  if (__builtin_expect(((mstrct_usize)mstrct_span(type_size, id, mstrct_reset(id, tid), tid) > (mstrct_usize)index), 1))
  {return index;} else {return mstrct_bounds_error(MSTRCT_IS_ID(id), line, tid);}
}

// prototypes for free() & munmap()                                                                                   
typedef void (*mstrct_free_proto)(void *); typedef char (*mstrct_munmap_proto)(void *, mstrct_usize);

static inline void
mstrct_dealloc_0(void *fun, mstrct_unit id, mstrct_uhalf tid) {
  if (mstrct_byte(id, tid) != 0) {((mstrct_free_proto)fun)(mstrct_addr(id, tid));}
}

static inline void
mstrct_dealloc_1(void *fun, mstrct_unit id, mstrct_unit line, mstrct_uhalf tid) {
  if (mstrct_byte(id, tid) != 0) {
    if (((mstrct_munmap_proto)fun)(mstrct_addr(id, tid), mstrct_byte(id, tid)) == -1) {
      mstrct_error("DEALLOC_FAIL", 4, line, tid);
    }
  }
}

static inline mstrct_unit 
mstrct_alloc(mstrct_uhalf tid, char heap) {
  if ((MSTRCT_ARG_COUNT(MSTRCT_SOFT) == 0) && (mstrctx[tid] & 1023) == 0) {MSTRCT_PRINT(MSTRCT_PRINT_FMT,"","ID",mstrctx[tid]);}
  mstrct_unit ret = 0; if (__builtin_expect(mstrctx[tid] >= mstrcty[tid], 0)) {mstrct_error("META_OVF", 5, 0, tid);}
  if (heap) {ret = mstrctx[tid]; mstrctx[tid] += MSTRCT_STEP;}
  else {mstrcty[tid] -= 2; ret = mstrcty[tid];}
  return ret;
}

static inline mstrct_unit
mstrct_put(void *arr, mstrct_unit exist, mstrct_usize size, mstrct_uhalf tid, mstrct_unit hash) {
  mstrct_unit id; if (!exist) {id = mstrct_alloc(tid, hash);} else id = exist;
  *(void **)((mstrct_fixed[tid]) + id) = arr;
  if (hash) {
    *(typeof(__builtin_choose_expr(MSTRCT_STEP == 2, (mstrct_usize *)0, (mstrct_utwice *)0)))(mstrct_fixed[tid] + id + 1) =
    (mstrct_utwice)size | ((mstrct_utwice)(mstrct_uhalf)hash << (8 * sizeof(mstrct_utwice) - MSTRCT_SHIFT));
  } else *(mstrct_fixed[tid] + id + 1) = size; return id;
}

static inline void
mstrct_set(void *ptr) {
  if (*(mstrct_uhalf *)ptr) {
    mstrct_pack p = *(mstrct_pack *)ptr; mstrct_uhalf tid = p.tid - 1;
    for (mstrct_unit j = mstrcty[tid]; j < p.id; j += 2) {
      *(mstrctfixed[tid] + j + 1) = 0; *(mstrctfixed[tid] + j) = (mstrct_usize)(mstrct_fixed[tid]);
      asm volatile (" " : "+m" (*(mstrct_fixed[tid] + j + 1)));
    }
    mstrcty[tid] = p.id;
  }
}

__attribute__((constructor(101))) static inline void
mstrct_init(void) {
  if (mstrctfixed[0] == 0) {void *space;
    for (mstrct_uhalf i = 0; i <= MSTRCT_TNO; i++) {
      if (mstrctbox[i] == 0) mstrctbox[i] = MSTRCT_BLOCK;
      space = MSTRCT_ALLOC(1024 * mstrctbox[i]);
      if (space == NULL) {mstrct_error("ALLOC_FAIL", 3, 0, MSTRCT_TID); __builtin_trap();}
      mstrct_fixed[i] = space; mstrctx[i] = 2; *(mstrctfixed[i] + 1) = (1024 * mstrctbox[i]);
      mstrcty[i] = (*(mstrctfixed[i]+1) / sizeof(mstrct_usize));
    }
  } if (mstrct_fixed[0] == NULL) {for (mstrct_uhalf i = 0; i <= MSTRCT_TNO; i++) {mstrct_fixed[i] = mstrctfixed[i];}}
}

__attribute__((destructor)) static inline void
mstrct_leak(void) {
  if (MSTRCT_CHK2 && mstrctfixed[0] != (mstrct_usize *)7) {
    for (mstrct_uhalf i = 0; i <= MSTRCT_TNO; i++) {
      for (mstrct_unit j = 2; j <= mstrctx[i]; j += MSTRCT_STEP) {
        if (*(mstrct_utwice *)(mstrctfixed[i] + j + 1) != 0) {
          mstrct_unit line = (*(mstrct_utwice *)(mstrctfixed[i] + j + 1)) >> (8 * sizeof(mstrct_utwice) - MSTRCT_SHIFT);
          if (line != 0) {MSTRCT_PRINT(MSTRCT_PRINT_FMT, "LEAK", __BASE_FILE__, line);}
        }
      }
    }
  } mstrctfixed[0] = (mstrct_usize *)7;
}

// evil macros

#define MSTRCT_$$8(name, mstrct_mmap, mstrct_addr, mstrct_size, mstrct_prot, mstrct_flag, mstrct_fd, mstrct_ofset)  do {   \
char *ptr = (char*)mstrct_mmap(mstrct_addr, mstrct_size, mstrct_prot, mstrct_flag, mstrct_fd, mstrct_ofset);   \
__builtin_memset(&name, 0, sizeof(name)); MSTRCT_HELP(ptr, name._id, mstrct_size, 0);  \
} while(0)

#define MSTRCT_$$7(name, mstrct_mremap, mstrct_old_addr, mstrct_old_size, mstrct_new_size, mstrct_flag, mstrct_new_addr) do { \
char *ptr = (char *)mstrct_mremap(mstrct_old_addr, mstrct_old_size, mstrct_new_size, mstrct_flag, mstrct_new_addr);  \
MSTRCT_HELP(ptr, name._id, mstrct_new_size, name._id); \
} while(0)
   
#define MSTRCT_$$6(name, mstrct_mremap, mstrct_old_addr, mstrct_old_size, mstrct_new_size, mstrct_flag) do {   \
char *ptr = (char *)mstrct_mremap(mstrct_old_addr, mstrct_old_size, mstrct_new_size, mstrct_flag);  \
MSTRCT_HELP(ptr, name._id, mstrct_new_size, name._id); \
} while(0)

#define MSTRCT_$$5(name, mstrct_custom, mstrct_addr, mstrct_size, mstrct_arg) do { /* arena allocators fit here */   \
char *ptr = (char *)mstrct_custom(mstrct_addr, mstrct_size, mstrct_arg); __builtin_memset(&name, 0, sizeof(name));   \
MSTRCT_HELP(ptr, name._id, mstrct_size, 0);  \
} while(0)


#define MSTRCT_$$4(name, mstrct_realloc, mstrct_addr, mstrct_size) do {   \
char *ptr = (char *)mstrct_realloc(mstrct_addr, mstrct_size); MSTRCT_HELP(ptr, name._id, mstrct_size, name._id);  \
} while(0)

#define MSTRCT_$$3(name, mstrct_alloc, mstrct_size) do {   \
char *ptr = (char *)mstrct_alloc(mstrct_size); __builtin_memset(&name, 0, sizeof(name)); MSTRCT_HELP(ptr,name._id,mstrct_size,0); \
} while(0)

#define MSTRCT_$$2(name, mstrct_free) do {__builtin_choose_expr((sizeof(mstrct_free) == 1),   \
(mstrct_dealloc_0(mstrct_free, (name._id), MSTRCT_TID)), (mstrct_dealloc_1(mstrct_free, (name._id), __LINE__, MSTRCT_TID))); \
*(mstrct_utwice *)(mstrct_fixed[MSTRCT_TID] + name._id + 1) = 0;  \
*(mstrct_fixed[MSTRCT_TID] + name._id) = (mstrct_usize)(mstrct_fixed[MSTRCT_TID]); \
asm volatile (" " : "+m" (*(mstrct_fixed[MSTRCT_TID] + name._id + 1)));   \
} while(0)

#define MSTRCT_$$1(ptr) mstrct_uhalf mstrct_tid = (mstrct_uhalf)((mstrct_usize)ptr >> 8 * sizeof(mstrct_uhalf));

#define MSTRCT_$$0() mstrcterrno[MSTRCT_TID]

#define MSTRCT_$4(name, n, typ, store)    MSTRCT_CAT2(MSTRCT_$4, MSTRCT_QUAL(store))(name, n, typ, store)

#define MSTRCT_$40(name, n, typ, store) MSTRCT_$400(name, n, typ, store, __COUNTER__)
#define MSTRCT_$400(name, n, typ, store, cnt)   \
store MSTRCT_T1(typ, MSTRCT_SUB(n), __LINE__, MSTRCT_PAREN(n)) name  \
static inline void __attribute__((constructor(102))) MSTRCT_CAT2(mstrct$, cnt)(void) { \
  __builtin_memset(&name, 0, sizeof(name)); name._id = mstrct_put(&(name.dim[0].a), 0, sizeof(name.dim[0].a), 0, 0); \
}

#define MSTRCT_$41(name, n, typ, auto) MSTRCT_$411(name, n, typ, auto, __COUNTER__)
#define MSTRCT_$411(name, n, typ, auto, cnt) MSTRCT_T1(typ, MSTRCT_SUB(n), __LINE__, MSTRCT_PAREN(n)) name; MSTRCT_CLEAN(cnt);   \
__builtin_memset(&name, 0, sizeof(name)); name._id = mstrct_put(&(name.dim[0].a), 0, sizeof(name.dim[0].a), MSTRCT_TID, 0)

#define MSTRCT_$42(name, n, typ, do) MSTRCT_CAT2(MSTRCT_$42, MSTRCT_MULT)(name, n, typ, do)
#define MSTRCT_$420(name, n, typ, do)  MSTRCT_DATA(mstrctbox[(mstrct_uhalf)(mstrct_usize)name], 0, MSTRCT_FLAT(typ[1], n), \
(&((typ){0})[0]), __LINE__, (mstrct_uhalf)(mstrct_usize)name)

#define MSTRCT_$43(name, n, typ, _) ({MSTRCT_DATA(mstrctbox[(mstrct_uhalf)(mstrct_usize)name], 0, MSTRCT_FLAT(typ[1], n),  \
(&((typ){0})[0]), __LINE__, (mstrct_uhalf)(mstrct_usize)name);})

#define MSTRCT_$3(name, n, typ)           MSTRCT_CAT2(MSTRCT_$3, MSTRCT_QUAL(n))(name, n, typ)
#define MSTRCT_$30(name, n, typ)          MSTRCT_T0(typ, MSTRCT_SUB(n), __LINE__, MSTRCT_PAREN(n)) name
#define MSTRCT_$31(name, auto, tid)       ({mstrctbox[MSTRCT_TID] = name._id; (void *)(((mstrct_usize)(tid+1) \
                                          << 8*sizeof(mstrct_uhalf)) | (mstrct_usize)MSTRCT_TID);})
#define MSTRCT_$33(name, _, typ)          mstrct_span(sizeof(typ), (mstrct_unit)(mstrct_usize)name, 1, MSTRCT_TID)

#define MSTRCT_$2(foo, n)                 MSTRCT_CAT2(MSTRCT_$2, MSTRCT_QUAL(n))(foo, n)
#define MSTRCT_$20(name, n)               MSTRCT_DATA(name._id, sizeof(name.i), (__builtin_choose_expr(sizeof(name.i), name.i, 0) \
                                          + MSTRCT_FLAT(name.dim[0].a, n)), name.typ[0], MSTRCT_LIN(name), MSTRCT_TID)
#define MSTRCT_$21(name, auto)            (*({asm volatile ("":"+m"(*(mstrct_fixed[MSTRCT_TID] + name._id +1))); &(name._ID);}))
#define MSTRCT_$23(name, _)               mstrct_span(MSTRCT_TSIZ(name), name._id, mstrct_reset(name._id,MSTRCT_TID), MSTRCT_TID)

#define MSTRCT_$1(name)                   (*(typeof(name.typ[0]))mstrct_base(MSTRCT_TSIZ(name), name._id,   \
                                          mstrct_reset(name._id, MSTRCT_TID), MSTRCT_TID))
#define MSTRCT_$0()                       MSTRCT_TID

#define MSTRCT_DATA(id, no_i, flat, typ, lin, tid) \
(__builtin_choose_expr((no_i && __builtin_constant_p(flat)) || !MSTRCT_CHK1, ((typeof(typ))mstrct_addr(id, tid))[flat], \
({asm(""::"r"(flat)); (typeof(typ))mstrct_base(sizeof(*typ), id, mstrct_reset(id, tid), tid); \
MSTRCT_PRAG1}) [({mstrct_check(id, sizeof(*typ), lin, flat, tid); MSTRCT_PRAG0})]))

#define MSTRCT_CLEAN(cnt) struct mstrct_arc; \
MSTRCT_PRAG2 typeof(__builtin_choose_expr(MSTRCT_ARC, (mstrct_pack){}, (mstrct_pass){})) MSTRCT_CAT2(mstrct_clean_, cnt)   \
__attribute__((cleanup(mstrct_set))) = {.id = mstrcty[MSTRCT_TID], .tid = MSTRCT_ARC * (1 + MSTRCT_TID)}; \
typedef struct mstrct_arc mstrct_arc; MSTRCT_PRAG0

#define MSTRCT_HELP(ptr, id, size, key) \
if (ptr == NULL || ptr == ((void *) -1)) {mstrct_error("ALLOC_FAIL", 3, __LINE__, MSTRCT_TID);}   \
mstrct_unit temp = mstrct_put(ptr, key, size, MSTRCT_TID, (MSTRCT_CHK2 ? __LINE__ : 0)); if (!key) id = temp;  \

#endif
