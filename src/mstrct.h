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
 *  MSTRCT_PRINT_FMT              print format string
 *  MSTRCT_ALLOC                block allocator macro
 *
 * literals *****************************************
 *
 *  MSTRCT_BLOCK              reference metadata size
 *  MSTRCT_TNO (>=0)   no of multithreads (over main)
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
#define MSTRCT_$0()                       mstrcterrno[MSTRCT_TID]

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
#define MSTRCT_PARSE_D40(_void_,foo,n)    MSTRCT_LET_D5(foo, n)              

#define MSTRCT_STORE(arg)                 MSTRCT_HAS_COMMA(MSTRCT_ ## arg)  /* 4=addr, 3=alloca, 2=realloc, 1=store, 0=none */
#define MSTRCT_AUTO(arg)                  MSTRCT_HAS_COMMA(_MSTRCT_ ## arg) /* 1=auto, 0=none */
#define MSTRCT_META(arg)                  MSTRCT_HAS_COMMA(MSTRCT__ ## arg) /* 4=size, 3=base, 2=span, 1=id, 0=none */

// user API
#define m(...)                            MSTRCT_CAT2(MSTRCT_$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
#define M(...)                            MSTRCT_CAT2(MSTRCT_$$, MSTRCT_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

static struct {} mstrct_tid;
#define MSTRCT_TID                        ((mstrct_uhalf)__builtin_choose_expr(sizeof(mstrct_tid), mstrct_tid, 0))

#define MSTRCT_CHK1                       MSTRCT_ARG_COUNT(NMSTRCT)
#define MSTRCT_CHK2                       MSTRCT_ARG_COUNT(NMSTRCTH)
#define MSTRCT_CHK3                       MSTRCT_ARG_COUNT(NMSTRCTS)

#define MSTRCT_LINE(name)                 MSTRCT_CAT3(MSTRCT_L,MSTRCT_ARG_COUNT(MSTRCT_SOFT),MSTRCT_ARG_COUNT(MSTRCT_HARD))(name)
#define MSTRCT_L01(name)                  __LINE__
#define MSTRCT_L11(name)                  ((mstrct_unit)sizeof(name.lin[0]))
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
#pragma GCC diagnostic ignored            "-Wstringop-overflow" /*_/\_*/
#endif

#ifndef MSTRCT_PRINT_FMT
  #define MSTRCT_PRINT_FMT                "M_%s/%s/%d\n"
#endif

#if defined(MSTRCT_MCU)
  #define MSTRCT_BLOCK                    1024 // 1 KiB
  typedef unsigned int mstrct_usize;      typedef signed int mstrct_size; // 8,16,32b :: 2,2,4 B
#else
  #include <stdio.h>
  #include <stdlib.h>
  #define MSTRCT_PRINT                    printf
  #define MSTRCT_ALLOC                    malloc
  #define MSTRCT_BLOCK                    (10 * 1024 * 1024) // 10 MiB
  typedef unsigned long mstrct_usize;     typedef signed long mstrct_size; // 64b :: 8B
#endif

typedef unsigned int mstrct_unit;         typedef signed int mstrct_nit;   // 8,16,32,64b :: 2,2,4,4 B
typedef typeof(__builtin_choose_expr(sizeof(mstrct_unit) > 2, (unsigned short)0, (unsigned char)0)) mstrct_uhalf;
typedef typeof(__builtin_choose_expr(sizeof(mstrct_unit) > 2, (unsigned long long)0, (unsigned long)0)) mstrct_utwice;
typedef union {struct {mstrct_unit _mstrct_id; mstrct_uhalf _mstrct_dest; mstrct_uhalf _mstrct_src;};
               mstrct_utwice _mstrct_uni;} mstrct_pack;

#define MSTRCT_SHIFT (8*(sizeof(mstrct_utwice) - sizeof(mstrct_unit) - sizeof(mstrct_uhalf) * MSTRCT_ARG_COUNT(MSTRCT_MCU)))
#define MSTRCT_SIZE(word) ((((mstrct_utwice)(word)) << MSTRCT_SHIFT) >> MSTRCT_SHIFT)

__attribute__((weak)) char mstrcterrno[MSTRCT_TNO + 1];
__attribute__((weak)) mstrct_utwice mstrctblock[MSTRCT_TNO + 1];
__attribute__((weak)) mstrct_unit *mstrctbin[MSTRCT_TNO + 1];
__attribute__((weak)) mstrct_usize *mstrctfixed[MSTRCT_TNO + 1]; static mstrct_usize **restrict mstrct_fixed = mstrctfixed;
__attribute__((weak)) mstrct_unit mstrctx[MSTRCT_TNO + 1] = {[0 ... MSTRCT_TNO] = 2}, mstrcty[MSTRCT_TNO + 1] = {0};

__attribute__((alloc_size(1), noinline, unused, const)) static char*
mstrct_base(mstrct_unit siz, mstrct_unit offset, char var, mstrct_uhalf tid) {
  (void)siz; (void)var; return (char *)(*((mstrct_fixed[tid]) + offset));
}

static inline char*
mstrct_addr(mstrct_unit offset, mstrct_uhalf tid) {return (char *)(*((mstrct_fixed[tid]) + offset));}

static inline mstrct_usize
mstrct_byte(mstrct_unit offset, mstrct_uhalf tid) {return MSTRCT_SIZE(*(mstrct_utwice *)((mstrct_fixed[tid]) + offset + 1));}

__attribute__((noinline, unused, const)) static mstrct_size
mstrct_span(mstrct_unit tsiz, mstrct_unit offset, char var, mstrct_uhalf tid) {
  (void)var; return mstrct_byte(offset, tid) / tsiz;
}

__attribute__((always_inline)) static inline char
mstrct_reset(mstrct_unit offset, mstrct_uhalf tid) {return (char)(*(mstrct_utwice *)((mstrct_fixed[tid]) + offset + 1));}

// memstruct; see doc
#define MSTRCT_T(type, index, line, key) struct {  \
  MSTRCT_CAT3(MSTRCT_TYP_, MSTRCT_ARG_COUNT(MSTRCT_LONG), MSTRCT_ARG_COUNT(key))(type, index) i; \
  mstrct_unit _id;   \
  /* typ[0] */ typeof(type) typ[0] __attribute__((packed)); \
  /* con[0] */ struct {char a[((MSTRCT_CON(type)) ? ((__builtin_constant_p(sizeof(char index))) ? 1 : 0) : 0)];} con[0];   \
  /* lin[0] */ struct {char a[line];} lin[0];   \
  /* dim[0] */ struct {char b; char a[] index [1];} dim[0];   \
}

#define MSTRCT_CON(type) __builtin_types_compatible_p(typeof(type) const *, type *)

#define MSTRCT_TYP_00(type, index) typeof((struct {}){})
#define MSTRCT_TYP_01(type, index) typeof((MSTRCT_CON(type)) ? (mstrct_size const)0 : (mstrct_size)0)
#define MSTRCT_TYP_10(type, index) typeof((struct {}){})
#define MSTRCT_TYP_11(type, index) typeof((MSTRCT_CON(type)) ? (mstrct_nit const)0 : (mstrct_nit)0)

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
  {return index;} else {return mstrct_bounds_error(MSTRCT_ID(id), line, tid);}
}

// utility

#define MSTRCT_TSIZ(name)        ((mstrct_unit)sizeof(*(name.typ[0])))
#define MSTRCT_ASIZ(name, range) ((range) * (mstrct_usize)sizeof(*(name.dim[0].a)))
#define MSTRCT_BSIZ(name, range) (MSTRCT_TSIZ(name) * MSTRCT_ASIZ(name, range))
#define MSTRCT_FLAT(name,idx)    ((mstrct_size)(&(*(typeof(name.dim[0].a) *)0) idx [0]) + \
                                 __builtin_choose_expr(sizeof(name.i), (name.i), (0)))

#define MSTRCT_CLEANUP(store)    (MSTRCT_CHK3 && MSTRCT_AUTO(store))
#define MSTRCT_R(counter)        (MSTRCT_CAT2(mstrct__, counter))
#define MSTRCT_R1(counter)       MSTRCT_CAT2(mstrct_clean_, counter)
#define MSTRCT_PRAG0             _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
#define MSTRCT_PRAG1             _Pragma("GCC diagnostic warning \"-Warray-bounds\"")
#define MSTRCT_CID(ptr)          ((mstrct_pack) {._mstrct_uni = *(mstrct_utwice *)ptr}._mstrct_src)
#define MSTRCT_I(ptr)            ((mstrct_pack) {._mstrct_uni = *(mstrct_utwice *)ptr}._mstrct_id)

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

#define MSTRCT_GET_B1(name)      (*({asm volatile (" " : "+m" (*(mstrct_utwice *)(mstrct_fixed[MSTRCT_TID] + name._id + 1))); \
                                 &(name._id);}))

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

#define MSTRCT_LET_D5(name, n)   \
({if ((mstrct_uhalf)(n) >= MSTRCT_TNO) {mstrct_error("TID_OVF",6,__LINE__,MSTRCT_TID);}; \
mstrctblock[1 + (mstrct_uhalf)(n)] =   \
(mstrct_pack) {._mstrct_dest = 1 + (mstrct_uhalf)(n), ._mstrct_src = MSTRCT_TID, ._mstrct_id = name._id}._mstrct_uni;   \
(void *)&mstrctblock[1 + (mstrct_uhalf)(n)];})

#define MSTRCT_LET_E0(ptr) mstrct_uhalf mstrct_tid = ((mstrct_pack) {._mstrct_uni = *(mstrct_utwice *)ptr}._mstrct_dest)

#define MSTRCT_DEL(de_alloc, name) do {__builtin_choose_expr((sizeof(de_alloc) == 1),   \
  (mstrct_dealloc_0(de_alloc, (name._id), MSTRCT_TID)), (mstrct_dealloc_1(de_alloc, (name._id), __LINE__, MSTRCT_TID))); \
  *(mstrct_fixed[MSTRCT_TID] + name._id + 1) = 0;  \
  *(mstrct_fixed[MSTRCT_TID] + name._id) = (mstrct_usize)(mstrct_fixed[MSTRCT_TID]); \
  asm volatile (" " : "+m" (*(mstrct_utwice *)(mstrct_fixed[MSTRCT_TID] + name._id + 1))); mstrct_archive(name._id, MSTRCT_TID); \
} while(0)

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

static inline void
mstrct_put(mstrct_pack *cleaner, void *arr, mstrct_unit name_id, mstrct_usize size, char cleanup, mstrct_uhalf tid) {
  *(void **)((mstrct_fixed[tid]) + name_id) = arr; *((mstrct_fixed[tid]) + name_id + 1) = size;
  if (cleanup) {cleaner->_mstrct_id = name_id; cleaner->_mstrct_src = tid;}
}

static inline mstrct_unit mstrct_alloc(mstrct_uhalf tid) {
  if ((MSTRCT_ARG_COUNT(MSTRCT_SOFT) == 0) && (mstrctx[tid] & 1023) == 0) { // print ID per 1024, in SOFT MODE
    MSTRCT_PRINT(MSTRCT_PRINT_FMT, "", "ID", mstrctx[tid]);
  }
  if (mstrcty[tid] == 0) { // no archive
    if (__builtin_expect(mstrctx[tid] + 2 > *(mstrctfixed[tid] + 1) / sizeof(mstrct_usize), 0)) {
      mstrct_error("META_OVF", 5, 0, tid);
    } else {mstrctx[tid] += (1 + sizeof(mstrct_utwice) / sizeof(mstrct_usize));}
    return mstrctx[tid] - (1 + sizeof(mstrct_utwice) / sizeof(mstrct_usize));
  } else {return (mstrcty[tid])--;}
}

__attribute__((noinline, unused)) static void
mstrct_archive(mstrct_unit id, mstrct_uhalf tid) {
  if (__builtin_expect(mstrcty[tid] + 2 > *(mstrctfixed[tid] + 1) / (8 * sizeof(mstrct_unit)), 0)) {
    mstrct_error("META_OVF", 5, 0, tid);
  } else {mstrcty[tid] += 1;} *(mstrctbin[tid] + mstrcty[tid]) = id;
}

__attribute__((always_inline)) static inline void
mstrct_assign(mstrct_unit id, mstrct_unit line, mstrct_uhalf tid, mstrct_usize size, void *ptr) {
  if (ptr == NULL || ptr == ((void *) -1)) {mstrct_error("ALLOC_FAIL", 3, line, tid);}   \
  *(mstrct_fixed[tid] + id) = (mstrct_usize)ptr; *(mstrct_utwice *)(mstrct_fixed[tid] + id + 1) =
  ((mstrct_utwice)size | ((mstrct_utwice)(mstrct_uhalf)line << (8 * sizeof(mstrct_utwice) - MSTRCT_SHIFT)));
}

__attribute__((always_inline)) static inline void
mstrct_set(void *ptr) {
  *(mstrct_fixed[((mstrct_pack *)ptr)->_mstrct_src] + ((mstrct_pack *)ptr)->_mstrct_id + 1) = 0;
  asm volatile
    (" " : "+m" (*(mstrct_utwice *)(mstrct_fixed[((mstrct_pack *)ptr)->_mstrct_src] + ((mstrct_pack *)ptr)->_mstrct_id + 1)));
  mstrct_archive(((mstrct_pack *)ptr)->_mstrct_id, ((mstrct_pack *)ptr)->_mstrct_src);
}

__attribute__((constructor)) static inline void
mstrct_init(void) {
  if (mstrctfixed[0] == 0) {void *space, *time;
    for (mstrct_uhalf i = 0; i <= MSTRCT_TNO; i++) {
      if (mstrctblock[i] == 0) mstrctblock[i] = MSTRCT_BLOCK;
      space = MSTRCT_ALLOC(mstrctblock[i]); time = MSTRCT_ALLOC(mstrctblock[i] / 8);
      if (space == NULL || time == NULL) {mstrct_error("ALLOC_FAIL", 3, 0, MSTRCT_TID); __builtin_trap();}
      mstrct_fixed[i] = space; mstrctbin[i] = time; *(mstrctfixed[i] + 1) = (mstrct_usize)mstrctblock[i];
    }
  } if (mstrct_fixed[0] == NULL) {for (mstrct_uhalf i = 0; i <= MSTRCT_TNO; i++) {mstrct_fixed[i] = mstrctfixed[i];}}
}

__attribute__((destructor)) static inline void
mstrct_leak(void) {
  if (MSTRCT_CHK2 && mstrctfixed[0] != (mstrct_usize *)7) {
    for (mstrct_uhalf i = 0; i <= MSTRCT_TNO; i++) {
      for (mstrct_unit j = 2; j <= mstrctx[i]; j += 1 + sizeof(mstrct_utwice) / sizeof(mstrct_usize)) {
        if (*(mstrct_utwice *)(mstrctfixed[i] + j + 1) != 0) {
          mstrct_unit line = (*(mstrct_utwice *)(mstrctfixed[i] + j + 1)) >> (8 * sizeof(mstrct_utwice) - MSTRCT_SHIFT);
          if (line != 0) {MSTRCT_PRINT(MSTRCT_PRINT_FMT, "LEAK", __BASE_FILE__, line);}
        }
      }
    }
  } mstrctfixed[0] = (mstrct_usize *)7;
}


#endif
