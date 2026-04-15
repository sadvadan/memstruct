
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
    
- The core idea is to replicate the efficiency of stack layouts in a custom metadata bss segment, such that metadata accesses are as fast as offset type ptr accesses in stack memory. Once this hurdle is overcome, cache locality of ptr address and its metadata kicks in to make the baseline memory design performant. This is then enhanced with a custom error mechanism designed to respond to compile times and optimization levels before degrading to runtime checks that eventualy utilize constant foldings through compiler and assembler stages.

- Your "safe ptr" is basically a unique, anonymous struct type on the 'outside' but also of the size of a plain int (like a memory-ID that can be compile time constant) that is easily passed around among multiple partners, including callee functions. This "memstruct" design, as stated before, is orthogonal to the rest of the language: be it performance, concurrency, and so on.


