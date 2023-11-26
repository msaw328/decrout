#!/bin/bash

make

# Just a helper script to run valgrind leak check on the dcrtc
# Compilation uses a lot of dynamic memory so this is handy

valgrind --leak-check=full --track-origins=yes ./build/dcrtc ./examples/empty_decls.dcrt
