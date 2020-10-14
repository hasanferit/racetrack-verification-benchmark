#!/bin/bash

clang -Iinclude/ -emit-llvm -S src/barto_big.c
llvm-as barto_big.ll -o barto_big.bc