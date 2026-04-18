
# ![memstruct banner](banner.svg) memstruct DOCUMENT

This document explains how to configure and use the memstruct.h library.

## Table of contents

- [Overview](#overview)  
- [Features and design](#features-and-design)  
- [Configuration](#configuration)  
- [Usage](#usage)  
- [API reference](#api-reference)  
- [Troubleshooting](#troubleshooting)  
- [Contributing guidelines](#contributing)

## Overview

- The core working principle is to have the 'safe ptr' maximally carry compile time information in its type system. The error reporting system then supplements this with constant foldings from compiler optimizer. This results in either compile time or heavily optimized runtime checks. For runtime checks that cannot be elided, metadata is stored in a fast access, cache friendly, static data segment. UAF & NULL checks are part of OOB check in a way as to incur no extra overhead.

- A 'safe ptr' is basically a unique, anonymous struct type on the 'outside' but also the size of a plain int (like a memory-ID), casually passed around among stakeholders.

- `$` macro, with one symbol overload, provides the unified API.   

## Features and design

- Compile time checks & reporting for static and constant folded data.
- Single‑header; no separate `.c` file needed.
- Supports on-heap, on-stack, and custom allocators.
- No external dependencies (only standard `C` headers).
- Safe to include in multiple translation units.

- Since a 'safe ptr' is a unique anonymous struct type, it doesn't mix with other types, including other safe ptrs; it can't be naively de-referenced, or cast either. It takes deliberate `C` gymnastics, other than the `$(name)` escape hatch API, to break the safety net.
- Thread safety relies on target `x86` h/w-level atomicity and cache coherency through `MESI`, enforced with strict `ASM qword` alignment and `ASM "=m"` constraint, respectively.
- Logical concurrency for strict causal orderings is implemented by the user, and is orthogonal (justifiably, in the eyes of the author) to this library's workings.

## Configuration

- Include `mstrct.h`.
- Define `NMSTRCT` to disable all checks for production, if needed.
- Use `MSTRCT_L` flag for choosing hardening level of error reporting.
```
    default 0: print detailed err, continue with default "the arr end value"
    strict  1: print detailed err, halt program with segfault at error site
    hard    2: print detailed err, exit program with mstrct_status code
```
## Usage

- **Memory sharing:** memstruct field id is simply passed around to share memory. 
```
    bar.id = foo.id; // makes bar refer the same memory as foo, but retain its type "view"
    callee_function(int id, other_inputs); // callee is given int foo.id to access memory, metadata
```
- **Safe access of data:** `$(foo, index)` is equivalent to `foo[index]` but with memory checks (as needed!) inlined.

- **Raw access (w/o checks) of data:** through meta data type `$(foo)` (see the metadata API reference).

- **memstruct declaration:** declare a "safe ptr" foo as `$(ptr_type, foo, range, addr)`. If foo is already declared as a safe ptr, then call `$( , foo, range, addr)` for reassign.
    ```
    // on-heap arr of 10 longs
    $(long int *, foo, 10, malloc(40));

    // on-stack multi-dim arr of range 10
    $(int * const, bar[2][3], 10, (int [60]){0});

    // declare safe ptr w/o defining; could be inside struct type declaration
    $(float *, cux, );

    // re-assign memory `$(,name, range, addr)`:
    $(, var, 16, (int [16]){0});
    ```
    Here, the input fields hold the following relationship:
    ```
    sizeof(*ptr_type) x (name_cardinality) x (range) = (total_allocated_size_in_bytes)
    ```
    This relation is enforced by the library (at compile time, where possible!).

- In `$(ptr_type, foo, range, addr)`, addr is not necessarily an allocator but could also be an address pointing to a memory. Such usage does not pose memory hazard by its own, and as such is left to the occasional user to suit their program logic.

- Under the macro `$(ptr_type, foo, range, addr)`, boiler-plate if(NULL) checks are already included (see macro expansion in your editor), and needn't be repeated by user. 

- **Macro wrap of free() and munmap():** provides polymorphism of either de-allocating safe ptrs (as simply as `free(foo)` and `munmap(foo)`) or de-allocating normal ptrs (with std C interface). Moreover, an `if(NULL)` check is also inserted so that double de-allocations are redundant. The user is the best judge of when to free a memory, but in complex CFGs - or when in doubt - it's better to make over-use of the overloaded free() or munmap(), as mstrct.h ensures redundant de-allocations are **elided by the compiler**, rather than corrupting memory.

## API reference

- `$(...)` **macro:**
```
    $(foo): foo = safe ptr name
    $(foo, index): foo = safe ptr name, index = a numeric type

    $(type, foo, ): type = ptr typ, foo = a new name, or a struct's field name
    $(type, foo, range, addr): type = ptr type, foo = new name (or one masking another), range = a numeric type, addr = 64 bit value
    $( , foo, range, addr): similar to above

```

- **memstruct:**
mstrct.h targets ptrs holding array-like memory. Much like how a ptr variable's type carries static metadata about the data it points to, a memstruct carries even richer set of information in its type system. As the layout below shows, only id and type fields may be of immediate user interest in general, even as the rest play equal role in memory safety.
```
    struct {
      union {
        uint32_t id;
        struct {
          uint16_t _d;
          uint16_t _s;
          type typ[0];
          struct {char a[line];}  lin[0];
          struct {char a[range];} ran[0];
          struct {char a[card];}  car[0];
          struct {char a[cons];}  con[0];
        };
      };
    }

    // field description:
       foo.id: public API
       foo._d: (private) the offset of metadata in custom static segment
       foo._s: (private) usually static ID for immediate access
       typeof(foo.typ[0]): ptr type
       sizeof(foo.lin[0]): __LINE__ at declaration site
       sizeof(foo.ran[0]): memory range if static, else 0
       sizeof(foo.car[0]): cardinality of name, 1 if not multidim
       sizeof(foo.con[0]): 1 if ptr is *const type, 0 if not

```
- **metadata:** metadata fields are accessed as `$(foo).metadata` e.g. `$(foo).addr`, `$(foo).size` and `$(foo).base`. This API is mainly for internal use, but also made available to enable ptr arithmetic (like so: `$(foo).addr++`), and to meet the metadata access needs of the occasional user. **Note:** since multidim names don't have separate metadata for each element, `$(foo[i][j]..).metadata` results in error -- there is only `$(foo).metadata` available.
- **Raw access** of data through `$(foo).addr[index]` (verbose on purpose!) is allowed, mainly for occasional cases e.g. when clear performance benefits (of raw access) can be proven and/or primary check is already hoisted before a hot loop. 

```
    // meta data struct layout
    typedef struct  {
      ptr_type        addr; // (mutable)   ptr addr
      const uint64_t  size; // (immutable) memory byte size
      ptr_type const  base; // (immutable) base addr
    }
```

##  Troubleshooting
LTS for memstruct is ensured as mstrct.h will be used in a forthcoming project. Currently, however, mstrct.h is at an initial stage. So, bugs/errors can be directly reported here. Raise an issue if you need to discuss or ask clarification, too.

## ![memstruct banner](banner.svg) Contributing guidelines

NOTE: early contributors and adopters

TODO: CONTRIBUTING.md

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
Issues:
Search existing issues before opening a new one.

When reporting bugs, include commands, versions, and a minimal example.

License:
By contributing, you agree your code will be distributed under the project’s license (see LICENSE).
