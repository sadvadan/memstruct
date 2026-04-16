
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

- This project provides memory safety as an error mechanism to complement `C`'s performance, making it possible (going forward) to have `reliable`, `large scale`, `collaborative` projects in `C` that still leverage the language and its codebase.
    
- The core working principle is to have efficient custom static segment for metadata, such that accesses are as fast as in the stack. Then cache locality kicks in, further helped by a custom error reporting made to respond to compiler optimizations.

- A "safe ptr" is basically a unique, anonymous struct type on the 'outside' but also of the size of a plain int (like a memory-ID) casually passed around among ptrs and functions. `$` is a thin macro wrapper over this `memstruct`, however, underneath transparent is plain old `C` and `ASM` code.   

## Features and design

- Single‑header; no separate `.c` file needed.
- No external dependencies (only standard `C` headers).
- Designed to be inlined heavily by the compiler.
- Safe to include in multiple translation units.
- Thread safety relies on target `x86` h/w-level atomicity and cache coherency through `MESI`, forced by strict `ASM` 8B alignment and `ASM "=m"` constraint respectively.
- Logical concurrency for desired causal orderings is implemented by the user, and is orthogonal to this library's workings.

- memstruct layout:
  Much like how a ptr variable's type carries static metadata about the data it points to, a memstruct carries even richer set of information in its type system. As the layout below shows, only id and type fields may be of immediate user interest in general.
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
       id: public API
       _d: (private) the offset of metadata in custom static segment
       _s: (private) usually static ID for immediate access
       typeof(typ[0]): ptr type
       sizeof(lin[0]): __LINE__ at declaration site
       sizeof(ran[0]): memory range if static, else 0
       sizeof(car[0]): cardinality of name

```
