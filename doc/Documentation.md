
# ![memstruct banner](banner.svg) memstruct DOC

This document explains how to configure and use the memstruct.h library.

## Table of contents

- [Overview](#overview)  
- [Installation](#installation)  
- [Configuration](#configuration)  
- [Usage](#usage)  
- [API reference](#api-reference)  
- [Troubleshooting](#troubleshooting)  
- [Contributing guidelines](#contributing)

## Overview

- This project complements C with memory safety as error mechanism mostly orthogonal to C's performance. This makes it possible - going forward in the future - to have reliable, large scale, collaborative projects in C while still leveraging the language itself and also its legacy codebase.
    
- The core idea is to replicate efficiet stack layouts in a custom bss segment, such that metadata accesses are as fast as offset based accesses in the stack. Once this is out of the way, cache locality of addresses + metadata kicks in to make the baseline layout performant. This is further enhanced with a custom error mechanism made to respond to compiler optimization stages, before degrading to runtime checks utilizing constant foldings through compiler and assembler stages.

- A "safe ptr" is basically a unique, anonymous struct type on the 'outside' but also of the size of a plain int (like a memory-ID, possibly compile time constant) that is casually passed around among multiple partners, including callee functions. '$' is a thin macro wrapper over this 'memstruct', and underneath transparent is plain old C and ASM code.   


