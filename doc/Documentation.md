
# ![memstruct banner](../banner.svg) memstruct DOCUMENT

This document explains how to configure and use the memstruct library.

## Table of contents

- [Overview](#overview)  
- [Features and design](#features-and-design)  
- [Configuration](#configuration)  
- [Usage](#usage)  
- [API reference](#api-reference)  
- [Troubleshooting](#troubleshooting)  
- [Contributing guidelines](#contributing-guidelines)

## Overview

- **Working:** the 'safe ptr' (henceforth called memstruct) carries rich compile-time data in its type system suited to compiler optimizations facilitating either fully compile-time or heavily elided / auto-hoisted / pipelined checks. also, UAF & `NULL` checks are folded within OOB check. memstruct provides thread safe (locks/atomics free, single-write & read-share) semantics to make safe multi-threading feasible in bare metal while also complementing external libraries (pthread etc) on 64-bit builds.

- **API:** `m/M` macro, with 1 symbol overload, provides the unified API -- including access to metadata stored in a thread-safe custom heap arena. `m/M` effectively eliminates the usage of `[/]` in safe code so there is no language level abstraction overhead.

- **Philosophy:** safety and optimization escalate together. with each O level more static analysis kicks in and the safety and performance guarantees become stronger. also, 'pay what you use' to unlock extra features e.g. locks, ptr-arithmetic, finer diagnosis, etc.

## Features and design

- Flags: opt-out, hardening, and MCU. Literals: metadata size, and max threads.

- Supports: 8-64 bit CPUs | gcc, clang : GNU C99 & ONWS | custom allocators & de-allocators.

- Single‑header: no separate `.c` file needed. no external dependencies. MCU support.

- Guard-rails: non-idiomatic usage and puns warned through linter (gcc & clang) and compiler (gcc only).

- Works across TUs: int `m(foo,auto)` (memory ID) passed around to share memory.

- Thread safety: inherently thread safe using exclusive-writer / shared-read framework (no locks or atomics). this is more than sufficient, but if locks / atomics are really needed, use external libraries (e.g. pthread) and protect critical section as usual.

- Health monitoring: except for on-stack memories, memory IDs aren't recycled. an unboundedly increasing ID score (printed every 1024 allocs in SOFT mode) is likely indicative of poor temporal layout in a program (=> heap fragmentation, etc).

## Limitations

- doesn't support Harvard architecture. and, MSVC, Big Endian.

- no GC: every allocation must have a de-allocation (dup-de-allocations -> no-op though).

- multi-threading model is lock-free. if locks are a requirement include relevant multi-threading library.

## Configuration

- In source, optionally include `#define NMSTRCT` to disable checks. disable locally like so: `#define NMSTRCT` `unsafe code here` `#undef NMSTRCT`.

- Include `#define MSTRCT_SOFT` or `#define MSTRCT_HARD` to choose custom hardening level of error reporting.
    ```
    [ default ]   : print detailed err (with line_no of memsruct genesis site), continue with default "safe", handle mstrct_errno.
    MSTRCT_SOFT   : print detailed err (with line_no of error_site), continue with default "safe", handle mstrct_errno.
    MSTRCT_HARD   : print "BAD", segfault at the error site (get line_no from crash-analysis).
    ```

- Include MCU flag: `#define MSTRCT_MCU` for MCU programming.

- Define literal `MSTRCT_TNO` (number of maximum simultaneous threads over and above main; default: 0) if required. if locks/atomics etc are needed, `#define MSTRCTM` to enable read-write share.

- Define literal `MSTRCT_BLOCK` (reference metadata size in bytes) if required. or, define thread/core-specific `mstrctblock[thread_ID]`. 

- Include `mstrct.h`.

- Alternatively, instead of directives in the source, use flags `-D[FLAG]` directly in makefile.

## Usage

- **Working theory**: a memory array `foo[ I ][ J ][ K ]..` is constructed of two components-

    a) the static part `foo[ ][ J ][ K ]..` that is with constant indexes and part of the memstruct type system, and

    b) the dynamic part `foo[ I ][ ][ ]..` where the single index (may or may not be literal) i is dynamically allocated.

    a standalone memstruct is declared as `m(name, (J,K..), type)` where `(J,K,..)` is a typical multi-dim static index. dynamic index `I` is implicitly calculated from total allocation size later as e.g. `I=12` in `M(name, malloc, 48)`.

    for most purposes, the array is a simple 1-D array, so declare memstruct as e.g. `m(name, 1, type)`, then allocate as e.g. `M(name, calloc, 48)`, and access as `m(foo,i)`.

    for non-array types, declare the memstruct as e.g. `M(name, 1, struct alpha)`, then allocate as e.g. `M(name, malloc, 4)`, and access as `m(foo)`.

    for on-stack & global memories, memstruct is declared and allocated in a single statement as: `m(name, 12, int, auto)`, `m(name, 12, char, static __thread)` etc. 

- **Memory sharing:** a int sized metadata ID `m(foo,auto)` is simply passed around. one may also share base_addr & span as `&m(base)` & `m(foo,_)` directly. in multithreading, share the ID as: `m(foo,auto,TID)` where `TID` is a `short` equal to the thread id of the receiver thread.
    ```
    m(bar,auto) = m(foo,auto); // makes bar safely refer the same memory as foo, but retain its type alias

    callee_function(m(foo,auto), other_args); // callee is given foo.id safely to do whatever (single thread)

    pthread_create(&m(threads,i), NULL, native_thread, m(foo, auto, i)); // a pthread example of multithreaded sharing
    ```
- **Safe access of data:** 

    `m(foo,index)` is equivalent to `foo[index]`. or `m(foo,i,j,k..)` to `foo[ i ][ j ][ k ]..`

    Read / write memory: `m(name,index) = value`.
    ```
    // single dim array types
    m(foo,5) = 10;

    // multi-dim array types
    m(bar,(5,7,2)) = 10;
     ```
- **Raw access (w/o checks) of data:** 

    `(&m(foo,0))[i]` is an example of attempted raw access of data: such puns are warned at compile-time. memstruct-returned addresses carry dummy alloc_size compile-time metadata to deny raw memory access.

    raw access for legitimate reasons is, as discussed before, e.g. `#define NMSTRCT` `unsafe code here` `#undef NMSTRCT` for disabling safety locally.

- **Pointer arithmetic:**

    not ptr arithmetic per se, but the (flat) array index is accessible as `foo.i` as L-value and can be incremented / decremented / set to a value. notice the () around the static index to force the index feature into existence.
     ```
    m(foo, (1), int);               // parentheses force index feature in foo
    foo.i++;                        // increment array index
     ```
- **memstruct declaration:** declare a memstruct foo as `m(foo, static_index, data_type)`. if the memory is non-heap, declare + allocate as `m(foo, range, data_type, storage)`. if the index (or range) is enclosed with (), foo.i as index is added.
    ```
    // int[][1]
    m(foo, 1, long int);            // or: M(long int *,foo,,1)
    m(foo, (1), long int) = {0};    // declare & make foo.id = foo.i = 0

    // char[][2][5]
    M(foo, (2,5), char);

    // valid as struct field and other nested types
    ```
- **Allocate / re-allocate:** heap memory to a memstruct `M(foo, your_allocator_or_reallocator, args...)`. sanity checks (e.g. `if (ptr == NULL)`) are done internally, and need not be repeated by user.
    ```
    /* memstruct supports custom allocators / re-allocators (check API section below) */

    M(foo, malloc, 80);

    M(foo, realloc, &m(foo), 60)

    M(bar, mremap, &m(bar), 48, 44, MREMAP_MAYMOVE)
    ```
- **Metadata** access: `m(metadata foo)`.
     ```
    char *temp = &m(base);          // base addr

    int temp = m(foo,_);            // index span

    int temp = m(foo,auto);         // memory ID

    foo_type temp = m(foo);         // same as m(foo, 0)
     ```
- **De**-allocate: double de-allocs are redundant (later elided by compiler). custom de-allocators supported. de-allocation failure check is done internally, and need not be repeated by user.
     ```
    M(foo, free);                   // on-heap memory

    M(foo, munmap);                 // mmapped memory
     ```
    during de-allocation, size is `NULL`-ed (and base addr changed to a safe zone) so that double frees become redundant. NOTE: the user knows the best when to free a memory, but in complex CFGs - or when in doubt - it's advisable to over-use, rather than not, the de-allocators, as redundant frees get anyways **elided by the compiler**, rather than corrupt memory.

- **Loop optimization**: in general, at >O0 memstruct hoists OOB checks and at worst only a (pipelined) cmp op remains for later checks. to strictly force total elision in loops, e.g., change the syntax in `for (int i = 0; i < 50; i++)` to `for (int i = 0; i < m(foo,_); i++)` where `m(foo,_)` = index_span_size, and expression `i < m(foo,_)` is the strictest OOB check (resulting in elision of within-the-loop checks). further, `m(foo,_)` is evaluated only once as it calls a `const` attribute function. 

- **MCU implementation**: a) define `MSTRCT_MCU`. b) define `MSTRCT_PRINT()` and `MSTRCT_ALLOC(bytes, i)` macros in your code; leverage `i` in `MSTRCT_ALLOC` to define custom memory areas in .ld for each metadata block, if needed. c) refer `mstrct.h` to match API of these macros. d) make use of memstruct's locks/atomics free multi-threading as discussed below.

- **Multithreading:** to share memory use `m(foo, auto, TID)` instead of `m(foo, auto)` used in single-thread contexts. here, `TID` is a `short` representing the destination thread ID (see example 8). `m(foo, auto, TID)` is a ptr which on the thread side is utilized as `m(ptr, i, array_type, _)` for accessing the shared memory as read-only. this supports the single-writer + shared-read framework. additionally, in case read-write access to a shared memory is needed, use `#define MSTRCTM` to unlock the syntax `m(ptr, i, array_type, do)` enabling read-write access to the shared memory. note: wrapping such code with mutextes/locks etc is user's resposobility as memstruct's thread safety guarantee is upto memstruct's thread safety model only.

## API reference

- `M()`/`m()` **macro:**
    ```
    // GET errno:
    M():
        generates the current error number (thread safe).
        list (err strings and errnos):
          "BAD" (ILLEGAL_ACCESS) ........................ 1
          "OOB" (BOUNDS_CHECK_FAIL) ..................... 2
          "ALLOC_FAIL" .................................. 3
          "DE_ALLOC_FAIL" ............................... 4
          "META_OVF" (METADATA_OVERFLOW)................. 5
          "TID_OVF" (THREAD_ID_OVERFLOW)................. 6
         

    // THREAD ID declaration for the current thread:
    M(ptr):
        ptr is a void * received from the parent thread.
    note:
        a) the ptr carries the coded ID that's duly instated by M(ptr) as thread ID,
            referenced by the succeeding thread body.
        b) this can only be used once in a thread, else duplicated declaration error ensues.
            use m() to reset the value (see m() API ref).


    // DE-ALLOCATION
    M(foo, de_allocator):
        foo = memstruct name
        de_allocator = name of the de-allocator, e.g. free, munmap, etc.
    note:
        a) any custom de-allocator matching the API of either free(...) or munmap(...) is supported
        b) the operation is idempotent: multiple de-allocations are redundant.
        c) the macro performs sanity check; user doesn't need to do it. 


    // ALLOCATION/ RE-ALLOCATION on-heap: 
    M(foo, allocator_or_reallocator, args...):
        foo = memstruct name
    note:
        a) the macro performs sanity check; user doesn't need to do it. 
        b) custom allocators and re-allocators are supported. the only criterion is
            that an allocator must take 1, 3 or 6 args whereas a re-allocator must 2, 4 or 5.
        c) calloc version that takes 1 arg is therefore allowed, not the one that takes 2.


    // DECLARATION & ALLOCATION on stack, static, or global segment:
    m(foo, range, data_type, storage_class):
        foo = memstruct name
        range = numbers of elements 
        data_type = type of elements
        storage (keyword) = static / static __thread / auto / left_empty_for_global
    note:
        a) this syntax both declares and allocates foo.
        b) to declare and allocate static memory at function scope, use the following syntax:
            static m(f00, range, data_type, auto).
        c) static memories at global scope, however, follow the standard template:
            m(foo, range, data_type, static).
        d) the resulting memstruct isn't standalone: the memory is one of its fields. the rest
            of memstruct behavior, and the API, remains same.


    // GET cross-thread data:
    m(ptr, index, array_type, access_type):
        ptr = ptr received from source thread (through m(foo,auto,TID) syntax; see API ref)
        array_type = type of array to interpret data with, e.g. int[] for an array of ints
        access_type (keyword) = _ / do
    note:
        a) use "_" for read-only access, "do" for read-write access.
        b) before read-write access enable the same with: #define MSTRCTM.
 

    // DECLARATION of standalone memstructs: 
    m(foo, static_index, data_type):
        foo = memstruct name
        static_index = staic index in the format of plain number or a set of no's in () e.g. (2,4,3) 
        data_type = type of elements, e.g. int etc
    note:
        a) the static index can be multi-dimensional, but the corresponding
            dynamic index is a single number implicitly deduced from the total
            size divided by the static size.
        b) usually arrays are 1-D. in such cases let the static index be 1.
        c) the type qualifiers e.g. *const, *restrict, *atomic and *volatile
            have been deemed unnecessary and not considered in the API. the
            reason is: a memstruct isn't a named access to a memory and the
            accesses are already direct and optimized.


    // SHARE cross-thread data: 
    m(foo, auto, TID):
        foo = memstruct name whose memory is being shared
        auto = keyword 
        TID = short-int sized thread ID with which data is being shared
    note:
        a) this applies to multi-threaded sharing, for single-threaded see m(foo,auto) API.
 

    // GET span as R-value
    m(foo, _):
        _ = metadata keyword for span
        foo = memstruct name
        returns: a ptr sized signed number


    // SHARE ID as L-value
    m(foo, auto):
        auto = metadata keyword for memory ID
        foo = memstruct name
        returns: an short-int sized unsigned number
    note:
        a) this applies to single-threaded sharing, for multithreaded see m(foo,auto,TID) API.


    // GET 1-D data, as L-value
    m(foo, i): 
        foo = memstruct name
        i = sole index


    // GET multi-dim data, as L-value
    m(foo, (i, j, k,...)): 
        foo = memstruct name
        i, j, k,... = dynamic and/or static indexes


    // GET the first element as L value:
    m(foo):
        foo = memstruct name
    note: this is same as m(foo,0).


    // GET the current thread ID:
    m():
        result: a short int.
    note:
        a) this is an L-value, so whereas the main thread ID is unchangeable, m()
            can be used to change other threads' IDs.
        b) setting a thread's ID is best done with M(ptr), see its API ref.


    ```

- **memstruct:**
    ```
    // memstruct layout

    A.  standalone memstructs (do not carry the memory as field):

        union {
            typeof(__builtin_choose_expr(key, (mstrct_utwice)0, (mstrct_unit)0)) _ID;
            struct {
                mstrct_unit _id;
                typeof(__builtin_choose_expr(key, (MSTRCT_UNIT)0, (struct {}){})) i;
                typeof(type) * const typ[0] __attribute__((packed)); \
                struct {char a[line];} lin[0];   \
                struct {char b; typeof(type) a[] index [1];} dim[0];   \
            };
        }

    B.  integral memstructs (carry memory as a field):

        union {
            typeof(__builtin_choose_expr(key, (mstrct_utwice)0, (mstrct_unit)0)) _ID;
            struct {
                mstrct_unit _id;
                typeof(__builtin_choose_expr(key, (MSTRCT_UNIT)0, (struct {}){})) i;
                typeof(type) * const typ[0] __attribute__((packed)); \
                struct {char a[line];} lin[0];   \
                struct {typeof(type) a index [1];} dim[1];  \
            };
        }

    // field description:

        foo.ID: union field used in clobbering foo.id & foo.i together during ID sharing
        foo.id: metadata ID
        foo.i: current array index (suports index arithmetic)
        typeof(foo.typ[0]): pointer type
        sizeof(foo.lin[0]): line number where foo was declared
        typeof(dim[0].a): holds geometry of - static indexes [index] and dynamic index []

    // GET current index
    foo.i 
        foo = memstruct name
        i = field in foo containiing current int index
    note:
        a) to extend i to be long sized, redefine MSTRCT_UNIT locally as mstrct_size.

    // GET metadata ID
    foo._id
        foo = memstruct name
        _id = field in foo containiing metadata uint ID
    note:
        a) accessing _id is discouraged. use m(foo,auto) instead.

    ```

##  Troubleshooting

- I disabled checks with `#define NMSTRCT` but the metadata is still getting stored in the heap arena

    feature, not bug: the metadata layer is a trade-off against performance + memory-safety that memstruct made in design to make a program provably memory safe yet on par with raw C in speed and flexibility.

- Memstruct is catching all the bugs but the program isn't panicking

    this is a feature at hardening level 0 (default): after generating the error message the program continues with default "safe" values. you may set the hardening level to HARD (`#define MSTRCT_HARD`) to cause segfault at the site after error print. 

    at the default level, the line number of the declaration site of the erring memstruct (not the erring site itself) is printed. to print the line number of the erring site set the level to SOFT (`#define MSTRCT_SOFT`). HARD, on the other hand, prints 0 for the line number (and `BAD` irrespective of error) but since it segfaults at the error site, the line number can be recovered in post-analysis. HARD generates the least binary footprint (for the unhappy path), followed by the default level and then SOFT (which is well suited for development phase). the default level is the sweet spot, and caters to fail-safe design (no panics; + has the thread-safe errno `M()` to handle error if needed).

- How to check what `m()` and `M()` macro abstractions are expanding into?
    
    the most convenient method is to expand the macro locally in your code editor itself. currently, clangd LSP works fine. or, more conventionally, pre-compile with -E flag into expanded source.

- Memstruct is good at denying raw accesses to its own memories. but how to know (quickly) if raw accesses have been used for memories unrelated to memstruct in a project?

    search `[` or `]` in your editor to quickly find if `[]`-idiom is used. in fact, `m()` & `M()` symbols are meant to eliminate `[` & `]`, and therefore in a memstruct conforming file no `[]` should be found. further, raw dereferences directly on memstructs, or punning, generates compile time error (gcc only).

- Under which scenarios safety can be by-passed (via flags)?
    
    a) (rare; profile first) edge cases where raw accesses, as compared to spatially safe accesses, are gainfully faster. b) (low level) large number of sub-allocations that must suppress temporal safety to avoid individual de-allocs. c) (design) temporal safety suppression to let OS reclaim the memory. d) (compile flag) program wide safety supression in production release (rare, too).

- Can memstruct work together with legacy code?    

    while the criterion regarding which legacy codes are allowed to work with memstruct is very liberal, it is still very precise:

    "any legacy or 3rd party code that is not an allocator / de-allocator / re-allocator but still modifies the size or base address of a *shared* memory -- **is considered unsafe**."

    with the above criterion accounted for, empirically proven safety of legacy C code is acknowledged and no re-writes are necessary (simply share `&m(foo)` & `m(foo,_)`). however, if one were authoring a `C` library today, one may use `m(foo,auto)` to safely share memory while intending to modify base address and size.

    also note that `&m(base)` is constrained by the library to only allow access to one element (existing at the address), and naive sharing with legacy code will generate compile or runtime fault. therefore, share it with indirection: `(void *)&m(foo)`.

    in summary:

    a) large code bases can be furthered with memstruct, or b) greenfield projects with memstruct can work with legacy code.

    there is no contradiction: memory safety is like the *fabric* of space-time: in empirically proven safe C code, the programmer held the fabric together; going forward, memstruct does it on the behalf of programmer.

- How to allocate memory with spatial checks enabled but temporal checks disabled?

    e.g. in arena allocation one may want spatial safety for sub-allocations but not temporal safety as single de-allocation covers whole arena. so, wrap each sub-allocation with e.g. `#define NMSTRCT` and `#undef NMSTRCT` or even better devise a dummy de-allocator for sub-allocation (see test #9).

- Does memstruct resuse memory IDs or is it just monotonically increasing?
    
    memstruct reuses IDs for on-stack memories; for heap, static and global located memories, fresh IDs are issued. as discussed before, every 1024th ID is printed so that an ever increasing ID count tells upon unsafe program design.

- When is the LTS release?

    one may use the latest memstruct.h (always passes the test suite!) directly. LTS will soon follow, pending sufficient test coverage.


    NOTE: as memstruct is being developed while also simultaneously used in an ongoing project, LTS for memstruct project itself is guaranteed.

- I found what seems to be a bug/deficiency in memstruct

    feel encouraged to raise an issue in the repo, and if possible a merge request as contributor!

- I need to use the library for a closed source work at our company

    one may contact the author for separate licensing, or, consider sponsoring.

## Contributing guidelines

### How to help

- Fix bugs, typos, or improve tests.
- Clarify or expand documentation.
- Keep changes small and focused; for big changes, open an issue first to discuss.

### Code style

- Follow the existing C style (indentation, braces, naming).
- Keep pull requests small and easy to review.

### Pull request workflow

1. Fork the repo and create a branch:
   ```bash
   git checkout -b fix/foo-bug
   ```
2. Issues:
Search existing issues before opening a new one.

3. When reporting bugs, include commands, versions, and a minimal example.

4. Vibe coding: good luck.

5. License:
By contributing, you agree your code will be distributed under the project’s license (see  [LICENSE](../LICENSE)).

![memstruct banner](../banner.svg)
