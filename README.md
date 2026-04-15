
# ![memstruct banner](banner.svg) memstruct README

Welcome to memstruct!

## 🎯 Features

- `memory safe` - deterministically covers: UAF, NULL deref, OOB, leaks, double free.
- `performant` - lightweight single file lib: leverages comptime checks, const folding, builtins & asm.
- `user ease` - exposes a thin macro `$(...)`, substituting e.g. `arr[i]` that is `*(arr + i)` with `$(arr, i)`.
- `target` - large scale sytem critical and performant systems. for resource critical applications memory checks can be switched off in prod.

## 🚀 Quick Start

- include `mstrct.h` in your file.
- declare a name and bind a memory to it: `$(ptr-type, name, range, addr)`; sample:
    ```
    // on-heap arr of 10 longs
    $(long int *, foo, 10, malloc(40));

    // on-stack multi-dim arr of range 10
    $(int * const, var[2][3], 10, (int [60]){0});

    // declare safe ptr w/o defining; could be inside struct type declaration
    $(float *, cux, );

    ```



📖 Documentation 

