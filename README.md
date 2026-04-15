
# ![memstruct banner](banner.svg) memstruct README

Welcome to memstruct !

## 🎯 Features

- `memory safe` - Deterministically covers: UAF, NULL deref, OOB, leaks, double free.
- `performant`  - Lightweight single file lib: leverages comptime checks, const folding, builtins & asm.
- `user ease`   - Exposes a thin macro `$(...)`, substituting e.g. `arr[i]` that is `*(arr + i)` with `$(arr, i)`.
- `suited for`  - Large scale sytem critical, performant systems. Toggle mem-checks off for resource scarce prods.
- `target`      - gcc, clang: -std=gnu99 ONWS; x86_64.

## 🚀 Quick Start

- Include `mstrct.h` in your file.
- Declare a name and bind a memory to it `$(ptr-type, name, range, addr)`:
    ```
    // on-heap arr of 10 longs
    $(long int *, foo, 10, malloc(40));

    // on-stack multi-dim arr of range 10
    $(int * const, bar[2][3], 10, (int [60]){0});

    // declare safe ptr w/o defining; could be inside struct type declaration
    $(float *, cux, );
    ```
- Define memory `$(name, index) = value`:
    ```
    // single dim array types
    $(foo, 5) = 10;

    // multi-dim array types
    $(bar[1][2], 5) = 10;
     ```
- Share memory:
     ```
    // share memory with another safe type
    foo.id = baz.id;

    // share memory with callee
    callee(foo.id, other_inputs);
     ```
- Access meta-data:
     ```
    // address:
    $(foo,)->addr

    // range(immutable):
    $(foo,)->range

    // base address(immutable):
    $(foo,)->base
 
- De-allocate memory:
     ```
    // automatic for on-stack memories.
    // free & munmap are thinly wrapped so that double frees are redundant (instead of causing corruption) and can be used freely as the redundant frees are elided by compiler.
    free(foo);
     ```
📖 Documentation 

