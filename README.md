# decrout

```decrout
// Calculate the second power of an unsigned number
const square: >rt [u32]: u32 = rt [x: u32]: u32 {
    return x * x;
}
```
Decrout is a hobby language, mostly inspired by C and Rust. It aims to be close to bare metal, with just enough abstraction to make writing in it more comfortable than assembly. The name comes from the words "DEClare ROUTine".

The language itself is still in development, current state is (not necessarily up to date with the compiler) described in the [language_design_notes](language_design_notes) directory. The [examples](examples) directory contains some code snippets which aim to illustrate the spec (again, may be not up to date) and [test_data](test_data) contains data that is not valid decrout, but is useful in testing parts of the compiler, for example token streams etc.

The state of the language *will* change in the future as it is in a very early development stage and i am not quite sure of everything i want it to do.

### Why?
The purpose of the language is for me to have fun and learn while developing it. In the end i would like to support structural types, floating point arithmetic and possibly OOP though i am not yet sold on that one. Planned backends include custom backend for x86_64 assembly and an LLVM one, although those are still abstract ideas for the future.

# dcrtc
The decrout compiler, called dcrtc, is currently developed in C. Once the language is ready i plan on rewritting dcrtc in decrout itself, and renaming current C-based dcrtc to dcrtc-bootstrap as well as freezing it.

The intended functionality is for dcrtc to consume a single source file of decrout and produce a single assembly file from it (or some other output, depending on the backend), which can be then assembled by the GAS. Intended extension for decrout source files is .dcrt (this may change in the future, as it is very similar to the Dart language).

### Current state of the compiler
Dcrtc was developed up to the point of parsing declarations properly, but then i decided the code was too disorganized and needed a refactor/rewrite. Currently lexing/tokenization has been redone, but the interface is not compatible with old version of parsing. Parsing of declarations will be refactored and then i can move onto parsing of expressions etc.

### Building
Just run `make` in the root directory of the project (the one this README is stored in). This will create a build directory which contains all the object files and the dcrtc binary. At the moment one needs some kind of C compiler to compile it. It uses libc extensively, but has no other dependencies besides it.

# License and copyright
I am not sure if the language itself can be licensed, but my intention is for anyone to be able to use it free of charge. As for the dcrtc code it is licensed under GPLv3 as described in the [LICENSE](LICENSE) file.

Copyright (C) 2023  Maciej Sawka <maciejsawka@gmail.com> <msaw328@kretes.xyz>
