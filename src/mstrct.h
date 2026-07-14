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

#define _MSTRCT_auto                      ~,1

#define MSTRCT_auto                       ~,1
#define MSTRCT_static                     ~,1
#define MSTRCT___thread                   ~,1
#define MSTRCT_extern                     ~,1
#define MSTRCT_realloc                    ~,~,2
#define MSTRCT_mremap                     ~,~,2
#define MSTRCT_alloca                     ~,~,~,3
#define MSTRCT___builtin_alloca           ~,~,~,3
#define MSTRCT_mstrct_base                ~,~,~,3

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

#define MSTRCT_$$4(typ1,typ2,foo,i)       MSTRCT_CAT3(MSTRCT_PARSE_C, MSTRCT_PARSE(i, MSTRCT_STORE),  \
                                          MSTRCT_ARG_COUNT(i))(typ1, typ2, foo, i)
#define MSTRCT_$$3(store,foo,range)       MSTRCT_CAT3(MSTRCT_PARSE_D, MSTRCT_PARSE(store, MSTRCT_STORE), \
                                          MSTRCT_PARSE(range, MSTRCT_STORE))(store,foo,range)
#define MSTRCT_$$2(de_store,foo)          MSTRCT_DEL(de_store, foo)
#define MSTRCT_$$1(foo)                   MSTRCT_LET_E0(foo)

#define MSTRCT_PARSE(i, macr)             MSTRCT_CAT2(MSTRCT_PARSE_, MSTRCT_ARG_COUNT(MSTRCT_DUMMY i))(i, macr)
#define MSTRCT_DUMMY(...)
#define MSTRCT_EXPAND(...)                __VA_ARGS__
#define MSTRCT_LIST(...)                  {__VA_ARGS__}
#define MSTRCT_PARSE_1(i, macr)           macr(i)  // keyword
#define MSTRCT_PARSE_0(i, macr)           5        // multi-index

#define MSTRCT_PARSE_A0(foo, n, typ)      MSTRCT_GET_A0(foo, typ, (n))              
#define MSTRCT_PARSE_A1(foo, id, tid)     MSTRCT_GET_A1(foo, (tid))              
#define MSTRCT_PARSE_A2(foo, span, typ)   MSTRCT_GET_A2(foo, typ)              
#define MSTRCT_PARSE_A3(foo, base, typ)   MSTRCT_GET_A3(foo, typ)              
#define MSTRCT_PARSE_A4(foo, size, typ)   MSTRCT_GET_A4(foo, typ)              

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

#define MSTRCT_STORE(arg)                 MSTRCT_HAS_COMMA(MSTRCT_ ## arg)  /* 3=alloca, 2=realloc, 1=store, 0=none */
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
#define MSTRCT_HID(id)                    MSTRCT_CAT2(MSTRCT_HID_, MSTRCT_ARG_COUNT(MSTRCT_HARD))(id)
#define MSTRCT_HID_0(id)                  0
#define MSTRCT_HID_1(id)                  (id)

#define MSTRCT_ARG_COUNT(...)             MSTRCT_MACR16(10 __VA_OPT__(,) ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define MSTRCT_HAS_COMMA(...)             MSTRCT_ARG6(__VA_ARGS__, 4, 3, 2, 1, 0)
#define MSTRCT_ARG6(_1, _2, _3, _4, _5, _6, ...)  _6
#define MSTRCT_MACR16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME,...) NAME

typedef unsigned int mstrct_unit;         typedef signed int mstrct_nit;   // 8,16,32,64b :: 2,2,4,4 B
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

#ifndef __clang__
#pragma GCC diagnostic ignored            "-Wstringop-overflow" // _/\_
#else
#pragma clang diagnostic ignored          "-Wtypedef-redefinition"
#endif

#ifndef MSTRCT_PRINT_FMT
  #define MSTRCT_PRINT_FMT                "M_%s/%s/%d\n"
#endif

#ifndef MSTRCT_TNO
  #define MSTRCT_TNO 0
#endif

typedef typeof(__builtin_choose_expr(sizeof(mstrct_unit) > 2, (unsigned short)0, (unsigned char)0)) mstrct_uhalf;
typedef typeof(__builtin_choose_expr(sizeof(mstrct_unit) > 2, (unsigned long long)0, (unsigned long)0)) mstrct_utwice;
typedef union {mstrct_uhalf _mstrct_dest; mstrct_uhalf _mstrct_id;} mstrct_pass;
typedef union {struct {mstrct_uhalf _mstrct_dest; mstrct_uhalf _mstrct_src; mstrct_unit _mstrct_id;};
               mstrct_utwice _mstrct_uni;} mstrct_pack;

#define MSTRCT_SHIFT (8*(sizeof(mstrct_utwice) - sizeof(mstrct_unit) - sizeof(mstrct_uhalf) * MSTRCT_ARG_COUNT(MSTRCT_MCU)))
#define MSTRCT_SIZE(word) ((((mstrct_utwice)(word)) << MSTRCT_SHIFT) >> MSTRCT_SHIFT)

__attribute__((common)) char mstrcterrno[MSTRCT_TNO + 1]; __attribute__((common)) mstrct_pack mstrctbox[MSTRCT_TNO + 1];
__attribute__((common)) mstrct_unit mstrctx[MSTRCT_TNO + 1], mstrcty[MSTRCT_TNO + 1];
__attribute__((common)) mstrct_usize *mstrctfixed[MSTRCT_TNO + 1]; static mstrct_usize **restrict mstrct_fixed = mstrctfixed;

_Static_assert(sizeof(void(*)(void)) == sizeof(void*), "M_ERR: code & data ptrs must be same size!"); // no harvard

__attribute__((alloc_size(1), noinline, unused, const)) static char*
mstrct_base(mstrct_unit siz, mstrct_unit offset, char var, mstrct_uhalf tid) {
  (void)siz; (void)var; return (char *)(*((mstrct_fixed[tid]) + offset));
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

// memstruct; see doc
#define MSTRCT_T(type, index, line, key) struct {  \
  MSTRCT_CAT3(MSTRCT_TYP_, MSTRCT_ARG_COUNT(MSTRCT_LONG), MSTRCT_ARG_COUNT(key))(type, index) i; \
  mstrct_unit _id;   \
  /* typ[0] */ typeof(type) typ[0] __attribute__((packed)); \
  /* con[0] */ struct {char a[((MSTRCT_CON(type)) ? ((__builtin_constant_p(sizeof(char index))) ? 1 : 0) : 0)];} con[0];   \
  /* lin[0] */ struct {char a[line];} lin[0];   \
  /* dim[0] */ struct {char b; char a[] index [1];} dim[0];   \
}

#define MSTRCT_CON(type)                  (__builtin_types_compatible_p(typeof(type) const *, type *))
#define MSTRCT_TYP_00(type, index)        typeof((struct {}){})
#define MSTRCT_TYP_01(type, index)        typeof((MSTRCT_CON(type)) ? (mstrct_size const)0 : (mstrct_size)0)
#define MSTRCT_TYP_10(type, index)        typeof((struct {}){})
#define MSTRCT_TYP_11(type, index)        typeof((MSTRCT_CON(type)) ? (mstrct_nit const)0 : (mstrct_nit)0)

struct mstrct_arc {}; typedef struct mstrct_arc mstrct_arc;
#define MSTRCT_ARC   \
(__builtin_choose_expr(__builtin_types_compatible_p(typeof((mstrct_arc *)0), typeof((struct mstrct_arc *)0)), 0, MSTRCT_TID + 1))

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
  {return index;} else {return mstrct_bounds_error(MSTRCT_HID(id), line, tid);}
}

// utility
#define MSTRCT_ID(ptr)                    ((*(mstrct_pack *)ptr)->_mstrct_id)
#define MSTRCT_CID(ptr)                   ((*(mstrct_pack *)ptr)->_mstrct_src)
#define MSTRCT_STEP                       (MSTRCT_CHK2 ? (1 + sizeof(mstrct_utwice) / sizeof(mstrct_usize)) : 2)
#define MSTRCT_TSIZ(name)                 ((mstrct_unit)sizeof(*(name.typ[0])))
#define MSTRCT_ASIZ(name, range)          ((range) * (mstrct_usize)sizeof(*(name.dim[0].a)))
#define MSTRCT_BSIZ(name, range)          (MSTRCT_TSIZ(name) * MSTRCT_ASIZ(name, range))
#define MSTRCT_FLAT(name,idx)             ((mstrct_size)(&(*(typeof(name.dim[0].a) *)0) idx [0]) + \
                                          __builtin_choose_expr(sizeof(name.i), (name.i), (0)))

#define MSTRCT_PRAG0                      _Pragma("GCC diagnostic ignored \"-Warray-bounds\"")
#define MSTRCT_PRAG1                      _Pragma("GCC diagnostic warning \"-Warray-bounds\"")
#define MSTRCT_PRAG2                      _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wattributes\"")   \
                                          _Pragma("GCC diagnostic ignored \"-Wunused-local-typedefs\"")
#define MSTRCT_PRAG3                      _Pragma("GCC diagnostic pop")

#define MSTRCT_CLEAN(cnt)  \
struct mstrct_arc; MSTRCT_PRAG2 typeof(__builtin_choose_expr(MSTRCT_ARC, (mstrct_pack){}, (mstrct_pass){}))   \
MSTRCT_CAT2(mstrct_clean_, cnt) __attribute__((cleanup(mstrct_set))) = {._mstrct_id = mstrcty[MSTRCT_TID]}; \
MSTRCT_CAT2(mstrct_clean_, cnt)._mstrct_dest = MSTRCT_ARC; typedef struct mstrct_arc mstrct_arc; MSTRCT_PRAG3

// GET

#define MSTRCT_GET_A0(ptr,typ,i) \
({MSTRCT_CAT3(MSTRCT_GET_, MSTRCT_FIRST i, MSTRCT_CHK1) (MSTRCT_ID(ptr), i, (typ *), sizeof(typ), MSTRCT_CID(ptr), __LINE__, 0);})

#define MSTRCT_GET_A1(name, n) ({mstrct_uhalf mstrct_temp = (mstrct_uhalf)(n); \
if (mstrct_temp >= MSTRCT_TNO) {mstrct_error("TID_OVF",6,__LINE__,MSTRCT_TID); mstrct_temp = 0;};  \
mstrctbox[1 + mstrct_temp] = {._mstrct_dest = 1 + mstrct_temp, ._mstrct_src = MSTRCT_TID, ._mstrct_id = name._id}; \
(void *)&mstrctbox[1 + mstrct_temp];})

#define MSTRCT_GET_A2(ptr,typ)   \
(mstrct_span(sizeof(typ), MSTRCT_ID(ptr), mstrct_reset(MSTRCT_ID(ptr), MSTRCT_CID(ptr)), MSTRCT_CID(ptr)))

#define MSTRCT_GET_A3(ptr,typ)   \
mstrct_base(sizeof(typ), MSTRCT_ID(ptr), mstrct_reset(MSTRCT_ID(ptr), MSTRCT_CID(ptr)), MSTRCT_CID(ptr))

#define MSTRCT_GET_A4(ptr,typ)   (mstrct_byte(MSTRCT_ID(ptr), MSTRCT_CID(ptr)))

#define MSTRCT_GET_B0(name,i)    MSTRCT_GET_B5(name, i)

#define MSTRCT_GET_B1(name)      (*({asm volatile (" " : "+m" (*(mstrct_fixed[MSTRCT_TID] + name._id + 1))); \
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
  __builtin_memset(&name, 0, sizeof(name)); char *ptr = (char *)(alloc); name._id = mstrct_alloc(MSTRCT_TID, 0); \
  if (ptr == NULL || ptr == ((void *) -1)) {mstrct_error("ALLOC_FAIL", 3, __LINE__, MSTRCT_TID);}   \
  mstrct_put(ptr, name._id, MSTRCT_BSIZ(name, range), MSTRCT_TID, (MSTRCT_CHK2 ? __LINE__ : 0)); \
} while(0)

#define MSTRCT_LET_D1(store, name, range, cnt)  \
store typeof(*(name.typ[0])) MSTRCT_CAT2(mstrct__, cnt)[MSTRCT_ASIZ(name, range)]; MSTRCT_CLEAN(cnt);  \
if (sizeof(MSTRCT_CAT2(mstrct__, cnt))) {  \
  __builtin_memset(&name, 0, sizeof(name)); name._id = mstrct_alloc(MSTRCT_TID, (MSTRCT_AUTO(store)));  \
  mstrct_put(MSTRCT_CAT2(mstrct__, cnt), name._id, MSTRCT_BSIZ(name, range), MSTRCT_TID, 0); \
}

#define MSTRCT_LET_D2(store, name, ini, cnt) store typeof(*(name.typ[0]))  \
MSTRCT_CAT2(mstrct__, cnt)[MSTRCT_ASIZ(name, MSTRCT_ARG_COUNT ini)] = MSTRCT_LIST(MSTRCT_EXPAND ini); MSTRCT_CLEAN(cnt); \
if (sizeof(MSTRCT_CAT2(mstrct__, cnt))) { \
  _Static_assert(!sizeof(name.i), "M_ERR: " #name " must not have static index as cardinality derives from initializer list!");  \
  __builtin_memset(&name, 0, sizeof(name)); name._id = mstrct_alloc(MSTRCT_TID, (MSTRCT_AUTO(store)));  \
  mstrct_put(MSTRCT_CAT2(mstrct__, cnt), name._id, MSTRCT_BSIZ(name, MSTRCT_ARG_COUNT ini), MSTRCT_TID, 0); \
}

#define MSTRCT_LET_D3(re_alloc, name, range) do {   \
  char *ptr = (char *)(re_alloc); if (ptr == NULL || ptr == ((void *) -1)) {mstrct_error("ALLOC_FAIL",3,__LINE__,MSTRCT_TID);} \
  mstrct_put(ptr, name._id, MSTRCT_BSIZ(name, range), MSTRCT_TID, (MSTRCT_CHK2 ? __LINE__ : 0)); \
} while(0)

#define MSTRCT_LET_D4(allo_ca, name, range) do {   \
  char *ptr = (char *)(allo_ca); __builtin_memset(&name, 0, sizeof(name)); \
  if (ptr == NULL || ptr == ((void *) -1)) {mstrct_error("ALLOC_FAIL", 3, __LINE__, MSTRCT_TID);}  \
  name._id = mstrct_alloc(MSTRCT_TID, 1); mstrct_put(ptr, name._id, MSTRCT_BSIZ(name, range), MSTRCT_TID, 0); \
} while(0)

#define MSTRCT_LET_E0(ptr) mstrct_uhalf mstrct_tid = (*(mstrct_pack *)ptr)->_mstrct_dest;  \
ptr = mstrct_addr((*(mstrct_pack *)ptr)->_mstrct_id, (*(mstrct_pack *)ptr)->_mstrct_src)

#define MSTRCT_DEL(de_alloc, name) do {__builtin_choose_expr((sizeof(de_alloc) == 1),   \
  (mstrct_dealloc_0(de_alloc, (name._id), MSTRCT_TID)), (mstrct_dealloc_1(de_alloc, (name._id), __LINE__, MSTRCT_TID))); \
  *(mstrct_utwice *)(mstrct_fixed[MSTRCT_TID] + name._id + 1) = 0;  \
  *(mstrct_fixed[MSTRCT_TID] + name._id) = (mstrct_usize)(mstrct_fixed[MSTRCT_TID]); \
  asm volatile (" " : "+m" (*(mstrct_fixed[MSTRCT_TID] + name._id + 1)));   \
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
mstrct_put(void *arr, mstrct_unit name_id, mstrct_usize size, mstrct_uhalf tid, mstrct_unit hash) {
  *(void **)((mstrct_fixed[tid]) + name_id) = arr;
  if (hash) {
    *(typeof(__builtin_choose_expr(MSTRCT_STEP == 2, (mstrct_usize *)0, (mstrct_utwice *)0)))(mstrct_fixed[tid] + name_id + 1) =
    (mstrct_utwice)size | ((mstrct_utwice)(mstrct_uhalf)hash << (8 * sizeof(mstrct_utwice) - MSTRCT_SHIFT));
  } else *(mstrct_fixed[tid] + name_id + 1) = size;
}

static inline mstrct_unit mstrct_alloc(mstrct_uhalf tid, char stack) {
  if ((MSTRCT_ARG_COUNT(MSTRCT_SOFT) == 0) && (mstrctx[tid] & 1023) == 0) {MSTRCT_PRINT(MSTRCT_PRINT_FMT,"","ID",mstrctx[tid]);}
  mstrct_unit ret = 0; if (__builtin_expect(mstrctx[tid] >= mstrcty[tid], 0)) {mstrct_error("META_OVF", 5, 0, tid);}
  if (!stack) {ret = mstrctx[tid]; mstrctx[tid] += MSTRCT_STEP;}
  else {mstrcty[tid] -= MSTRCT_STEP; ret = mstrcty[tid];}
  return ret;
}

static inline void
mstrct_set(void *ptr) {
  if (*(mstrct_uhalf *)ptr) {
    mstrct_pack p = *(mstrct_pack *)ptr; mstrct_uhalf tid = p._mstrct_dest - 1;
    for (mstrct_unit j = mstrcty[tid]; j <= p._mstrct_id; j += 2) {
      *(mstrctfixed[tid] + j + 1) = 0; *(mstrctfixed[tid] + j) = (mstrct_usize)(mstrct_fixed[tid]);
      asm volatile (" " : "+m" (*(mstrct_fixed[tid] + j + 1)));
    }
    mstrcty[tid] = p._mstrct_id;
  }
}

__attribute__((constructor)) static inline void
mstrct_init(void) {
  if (mstrctfixed[0] == 0) {void *space;
    for (mstrct_uhalf i = 0; i <= MSTRCT_TNO; i++) {
      if ((mstrctbox[i])._mstrct_uni == 0) (mstrctbox[i])._mstrct_uni = MSTRCT_BLOCK;
      space = MSTRCT_ALLOC((mstrctbox[i])._mstrct_uni);
      if (space == NULL) {mstrct_error("ALLOC_FAIL", 3, 0, MSTRCT_TID); __builtin_trap();}
      mstrct_fixed[i] = space; mstrctx[i] = 2; *(mstrctfixed[i] + 1) = (mstrct_usize)((mstrctbox[i])._mstrct_uni);
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


#endif
