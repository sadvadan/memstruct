
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

- This project provides memory safety as an error mechanism to complement `C`'s performance, making it possible (going forward) to have reliable, large scale, collaborative projects in `C` that still leverage the language and its codebase.
    
- The core working principle is to have efficient custom static segment for metadata, such that accesses are as fast as in the stack. Then cache locality kicks in, further helped by a custom error reporting made to respond to compiler optimizations.

- A "safe ptr" is basically a unique, anonymous struct type on the 'outside' but also of the size of a plain int (like a memory-ID) casually passed around among ptrs and functions. `$` is a thin macro wrapper over this `memstruct`, however, underneath transparent is plain old `C` and `ASM` code.   

## Features and design

- Single‑header; no separate `.c` file needed.
- No external dependencies (only standard `C` headers).
- Designed to be inlined heavily by the compiler.
- Safe to include in multiple translation units.
- Thread safety relies on target `x86` h/w-level atomicity and cache coherency through `MESI`, forced by strict `ASM qword` alignment and `ASM "=m"` constraint respectively.
- Logical concurrency for strict causal orderings is implemented by the user, and is orthogonal to this library's workings.

## API reference

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
- Memory sharing: memstruct field id is passed around to share memory. No special distinctions such as ownership, borrow, special, reference count, unique etc are needed.
```
    bar.id = foo.id; // makes bar refer the same memory as foo, but accessed as per its type "view"
    callee_function(int id, other_inputs); // callee is supplied foo.id as int to access the same memory in its scope
```
- Safe access of data: `$(foo, index)` is equivalent to `foo[index]` but with memory checks (as needed!) inlined.
- Raw access of data: `$(foo)` is data address as L value, so `*$(foo)` or `$(foo)[index]` is raw access without inlined memory checks. this API exists mainly for ptr arithmetic, and data access advised to be done only on rare occasions when performance benefits can be proven or primary check is already hoisted before a loop. 

- Metadata layout: metadata fields are accessed as `$(foo,)->size` etc. This API is used internally, but also made available to the user needing accessing the metadata.
```
    // meta data struct layout (lives in custom static segment)
    typedef struct  {
      uint64_t addr;        // (mutable) ptr addr
      const uint64_t  size; // (immutable) memory byte size
      const uint64_t  base; // (immutable) base addr
    } mstrct_meta;

```
- memstruct declaration:
