
# ![memstruct banner](banner.svg) memstruct v1.0.0 README

C + memstruct = performance + memory safety 

## Features

- **Code size**    - memstruct is a 330 LoC single header-file library with no external dependencies.
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

    declaration: `m(name, static_index, type)`

    allocation on heap: `M(name, allocator_name, allocator_args...)`

    declaration + allocation on stack or global: `m(name, static_index, type, storage)`
    ```
    m(foo, 1, int);             // declare simple foo as (int *const)[][1]
    M(foo, malloc, 48);         // allocate on-heap as int[][1] + int[12][] = int[12][1], i.e. simply int[12]

    m(bar, 10, int, auto);      // declare and allocate on-stack an int[10]

    m(baz, (2,5,7), int);       // declare multidim baz as (int *)[][2][5][7]
    M(baz, malloc, 2800);       // allocate bar on-heap as (int *)[10][2][5][7]
    ```
- **Re-allocate** memory: same as allocation.

- **Read / write** memory: `m(name,(i,j...)) is eqv to name[i][j]...`
    ```
    m(foo,5) = 10;              // 1-D memstruct

    m(bar,(5,7,2)) = 10;        // multidim memstruct
     ```
- **Metadata** access:
     ```
    &m(foo)                     // foo base addr

    m(foo,_)                    // foo index span

    m(foo,auto)                 // foo ID

    m(foo)                      // foo first element
     ```
- **Share** memory: simply pass around the int ID `m(foo,auto)`.
    ```
    m(bar,auto) = m(foo,auto);  // bar now shares memory with foo 
    
    func(m(foo,auto), args);    // share read-write memory with callee

    func(m(foo,_), args);       // read-only memory (or share other metadata)
     ```
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

- **Other features**: mostly thread safety related API, refer doc (link below).


## Documentation 
- See: [Documentation](doc/Documentation.md)

## License
This project is licensed under the AGPL-3.0 License - see the [LICENSE](LICENSE) file for details.
Copyright (c) 2026 @sadvadan.
