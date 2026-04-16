
# ![memstruct banner](banner.svg) memstruct README

Welcome to memstruct -- a compile time leveraged memory safety framework for performant, large scale C applications !

## 🎯 Features

- `memory safe` - Covers: UAF, NULL deref, OOB, leaks, double free.
- `performant`  - Compile time checks, const folding, VRP, builtins & asm; elision of run time ops where possible.
- `user ease`   - Thin convenience macro API `$(...)`, substituting e.g. `arr[i]` aka `*(arr + i)` with `$(arr, i)`.
- `suited for`  - Sytem critical, performant systems. Toggle mem-checks off for resource scarce smaller prods.
- `target`      - gcc, clang: -std=gnu99 &ONWS; x86_64.

## 🚀 Quick Start

- Include `mstrct.h` in your file.
- Declare a name and bind a memory to it `$(ptr-type, name, range, addr)` like so:
    ```
    // on-heap arr of 10 longs
    $(long int *, foo, 10, malloc(40));

    // on-stack multi-dim arr of range 10
    $(int * const, bar[2][3], 10, (int [60]){0});

    // declare safe ptr w/o defining; could be inside struct type declaration
    $(float *, cux, );
    ```
- Re-assign memory `$(,name, range, addr)`:
    ```
    $(, var, 16, (int [16]){0});
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
- Raw access:
     ```
    // (safe) ptr arithmetic
    $(foo)++

    // (unsafe) access (as L value) without memory checks:
    $(foo)[5] = 10;
     ```
- De-allocate:
     ```
    // automatic metadata update for on-stack memories.
    // free & munmap are thinly wrapped so that double frees are redundant (instead of causing corruption) and can be used freely as the redundant frees are elided by compiler.

    free(foo);
     ```
## 📖 Documentation 
- See: [Documentation](doc/Documentation.md)

