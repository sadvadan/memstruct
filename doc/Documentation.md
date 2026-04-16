
# ![memstruct banner](banner.svg) memstruct DOC

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

- This project formulates memory safety as error reporting system to complement `C`'s native performance, making it possible (going forward) to have reliable, large scale, collaborative projects in `C` that can leverage the language and its codebase.
    
- The core working principle is to have efficient custom static segment for metadata, such that accesses are as fast as in the stack. Then cache locality kicks in, supported by the custom error reporting made to respond to compiler optimizations.

- A "safe ptr" is basically a unique, anonymous struct type on the 'outside' but also of the size of a plain int (like a memory-ID) casually passed around among ptrs and functions. `$` is a thin convenience macro wrapper over this `memstruct`; however, there is good old `C` and `ASM` code transparent underneath!   

## Features and design

- Single‑header; no separate `.c` file needed.
- Supports on-heap, on-stack, and custom allocators.
- No external dependencies (only standard `C` headers).
- Designed to be inlined heavily by the compiler.
- Compile time checks & reporting wherever possible. (O1 and above helps, too)
- Safe to include in multiple translation units.
- Thread safety relies on target `x86` h/w-level atomicity and cache coherency through `MESI`, enforced by strict `ASM qword` alignment and `ASM "=m"` constraint, respectively.
- Logical concurrency for strict causal orderings is implemented by the user, and is orthogonal to this library's workings.

## Configuration

- Include `mstrct.h`.
- Define `NMSTRCT` to disable all checks for production, if needed.
- Use `MSTRCT_L` flag for choosing hardening level of error reporting.
```
    default 0: print detailed err, continue with default "start of the arr value"
    strict  1: print detailed err, halt program with segfault at error site
    strict  2: print detailed err, exit program with mstrct_status code
```
## Usage

- Memory sharing: memstruct field id is passed around to share memory. No special distinctions such as ownership, borrow, special, reference count, unique etc are needed.
```
    bar.id = foo.id; // makes bar refer the same memory as foo, but accessed as per its type "view"
    callee_function(int id, other_inputs); // callee is supplied foo.id as int to access the same memory in its scope
```
- Safe access of data: `$(foo, index)` is equivalent to `foo[index]` but with memory checks (as needed!) inlined.

- Raw access of data: `$(foo)` is data address as L value, so `*$(foo)` or `$(foo)[index]` is raw access without checks. This API exists mainly for ptr arithmetic (which by itself is safe). For data access, its use is advised mainly for cases e.g. when clear performance benefits can be proven and/or primary check is already hoisted. 

- memstruct declaration: declare a "safe ptr" foo as `$(ptr_type, foo, range, addr)`. If foo is already declared as a safe ptr, then call `$( , foo, range, addr)` for reassign.
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
    This relation is enforced by the library (at compile time, where possible) and any error informed to the user.

- In `$(ptr_type, foo, range, addr)`, addr is not necessarily an allocator but could also be an address pointing to a memory. Such usage does not pose memory hazard by its own, and as such is left to the occasional user to suit their program logic.

## API reference

- Memstruct layout:

mstrct.h targets ptrs holding memory. Much like how a ptr variable's type carries static metadata about the data it points to, a memstruct carries even richer set of information in its type system. As the layout below shows, only id and type fields may be of immediate user interest in general, even as the rest play equal role in memory safety.
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

```
- Metadata layout: metadata fields are accessed as `$(foo,)->size` etc. This API is mainly for internal use, but also made available to the occasional user needing metadata.
```
    // meta data struct layout (lives in custom static segment)
    typedef struct  {
      uint64_t addr;        // (mutable) ptr addr
      const uint64_t  size; // (immutable) memory byte size
      const uint64_t  base; // (immutable) base addr
    } mstrct_meta;

```
 
