// RUN: vast-cc --ccopts -xc --from-source %s | FileCheck %s
// RUN: vast-cc --ccopts -xc --from-source %s > %t && vast-opt %t | diff -B %t -

// adapted from https://gist.github.com/fay59/5ccbe684e6e56a7df8815c3486568f01

// CHECK: hl.func external @foo
int foo(int* ptr, int index) {
    // When indexing, the pointer and integer parts
    // of the subscript expression are interchangeable.
    // CHECK: [[P:%[0-9]+]] = hl.ref %arg0 : !hl.lvalue<!hl.ptr<!hl.int>>
    // CHECK: [[PC:%[0-9]+]] = hl.implicit_cast [[P]] LValueToRValue : !hl.lvalue<!hl.ptr<!hl.int>> -> !hl.ptr<!hl.int>
    // CHECK: [[I:%[0-9]+]] = hl.ref %arg1 : !hl.lvalue<!hl.int>
    // CHECK: [[IC:%[0-9]+]] = hl.implicit_cast [[I]] LValueToRValue : !hl.lvalue<!hl.int> -> !hl.int

    // CHECK: hl.subscript [[PC]] at [[[IC]] : !hl.int] : !hl.ptr<!hl.int> -> !hl.lvalue<!hl.int>

    // CHECK: [[P:%[0-9]+]] = hl.ref %arg0 : !hl.lvalue<!hl.ptr<!hl.int>>
    // CHECK: [[PC:%[0-9]+]] = hl.implicit_cast [[P]] LValueToRValue : !hl.lvalue<!hl.ptr<!hl.int>> -> !hl.ptr<!hl.int>
    // CHECK: [[I:%[0-9]+]] = hl.ref %arg1 : !hl.lvalue<!hl.int>
    // CHECK: [[IC:%[0-9]+]] = hl.implicit_cast [[I]] LValueToRValue : !hl.lvalue<!hl.int> -> !hl.int
    // CHECK: hl.subscript [[PC]] at [[[IC]] : !hl.int] : !hl.ptr<!hl.int> -> !hl.lvalue<!hl.int>
    return ptr[index] + index[ptr];
    // It works this way, according to the standard (§6.5.2.1:2),
    // because A[B] is the same as *(A + B), and addition
    // is commutative.
}
