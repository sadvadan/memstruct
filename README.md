
# ![memstruct banner](banner.svg) memstruct v1.0.0 README

C + memstruct = performance + memory safety 

## 🎯 Features

- **Code size**    - memstruct.h is a single file <400 LoC library with no external dependencies.
- **Memory safety**- targets pointers to cover UAF, NULL deref, OOB, leaks, & double free.
- **Performance**  - comptime, or largely elided / hoisted runtime checks; uses good amount of asm to retain / improve C speed.
- **User ease**    - convenience macro `m()` / `M()`, substituting e.g. `foo[i]` aka `*(foo + i)` with `m(foo,i)`.
- **Robustness**   - type checked C code underneath (your code editor itself flags bad memstruct grammar).
- **Target**       - gcc, clang: -std=gnu99 &ONWS, x86_64. "batteries" included: opt-out, & hardening flags.

## 🚀 Quick Start

- **Include**

    `mstrct.h` in your file.
- **Declare and allocate** a memstruct:

    declaration prototype: `M(ptr_type, name,, static_indexes)`

    allocation prototype: `M(storage, name, dynamic_index)`
    ```
    M(int *,foo,);                // declare simple foo as int[][1]
    M(auto,foo,10);               // allocate on-stack foo as int[10][1]

    M(int *const,bar,,2,5,7);     // declare multidim bar as int[][2][5][7]
    M(malloc(2800),bar,10);       // allocate bar on-heap as int[10][2][5][7]
    ```
- **Re-allocate** memory: same as allocation, `M(storage, name, index)`.
- **Share** memory: simply pass around `M(foo)` which is a ptr to metadata.
    ```
    M(M(foo), bar);               // bar now shares memory with foo 
    
    func(M(foo), other_inputs);   // share memory with callee
     ```
- **Read / write** memory:
    ```
    m(foo,5) = 10;                // simple memstruct

    m(bar,5,7,2) = 10;            // multidim memstruct
     ```
- **Metadata** access:
     ```
    uint64_t temp = M(foo)->size; // const byte size

    void *temp = M(bar)->addr;    // const base addr
     ```
- **Raw** access:
     ```
    m(foo)++;                     // ptr arithmetic; safe , as not dereferenced yet

    m(bar)[5] = 10;               // unsafe escape hatch
     ```
- **De**-allocate memstruct: double free is harmless (gets elided, hurray).
     ```
    free(foo);                    // on-heap memory

    munmap(bar);                  // mmapped memory
     ```
## 📖 Documentation 
- See: [Documentation](doc/Documentation.md)

## License
This project is licensed under the AGPL-3.0 License - see the [LICENSE](LICENSE) file for details.
Copyright (c) 2026 @sadvadan.
