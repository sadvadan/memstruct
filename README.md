
# ![memstruct banner](banner.svg) memstruct v1.0.0 README

C + memstruct = performance + memory safety 

## Features

- **Code size**    - memstruct is a 370 LoC single header-file library with no external dependencies.
- **Memory safety**- covers UAF, NULL deref, OOB (multi-dim), leaks, double free & memory sharing, for both array & non-array types.
- **Thread safety**- provides single-write memory & read-only share. complements libs like pthread when atomics/locks are needed.
- **Performance**  - compile-time / largely elided / hoisted / pipelined runtime checks to match native C speed at >O0.
- **User ease**    - convenience macro `m()` / `M()`, substituting e.g. `foo[i]` aka `*(foo + i)` with `m(foo,i)`.
- **Robustness**   - linter (gcc & clang) and compile-time (gcc only) warning for bad grammar, puns, and hatches.
- **Target**       - gcc, clang | -std=gnu99 &ONWS | 8-64 bit CPUs. batteries included: opt-out, hardening, and MCU flags.

## Quick Start

- **Include**

    `mstrct.h` in your file.
- **Declare and allocate** a memstruct:

    declaration prototype (4 args): `M(ptr_type, ptr_qualifier, name, optional_static_indexes)`

    allocation prototype (3 args): `M(any_allocator, name, dynamic_index)`
    ```
    M(int, const, foo,);        // declare simple foo as (int *const)[][1]
    M(auto, foo, 3);            // allocate on-stack as int[3][1] or simply int[3]
    M(auto, foo, (1,2,3));      // or allocate on-stack as int[3] = {1,2,3} (initializer list)

    M(int, , bar, (2,5,7));     // declare multidim bar as (int *)[][2][5][7]
    M(malloc(2800), bar, 10);   // allocate bar on-heap as (int *)[10][2][5][7]
    ```
- **Re-allocate** memory: same as allocation, `M(re_allocator, name, dynamic_index)`.

- **Read / write** memory: `m(name,(i,j...)) is eqv to name[i][j]...`
    ```
    m(foo,5) = 10;              // 1-D memstruct

    m(bar,(5,7,2)) = 10;        // multidim memstruct
     ```
- **Metadata** access:
     ```
    m(foo,sizeof)               // byte size

    m(foo,void)                 // base addr

    m(foo,_)                    // index span

    m(foo,enum)                 // foo ID

    m(foo)                      // foo first element
     ```
- **Share** memory: simply pass around the int `m(foo,id)`.
    ```
    m(bar,enum) = m(foo,enum);  // bar now shares memory with foo 
    
    func(m(foo,enum), args);    // share memory with callee

    func(m(foo,sizeof), args);  // or share other metadata
     ```
     in multithreading contexts, use `M(void, foo, thread_enum)` taxonomy (see, doc).

- **index** arithmetic:
     ```
    foo.i++;                    // array index increment

    foo.i--;                    // array index decrement

    foo.i = 0;                  // set array index
     ```
- **De**-allocate memstruct: double de-allocation is idempotent (gets elided!).

    prototype: `M(any_de_allocator, name)`
     ```
    M(free, foo);                 // on-heap memory

    M(munmap, bar);               // mmapped memory
     ```
- **Get errno**: `m()` generates memory related errno's (1-6). thread safe.


## Documentation 
- See: [Documentation](doc/Documentation.md)

## License
This project is licensed under the AGPL-3.0 License - see the [LICENSE](LICENSE) file for details.
Copyright (c) 2026 @sadvadan.
