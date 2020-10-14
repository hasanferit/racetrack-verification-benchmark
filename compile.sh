#!/bin/bash

clang -Iinclude/ -emit-llvm -S src/barto_small.c
llvm-as barto_small.ll -o barto_small.bc