
# ![memstruct banner](banner.svg) memstruct README

C + memstruct = performance + memory safety 

## 🎯 Features

- **Code size**    - memstruct.h is a single file 400 LOC library with no external dependencies.
- **Memory safety**- targets pointers to cover UAF, NULL deref, OOB, leaks, & double free.
- **Performance**  - comptime, or largely elided / hoisted runtime checks; uses good amount of asm to retain / improve C speed.
- **User ease**    - convenience macro `m()` / `M()`, substituting e.g. `foo[i]` aka `*(foo + i)` with `m(foo,i)`.
- **Robustness**   - type checked C code underneath (your code editor itself flags bad memstruct grammar).
- **Target**       - gcc, clang: -std=gnu99 &ONWS, x86_64. "batteries" included: opt-out, & hardening flags.

## 🚀 Quick Start

- **Include**

    `mstrct.h` in your file.
- **Declare**

    a name `M(ptr-type,name,,multidim_index)` like so:
    ```
    // single dim foo (interal rep: foo[][1])
    M(int *,foo,);

    // multidim foo (internal rep: foo[][2][5][7])
    M(int * const,foo,,2,5,7);

    // declaration could be a field declaration inside a struct type definition
    ```
- **Assign** 

    the name a memory `M(storage,name,index)` like so:
    ```
    M(int *,foo,,2); // first declare foo[][2]
    M(malloc(80),foo,10); // allocate 80 heap bytes as foo[10][2]

    M(int *,foo,); // declare simple (dim=1) foo
    M(auto,foo,10); // allocate foo[10] on-stack
    ```
- **Re-assign** memory:

    `M(storage,name,index)`:
    ```
    M(malloc(80),foo,40); // same as assignment
    ```
- **Share** memory

    simply pass around `foo.id` (a `uint16_t`):
    ```
    M(foo.id, bar); // bar now shares memory with foo 

    Callee_function(foo.id, other_inputs); // share with callee
     ```
- **Read / write** memory

    `m(name,index) = value`:
    ```
    // single dim array types
    m(foo,5) = 10;

    // multi-dim array types
    m(bar,5,7,2) = 10;
     ```
- **Metadata** access

    `M(foo)` is *struct {addr, size}:
     ```
    uint64_t temp = M(foo)->size; // byte size as R value

    void *temp = M(foo)->addr; // base addr as R value
     ```
- **Raw** access

    `m(foo)` is the current addr (L value):
     ```
    m(foo)++; // ptr arithmetic; safe, as it's not dereferenced yet

    m(foo)[5] = 10; // unsafe escape hatch
     ```
- **De**-allocate

    double frees are redundant (later elided by compiler):
     ```
    free(foo);   // on-heap memory
    munmap(foo); // mmapped memory
     ```
## 📖 Documentation 
- See: [Documentation](doc/Documentation.md)

## License
This project is licensed under the AGPL-3.0 License - see the [LICENSE](LICENSE) file for details.
Copyright (c) 2026 @sadvadan.
