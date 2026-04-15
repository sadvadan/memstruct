
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

- This project provides memory safety as an error mechanism to complement C's performance, making it possible (going forward) to have `reliable, large scale, collaborative projects` in C that still leverage the language and its codebase.
    
- The core working principle is to have efficient custom static segment for metadata, such that accesses are as fast as in the stack. Then cache locality kicks in, further helped by a custom error reporting made to respond to compiler optimizations.

- A "safe ptr" is basically a unique, anonymous struct type on the 'outside' but also of the size of a plain int (like a memory-ID) casually passed around among ptrs and functions. `'$'` is a thin macro wrapper over this `'memstruct'`, and underneath transparent is plain old C and ASM code.   

## Features and design

- Single‑header; no separate `.c` file needed.
- No external dependencies (only standard C headers).
- Designed to be inlined heavily by the compiler.
- Safe to include in multiple translation units.
- Thread safe: for mutable `8B` size global metadata (`addr`), the library implements -
    a) strict 8B alignment (through ASM) to rely on hardware level atomicity in x86,
    b) `"=m"` ASM constraint to enforce cache coherency of relevant region through x86 `MESI`.
- Logical concurrency: as access to data is through metadata (`addr`), mutexes or locks covering data accesses cover metadata automatically. Thus no atomics or locks need be separately implemented in memstruct.

