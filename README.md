
# ![memstruct banner](banner.svg) memstruct README

Welcome to Memory Safe C!

## 🎯 Features

- `memory safe` - deterministically covers: UAF, NULL deref, OOB, leaks, double free.
- `performant` - lightweight single file lib: leverages comptime checks, const folding, builtins & asm.
- `user ease` - exposes a thin macro ($), substituting e.g. arr[i] aka *(arr, i) with $(arr, i).

## 🚀 Quick Start

- include `mstrct.h` in your file



📖 Documentation 

