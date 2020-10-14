#!/bin/bash

clang -Iinclude/ -emit-llvm -S src/ring.c
llvm-as ring.ll -o ring.bc