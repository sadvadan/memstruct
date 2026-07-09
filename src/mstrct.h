/**
 *  Copyright(C) 2026 @sadvadan (github.com/sadvadan)
 *
 *  memstruct v1.0.0
 *  "Zero Cost Abstraction"         -std=gnu99 & onws
 *
 * SUMMARY ******************************************
 *
 *  Single header library for 8-64 bit CPUs, covering
 *  memory & thread safety through m()/M() macro API.
 *  Uses compile & runtime optimizations and hoisting
 *  to check OOB/UAF/NULL-deref/double-free and leaks
 *
 * errcodes & errnos ********************************
 *
 *  "BAD" (ILLEGAL_ACCESS) ........................ 1
 *  "OOB" (BOUNDS_CHECK_FAIL) ..................... 2
 *  "ALLOC_FAIL" .................................. 3
 *  "DE_ALLOC_FAIL" ............................... 4
 *  "META_OVF" (METADATA_OVERFLOW)................. 5
 *  "TID_OVF" (TID_OVERFLOW)....................... 6
 *
 * flags ********************************************
 *
 *  NMSTRCTS            disable stack temporal safety
 *  NMSTRCTH             disable heap temporal safety
 *  NMSTRCT                    disable spatial safety
 *  MSTRCT_SOFT                print err_site line_no
 *  MSTRCT_HARD                segfault at error site
 *  MSTRCT_LONG                       make foo.i long
 *  MSTRCT_MCU                        enable MCU mode
 *
 * MCU APIs *****************************************
 *
 *  MSTRCT_PRINT                          print macro
 *  MSTRCT_PRINT_FMT   print format string (optional)
 *  MSTRCT_ALLOC                block allocator macro
 *
 * literals *****************************************
 *
 *  MSTRCT_BLOCK              reference metadata size
 *  MSTRCT_TNO         no of multithreads (over main)
 *
 **/


#ifndef MSTRCT_H
#define MSTRCT_H

#define MSTRCT_VER_MAJOR                  1
#define MSTRCT_VER_MINOR                  0
#define MSTRCT_VER_PATCH                  0

#define MSTRCT_auto                       ~,1
#define MSTRCT_static                     ~,1
#define MSTRCT___thread                   ~,1
#define MSTRCT_extern                     ~,1
#define MSTRCT_realloc                    ~,~,2
#define MSTRCT_mremap                     ~,~,2
#define MSTRCT_alloca                     ~,~,~,3
#define MSTRCT___builtin_alloca           ~,~,~,3
#define MSTRCT_void                       ~,~,~,~,4

#define _MSTRCT___thread
#define _MSTRCT_static
#define _MSTRCT_auto                      ~,1

#define MSTRCT__enum                      ~,1
#define MSTRCT___                         ~,~,2
#define MSTRCT__void                      ~,~,~,3
#define MSTRCT__sizeof                    ~,~,~,~,4

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

#define MSTRCT_INDEX1(...)                MSTRCT_CAT2(MSTRCT_IDX, MSTRCT_ARG_COUNT(__VA_ARGS__)) (__VA_ARGS__)
#define MSTRCT_INDEX0(a,...)              [0] MSTRCT_CAT2(MSTRCT_IDX, MSTRCT_ARG_COUNT(__VA_ARGS__)) (__VA_ARGS__)
#define MSTRCT_FIRST(a,...)               MSTRCT_ARG_COUNT(a)

#define MSTRCT_$3(foo,typ,i)              MSTRCT_CAT2(MSTRCT_PARSE_A, MSTRCT_PARSE(i, MSTRCT_META))(foo, typ, i)
#define MSTRCT_$2(foo,i)                  MSTRCT_CAT2(MSTRCT_PARSE_B, MSTRCT_PARSE(i, MSTRCT_META))(foo, i)
#define MSTRCT_$1(foo)                    MSTRCT_GET0(foo._id, (typeof(foo.typ[0])), MSTRCT_TSIZ(foo), MSTRCT_TID,  \
                                          MSTRCT_LINE(foo), sizeof(foo.con[0]))
#define MSTRCT_$0()                       mstrct_errno[MSTRCT_TID]

#define MSTRCT_$$4(typ1,typ2,foo,i)       MSTRCT_CAT3(MSTRCT_PARSE_C, MSTRCT_PARSE(i, MSTRCT_AUTO),  \
                                          MSTRCT_ARG_COUNT(i))(typ1, typ2, foo, i)
#define MSTRCT_$$3(store,foo,range)       MSTRCT_CAT3(MSTRCT_PARSE_D, MSTRCT_PARSE(store, MSTRCT_STORE), \
                                          MSTRCT_PARSE(range, MSTRCT_AUTO))(store,foo,range)
#define MSTRCT_$$2(de_store,foo)          MSTRCT_DEL(de_store, foo)
#define MSTRCT_$$1(foo)                   MSTRCT_LET_E0(foo)

#define MSTRCT_PARSE(i, macr)             MSTRCT_CAT2(MSTRCT_PARSE_, MSTRCT_ARG_COUNT(MSTRCT_DUMMY i))(i, macr)
#define MSTRCT_DUMMY(...)
#define MSTRCT_EXPAND(...)                __VA_ARGS__
#define MSTRCT_LIST(...)                  {__VA_ARGS__}
#define MSTRCT_PARSE_1(i, macr)           macr(i)  // keyword
#define MSTRCT_PARSE_0(i, macr)           5        // multi-index

#define MSTRCT_PARSE_A0(foo, typ, n)      MSTRCT_GET_A0(foo, typ, (n))              
#define MSTRCT_PARSE_A2(foo, typ, span)   MSTRCT_GET_A2(foo, typ)              
#define MSTRCT_PARSE_A3(foo, typ, base)   MSTRCT_GET_A3(foo, typ)              
#define MSTRCT_PARSE_A4(foo, typ, size)   MSTRCT_GET_A4(foo, typ)              

#define MSTRCT_PARSE_B0(foo, n)           MSTRCT_GET_B0(foo, (n))              
#define MSTRCT_PARSE_B1(foo, id)          MSTRCT_GET_B1(foo)              
#define MSTRCT_PARSE_B2(foo, span)        MSTRCT_GET_B2(foo)              
#define MSTRCT_PARSE_B3(foo, base)        MSTRCT_GET_B3(foo)              
#define MSTRCT_PARSE_B4(foo, size)        MSTRCT_GET_B4(foo)              
#define MSTRCT_PARSE_B5(foo, index)       MSTRCT_GET_B5(foo, index)

#define MSTRCT_PARSE_C00(ty1,ty2,foo,i)   MSTRCT_LET_C0(ty1, ty2, foo)              
#define MSTRCT_PARSE_C01(ty1,ty2,foo,i)   MSTRCT_LET_C1(ty1, ty2, foo, (i))              
#define MSTRCT_PARSE_C51(ty1,ty2,foo,i)   MSTRCT_LET_C2(ty1, ty2, foo, i)              

#define MSTRCT_PARSE_D00(alloc,foo,r)     MSTRCT_LET_D0(alloc, foo, r)              
#define MSTRCT_PARSE_D10(store,foo,r)     MSTRCT_LET_D1(store, foo, r, __COUNTER__)              
#define MSTRCT_PARSE_D15(store,foo,ini)   MSTRCT_LET_D2(store, foo, ini, __COUNTER__)
#define MSTRCT_PARSE_D20(_realloc,foo,n)  MSTRCT_LET_D3(_realloc, foo, n)              
#define MSTRCT_PARSE_D30(_alloca,foo,n)   MSTRCT_LET_D4(_alloca, foo, n)              
#define MSTRCT_PARSE_D40(_union_,foo,n)   MSTRCT_LET_D5(foo, n)              

#define MSTRCT_STORE(arg)                 MSTRCT_HAS_COMMA(MSTRCT_ ## arg)  /* 4=mstrct, 3=alloca, 2=realloc, 1=store, 0=none */
#define MSTRCT_AUTO(arg)                  MSTRCT_HAS_COMMA(_MSTRCT_ ## arg) /* 1=auto, 0=none */
#define MSTRCT_META(arg)                  MSTRCT_HAS_COMMA(MSTRCT__ ## arg) /* 4=size, 3=base, 2=span, 1=id, 0=none */

// user API
#define m(...)                            MSTRCT_CAT2(MSTRCT_$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define M(...)                            MSTRCT_CAT2(MSTRCT_$$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

static struct {} mstrct_tid;
#define MSTRCT_TID                        ((short)__builtin_choose_expr(sizeof(mstrct_tid), mstrct_tid, -1*(MSTRCT_TNO != 0)))

#define MSTRCT_CHK1                       MSTRCT_ARG_COUNT(NMSTRCT)
#define MSTRCT_CHK2                       MSTRCT_ARG_COUNT(NMSTRCTH)
#define MSTRCT_CHK3                       MSTRCT_ARG_COUNT(NMSTRCTS)

#define MSTRCT_LINE(name)                 MSTRCT_CAT3(MSTRCT_L,MSTRCT_ARG_COUNT(MSTRCT_SOFT),MSTRCT_ARG_COUNT(MSTRCT_HARD))(name)
#define MSTRCT_L01(name)                  __LINE__
#define MSTRCT_L11(name)                  ((mstrct_uint)sizeof(name.lin[0]))
#define MSTRCT_L10(name)                  0
#define MSTRCT_ID(id)                     MSTRCT_CAT2(MSTRCT_ID_, MSTRCT_ARG_COUNT(MSTRCT_HARD))(id)
#define MSTRCT_ID_0(id)                   0
#define MSTRCT_ID_1(id)                   (id)

#define MSTRCT_ARG_COUNT(...)             MSTRCT_MACR16(10 __VA_OPT__(,) ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define MSTRCT_HAS_COMMA(...)             MSTRCT_ARG6(__VA_ARGS__, 4, 3, 2, 1, 0)
#define MSTRCT_ARG6(_1, _2, _3, _4, _5, _6, ...)  _6
#define MSTRCT_MACR16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME,...) NAME

#ifndef MSTRCT_TNO
  #define MSTRCT_TNO 0
#endif

#ifndef __clang__
#pragma GCC diagnostic ignored "-Wstringop-overflow" /*_/\_*/
#pragma GCC diagnostic warning "-Warray-bounds=2"
#endif

#if defined(MSTRCT_MCU)
  #define MSTRCT_BLOCK                    1024 // 1 KiB
  typedef unsigned int mstrct_ulong; typedef signed int mstrct_long;
#else
  #include <stdio.h>
  #include <stdlib.h>
  #define MSTRCT_PRINT                    printf
  #define MSTRCT_PRINT_FMT                "M_%s/%s/%d\n"
  #define MSTRCT_ALLOC                    malloc
  #define MSTRCT_BLOCK                    (10 * 1024 * 1024) // 10 MiB
  typedef unsigned long long mstrct_ulong; typedef signed long long mstrct_long;
#endif

typedef unsigned int mstrct_uint; typedef signed int mstrct_int;
typedef struct {union {void *mstrct_ptr; struct {short mstrct_dest; short mstrct_src; mstrct_uint mstrct_id;};};} mstrct_pack;

__attribute__((weak)) mstrct_ulong mstrctsize[MSTRCT_TNO + 1] = {[0 ... MSTRCT_TNO] = MSTRCT_BLOCK};
static mstrct_ulong *mstrct_size = &mstrctsize[MSTRCT_TNO ? 1 : 0];

__attribute__((weak)) mstrct_ulong *mstrctfixed[MSTRCT_TNO + 1];
static mstrct_ulong **restrict mstrct_fixed = &mstrctfixed[MSTRCT_TNO ? 1 : 0];

__attribute__((weak)) mstrct_uint *mstrctbin[MSTRCT_TNO + 1]; static mstrct_uint **mstrct_bin = &mstrctbin[MSTRCT_TNO ? 1 : 0];
__attribute__((weak)) mstrct_uint mstrctx[MSTRCT_TNO + 1] = {[0 ... MSTRCT_TNO] = 2}, mstrcty[MSTRCT_TNO + 1] = {0};

static volatile mstrct_uint *mstrct_x = &mstrctx[MSTRCT_TNO ? 1 : 0], *mstrct_y = &mstrcty[MSTRCT_TNO ? 1 : 0];
__attribute__((weak)) char mstrcterrno[MSTRCT_TNO + 1]; static char *mstrct_errno = &mstrcterrno[MSTRCT_TNO ? 1 : 0];

__attribute__((alloc_size(1), noinline, unused, const)) static char*
mstrct_base(mstrct_uint siz, mstrct_uint offset, char var, short tid) {
  (void)siz; (void)var; return (char *)((*((mstrct_fixed[tid]) + offset) << 8) >> 8);
}

static inline char*
mstrct_addr(mstrct_uint offset, short tid) {return (char *)((*((mstrct_fixed[tid]) + offset) << 8) >> 8);}

static inline mstrct_ulong
mstrct_byte(mstrct_uint offset, short tid) {return ((*((mstrct_fixed[tid]) + offset + 1) << 8) >> 8);}

__attribute__((noinline, unused, const)) static mstrct_long
mstrct_span(mstrct_uint siz, mstrct_uint offset, char var, short tid) {
  (void)var; return (mstrct_long)mstrct_byte(offset, tid) / siz;
}

__attribute__((always_inline)) static inline char
mstrct_reset(mstrct_uint offset, short tid) {return (char)(*((mstrct_fixed[tid]) + offset + 1));}

// memstruct; see doc
#define MSTRCT_T(type, index, line, key) struct {  \
  MSTRCT_CAT3(MSTRCT_TYP_, MSTRCT_ARG_COUNT(MSTRCT_LONG), MSTRCT_ARG_COUNT(key))(type, index) i; \
  mstrct_uint _id;   \
  /* typ[0] */ typeof(type) typ[0] __attribute__((packed)); \
  /* con[0] */ struct {char a[((MSTRCT_CON(type)) ? ((__builtin_constant_p(sizeof(char index))) ? 1 : 0) : 0)];} con[0];   \
  /* lin[0] */ struct {char a[line];} lin[0];   \
  /* dim[0] */ struct {char b; char a[] index [1];} dim[0];   \
}

#define MSTRCT_CON(type) __builtin_types_compatible_p(typeof(type) const *, type *)

#define MSTRCT_TYP_00(type, index) typeof((struct {}){})
#define MSTRCT_TYP_01(type, index) typeof((MSTRCT_CON(type)) ? (mstrct_long const)0 : (mstrct_long)0)
#define MSTRCT_TYP_10(type, index) typeof((struct {}){})
#define MSTRCT_TYP_11(type, index) typeof((MSTRCT_CON(type)) ? (mstrct_int const)0 : (mstrct_int)0)

__attribute__((cold)) static inline void
mstrct_error(const char *ops, const char err_no, const mstrct_int line, short tid) {
  MSTRCT_PRINT(MSTRCT_PRINT_FMT, ops, __BASE_FILE__, line);
  if (MSTRCT_ARG_COUNT(MSTRCT_HARD) == 0) {__builtin_trap();} else {mstrct_errno[tid] = err_no;}
}

__attribute__((cold)) static inline mstrct_long
mstrct_bounds_error(mstrct_int _d, mstrct_int line, short tid) {
  if (mstrct_byte(_d, tid) == 0) {mstrct_error("BAD", 1, line, tid);} else {mstrct_error("OOB", 2, line, tid);}; return 0;
}

__attribute__((hot)) static inline mstrct_long
mstrct_check(mstrct_uint id, mstrct_uint type_size, mstrct_int line, mstrct_long index, short tid) {
  if (__builtin_expect(((mstrct_ulong)mstrct_span(type_size, id, mstrct_reset(id, tid), tid) > (mstrct_ulong)index), 1))
  {return index;} else {return mstrct_bounds_error(MSTRCT_ID(id), line, tid);}
}

// utility

#define MSTRCT_TSIZ(name)        ((mstrct_uint)sizeof(*(name.typ[0])))
#define MSTRCT_ASIZ(name, range) ((range) * (mstrct_ulong)sizeof(*(name.dim[0].a)))
#define MSTRCT_BSIZ(name, range) (MSTRCT_TSIZ(name) * MSTRCT_ASIZ(name, range))
#define MSTRCT_FLAT(name,idx)    ((mstrct_long)(&(*(typeof(name.dim[0].a) *)0) idx [0]) + \
                                 __builtin_choose_expr(sizeof(name.i), (name.i), (0)))

#define MSTRCT_CLEANUP(store)    (MSTRCT_CHK3 && MSTRCT_AUTO(store))
#define MSTRCT_R(counter)        (MSTRCT_CAT2(mstrct__, counter))
#define MSTRCT_R1(counter)       MSTRCT_CAT2(mstrct_clean_, counter)
#define MSTRCT_PRAG0             _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
#define MSTRCT_PRAG1             _Pragma("GCC diagnostic warning \"-Warray-bounds\"")
#define MSTRCT_CID(ptr)          ((mstrct_pack) {.mstrct_ptr = ptr}.mstrct_src)
#define MSTRCT_I(ptr)            ((mstrct_pack) {.mstrct_ptr = ptr}.mstrct_id)

#define MSTRCT_CLEAN(store)      MSTRCT_CAT3(MSTRCT_CLEAN_, MSTRCT_CHK3, MSTRCT_AUTO(store))
#define MSTRCT_CLEAN_11          __attribute__((cleanup(mstrct_set)))
#define MSTRCT_CLEAN_00
#define MSTRCT_CLEAN_10
#define MSTRCT_CLEAN_01

// GET

#define MSTRCT_GET_A0(ptr,typ,i) \
({MSTRCT_CAT3(MSTRCT_GET_, MSTRCT_FIRST i, MSTRCT_CHK1) (MSTRCT_I(ptr), i, (typ *), sizeof(typ), MSTRCT_CID(ptr), __LINE__, 0);})

#define MSTRCT_GET_A2(ptr,typ)   \
(mstrct_span(sizeof(typ), MSTRCT_I(ptr), mstrct_reset(MSTRCT_I(ptr), MSTRCT_CID(ptr)), MSTRCT_CID(ptr)))

#define MSTRCT_GET_A3(ptr,typ)   \
mstrct_base(sizeof(typ), MSTRCT_I(ptr), mstrct_reset(MSTRCT_I(ptr), MSTRCT_CID(ptr)), MSTRCT_CID(ptr))

#define MSTRCT_GET_A4(ptr,typ)   (mstrct_byte(MSTRCT_I(ptr), MSTRCT_CID(ptr)))

#define MSTRCT_GET_B0(name,i)    MSTRCT_GET_B5(name, i)

#define MSTRCT_GET_B1(name)      (*({asm volatile (" " : "+m" (*(mstrct_fixed[MSTRCT_TID] + name._id + 1))); &(name._id);}))

#define MSTRCT_GET_B2(name)      (mstrct_span(MSTRCT_TSIZ(name), name._id, mstrct_reset(name._id, MSTRCT_TID), MSTRCT_TID))

#define MSTRCT_GET_B3(name)      (mstrct_base(MSTRCT_TSIZ(name), name._id, mstrct_reset(name._id, MSTRCT_TID), MSTRCT_TID))

#define MSTRCT_GET_B4(name)      (mstrct_byte(name._id, MSTRCT_TID))

#define MSTRCT_GET_B5(name,i)    \
MSTRCT_CAT3(MSTRCT_GET_, MSTRCT_FIRST i, MSTRCT_CHK1) (name._id, MSTRCT_FLAT(name, MSTRCT_CAT2(MSTRCT_INDEX, MSTRCT_FIRST i) i), \
(typeof(name.typ[0])), MSTRCT_TSIZ(name), MSTRCT_TID, MSTRCT_LINE(name), (sizeof(name.con[0])))

#define MSTRCT_GET0(id, typ, tsiz, cid, lin, con)  \
({typ(mstrct_base(tsiz, id, mstrct_reset(id, cid),  cid)); MSTRCT_PRAG0}) [({mstrct_check(id, tsiz, lin, 0, cid); MSTRCT_PRAG1})]

#define MSTRCT_GET_10(id, i, typ, tsiz, cid, lin, con) (typ(mstrct_base(__UINT32_MAX__, id, mstrct_reset(id, cid), cid))) [i]

#define MSTRCT_GET_11(id, i, typ, tsiz, cid, lin, con) ({(void)sizeof(i); \
typ(mstrct_base(tsiz, id, mstrct_reset(id, cid), cid)); MSTRCT_PRAG0}) [({mstrct_check(id, tsiz, lin, i, cid); MSTRCT_PRAG1})]

#define MSTRCT_GET_00(id, i, typ, tsiz, cid, lin, con) MSTRCT_GET_10(id, i, typ, tsiz, cid, lin)

#define MSTRCT_GET_01(id, i, typ, tsiz, cid, lin, con) (__builtin_choose_expr(con && __builtin_constant_p(sizeof(char[i])),  \
MSTRCT_GET_10(id, i, typ, tsiz, cid, lin, con), MSTRCT_GET_11(id, i, typ, tsiz, cid, lin, con)))

// LET

#define MSTRCT_LET_C0(ty1, ty2, name) MSTRCT_T(ty1 *ty2,, __LINE__, ) name

#define MSTRCT_LET_C1(ty1, ty2, name, i) MSTRCT_T(ty1 *ty2, MSTRCT_INDEX1 i, __LINE__, 1) name

#define MSTRCT_LET_C2(ty1, ty2, name, i) MSTRCT_T(ty1 *ty2, MSTRCT_INDEX1 i, __LINE__, ) name

#define MSTRCT_LET_D0(alloc, name, range) do {   \
  __builtin_memset(&name, 0, sizeof(name)); char *ptr = (char *)(alloc);   \
  name._id = mstrct_alloc(MSTRCT_TID); mstrct_assign(name._id, __LINE__, MSTRCT_TID, MSTRCT_BSIZ(name, range), ptr); \
} while(0)

#define MSTRCT_LET_D1(store, name, range, cnt)  \
store typeof(*(name.typ[0])) MSTRCT_CAT2(mstrct__, cnt)[MSTRCT_ASIZ(name, range)] __attribute__((aligned(MSTRCT_TSIZ(name))));   \
mstrct_pack MSTRCT_CAT2(mstrct_clean_, cnt)[MSTRCT_CLEANUP(store)] MSTRCT_CLEAN(store); \
if (sizeof(MSTRCT_CAT2(mstrct__, cnt))) {  \
  __builtin_memset(&name, 0, sizeof(name)); name._id = mstrct_alloc(MSTRCT_TID);  \
  mstrct_put(MSTRCT_R1(cnt), MSTRCT_R(cnt), (name._id), MSTRCT_BSIZ(name, range), MSTRCT_CLEANUP(store), MSTRCT_TID); \
}

#define MSTRCT_LET_D2(store, name, ini, cnt) store typeof(*(name.typ[0]))  \
MSTRCT_CAT2(mstrct__, cnt)[MSTRCT_ASIZ(name, MSTRCT_ARG_COUNT ini)] __attribute__((aligned(MSTRCT_TSIZ(name)))) = \
MSTRCT_LIST(MSTRCT_EXPAND ini); mstrct_pack MSTRCT_CAT2(mstrct_clean_, cnt)[MSTRCT_CLEANUP(store)] MSTRCT_CLEAN(store); \
if (sizeof(MSTRCT_CAT2(mstrct__, cnt))) {_Static_assert(!sizeof(name.i), "M_ERR: " #name " must not have static index!");  \
  __builtin_memset(&name, 0, sizeof(name)); name._id = mstrct_alloc(MSTRCT_TID);  \
  mstrct_put(MSTRCT_R1(cnt),MSTRCT_R(cnt),(name)._id,MSTRCT_BSIZ(name,MSTRCT_ARG_COUNT ini),MSTRCT_CLEANUP(store),MSTRCT_TID); \
}

#define MSTRCT_LET_D3(re_alloc, name, range) do {   \
  char *ptr = (char *)(re_alloc); mstrct_assign(name._id, __LINE__, MSTRCT_TID, MSTRCT_BSIZ(name, range), ptr); \
} while(0)

#define MSTRCT_LET_D4(allo_ca, name, range) do {   \
  char *ptr = (char *)(allo_ca); __builtin_memset(&name, 0, sizeof(name)); name._id = mstrct_alloc(MSTRCT_TID);  \
  *(void **)((mstrct_fixed[MSTRCT_TID]) + name._id) = ptr;  \
  *((mstrct_fixed[MSTRCT_TID]) + name._id + 1) = MSTRCT_BSIZ(name, range);   \
} while(0)

#define MSTRCT_LET_D5(name, n) ({if ((unsigned short)(n) >= MSTRCT_TNO) {mstrct_error("TID_OVF", 6, __LINE__, MSTRCT_TID);}; \
(mstrct_pack) {.mstrct_dest = (short)(n), .mstrct_src = MSTRCT_TID, .mstrct_id = name._id}.mstrct_ptr;})

#define MSTRCT_LET_E0(ptr) short mstrct_tid = ((mstrct_pack) {.mstrct_ptr = ptr}.mstrct_dest)

#define MSTRCT_DEL(de_alloc, name) do {__builtin_choose_expr((sizeof(de_alloc) == 1),   \
  (mstrct_dealloc_0(de_alloc, (name._id), MSTRCT_TID)), (mstrct_dealloc_1(de_alloc, (name._id), __LINE__, MSTRCT_TID))); \
  *(mstrct_fixed[MSTRCT_TID] + name._id + 1) = 0;  \
  *(mstrct_fixed[MSTRCT_TID] + name._id) = (mstrct_ulong)(mstrct_fixed[MSTRCT_TID]); \
  asm volatile (" " : "+m" (*(mstrct_fixed[MSTRCT_TID] + name._id + 1))); mstrct_archive(name._id, MSTRCT_TID); \
} while(0)

// prototypes for free() & munmap()                                                                                   
typedef void (*mstrct_free_proto)(void *); typedef mstrct_int (*mstrct_munmap_proto)(void *, mstrct_ulong);

static inline void
mstrct_dealloc_0(void *fun, mstrct_uint id, short tid) {
  if (mstrct_byte(id, tid) != 0) {((mstrct_free_proto)fun)(mstrct_addr(id, tid));}
}

static inline void
mstrct_dealloc_1(void *fun, mstrct_uint id, mstrct_uint line, short tid) {
  if (mstrct_byte(id, tid) != 0) {
    if (((mstrct_munmap_proto)fun)(mstrct_addr(id, tid), mstrct_byte(id, tid)) == -1) {
      mstrct_error("DEALLOC_FAIL", 4, line, tid);
    }
  }
}

static inline void
mstrct_put(mstrct_pack *cleaner, void *arr, mstrct_uint name_id, mstrct_ulong size, char cleanup, short tid) {
  *(void **)((mstrct_fixed[tid]) + name_id) = arr; *((mstrct_fixed[tid]) + name_id + 1) = size;
  if (cleanup) {cleaner->mstrct_id = name_id; cleaner->mstrct_src = tid;}
}

static inline mstrct_uint mstrct_alloc(short tid) {
  if ((MSTRCT_ARG_COUNT(MSTRCT_SOFT) == 0) && (mstrct_x[tid] & 1023) == 0) { // print ID per 1024, in SOFT MODE
    MSTRCT_PRINT(MSTRCT_PRINT_FMT, "", "ID", mstrct_x[tid]);
  }
  if (mstrct_y[tid] == 0) { // no archive
    if (__builtin_expect(mstrct_x[tid] + 2 > mstrct_size[tid] / sizeof(mstrct_ulong), 0)) {
      mstrct_error("META_OVF", 5, 0, tid);
    } else {mstrct_x[tid] += 2;} return mstrct_x[tid] - 2;
  } else {return (mstrct_y[tid])--;}
}

__attribute__((noinline, unused)) static void
mstrct_archive(mstrct_uint id, short tid) {
  if (__builtin_expect(mstrct_y[tid] + 2 > mstrct_size[tid] / (8 * sizeof(mstrct_uint)), 0)) {
    mstrct_error("META_OVF", 5, 0, tid);
  } else {mstrct_y[tid] += 1;} *(mstrct_bin[tid] + mstrct_y[tid]) = id;
}

__attribute__((always_inline)) static inline void
mstrct_assign(mstrct_uint id, mstrct_uint line, short tid, mstrct_ulong size, void *ptr) {
  if (ptr == NULL || ptr == ((void *) -1)) {mstrct_error("ALLOC_FAIL", 3, line, tid);}   \
  *(mstrct_fixed[tid] + id) = ((((mstrct_ulong)ptr << 8) >> 8) | (((mstrct_ulong)line >> 8) << (8*sizeof(mstrct_ulong) - 8)));
  *(mstrct_fixed[tid] + id + 1) = (size | ((mstrct_ulong)line << (8 * sizeof(mstrct_ulong) - 8)));
}

__attribute__((always_inline)) static inline void
mstrct_set(void *ptr) {
  *(mstrct_fixed[((mstrct_pack *)ptr)->mstrct_src] + ((mstrct_pack *)ptr)->mstrct_id + 1) = 0;
  asm volatile (" " : "+m" (*(mstrct_fixed[((mstrct_pack *)ptr)->mstrct_src] + ((mstrct_pack *)ptr)->mstrct_id + 1)));
  mstrct_archive(((mstrct_pack *)ptr)->mstrct_id, ((mstrct_pack *)ptr)->mstrct_src);
}

__attribute__((constructor)) static inline void
mstrct_init(void) {
  if (mstrctfixed[0] == 0) {void *space, *time;
    for (mstrct_uint i = 0; i <= (MSTRCT_TNO ? MSTRCT_TNO : 0); i++) {
      space = MSTRCT_ALLOC(mstrctsize[i]); time = MSTRCT_ALLOC(mstrctsize[i] / 8);
      if (space == NULL || time == NULL) {mstrct_error("ALLOC_FAIL", 3, 0, MSTRCT_TID); __builtin_trap();}
      mstrct_fixed[MSTRCT_TNO ? (short)i - 1 : 0] = space; mstrctbin[i] = time;
    }
  } if (mstrct_fixed[0] == NULL) {for (short i = 0; i <= MSTRCT_TNO; i++) {mstrct_fixed[MSTRCT_TNO ? i-1 : 0] = mstrctfixed[i];}}
}

__attribute__((destructor)) static inline void
mstrct_leak(void) {
  if (MSTRCT_CHK2) { 
    for (mstrct_uint i = 0; i <= MSTRCT_TNO; i++) {
      for (mstrct_uint j = 2; j <= mstrctx[i]; j += 2) {
        if (*(mstrctfixed[i] + j + 1) != 0) {
          mstrct_uint line = ((mstrct_uint)0 | (*(mstrctfixed[i] + j + 1) >> (8 * sizeof(mstrct_ulong) - 8))) |
          (*((mstrct_ulong *)mstrctfixed[i] + j) >> (8 * sizeof(mstrct_ulong) - 16));
          if (line != 0) {MSTRCT_PRINT(MSTRCT_PRINT_FMT, "LEAK", __BASE_FILE__, line);}
        }
      }
    }
  }
}


#endif
