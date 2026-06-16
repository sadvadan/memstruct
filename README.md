
# ![memstruct banner](banner.svg) memstruct v1.0.0 README

C + memstruct = performance + memory safety 

## Features

- **Code size**    - memstruct.h is a single file 350 LoC library with no external dependencies.
- **Memory safety**- targets pointers to cover UAF, NULL deref, OOB, leaks, & double free.
- **Performance**  - compile-time / largely elided / hoisted / pipelined runtime checks to match native C speed.
- **User ease**    - convenience macro `m()` / `M()`, substituting e.g. `foo[i]` aka `*(foo + i)` with `m(foo,i)`.
- **Robustness**   - either compile-time or linter flags for bad grammar, puns, and illegal raw dereferences.
- **Target**       - gcc, clang: -std=gnu99 &ONWS, 8-64 bit CPUs. batteries included: opt-out, hardening, & MCU flags.

## Quick Start

- **Include**

    `mstrct.h` in your file.
- **Declare and allocate** a memstruct:

    declaration prototype: `M(ptr_type, name,, static_indexes)`

    allocation prototype: `M(any_allocator, name, dynamic_index)`
    ```
    M(int *,foo,);                // declare simple foo as int[][1]
    M(auto,foo,10);               // allocate on-stack foo as int[10][1]

    M(int *const,bar,,2,5,7);     // declare multidim bar as int[][2][5][7]
    M(malloc(2800),bar,10);       // allocate bar on-heap as int[10][2][5][7]

    M(int *,foo,) = {0};          // declare foo as int[][1] & assign foo.id=0 & foo.i=0
    ```
- **Re-allocate** memory: same as allocation, `M(re-allocate, name, dynamic_index)`.
- **Share** memory: simply pass around the int `foo.id`.
    ```
    bar.id = foo.id;              // bar now shares memory with foo 
    
    func(foo.id, other_inputs);   // share memory with callee
     ```
- **Read / write** memory: `m(name,i,j...) is safe name[i][j]...`
    ```
    m(foo,5) = 10;                // simple memstruct

    m(bar,5,7,2) = 10;            // multidim memstruct
     ```
- **Metadata** access:
     ```
    m(size foo)                   // byte size

    m(base foo)                   // base addr

    m(span foo)                   // index span

    m(id foo)                     // foo ID
     ```
- **index** arithmetic:
     ```
    foo.i++;                      // array index increment

    foo.i--;                      // array index decrement

    foo.i = 0;                   // set array index
     ```
- **De**-allocate memstruct: double de-allocation is idempotent (gets elided!).

    prototype: `M(any_de_allocator, name)`
     ```
    M(free, foo);                 // on-heap memory

    M(munmap, bar);               // mmapped memory
     ```
## Documentation 
- See: [Documentation](doc/Documentation.md)

## License
This project is licensed under the AGPL-3.0 License - see the [LICENSE](LICENSE) file for details.
Copyright (c) 2026 @sadvadan.
