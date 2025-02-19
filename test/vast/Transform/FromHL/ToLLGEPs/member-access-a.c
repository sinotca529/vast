// RUN: vast-cc --ccopts -xc --from-source %s | vast-opt --vast-hl-splice-trailing-scopes --vast-hl-lower-types --vast-hl-to-ll-geps | FileCheck %s

struct X { int a; };

void fn()
{
    struct X x;
    // CHECK: [[V:%[0-9]+]] = "ll.gep"(%1) {idx = 0 : i32, name = "a"} : (!hl.lvalue<!hl.elaborated<!hl.record<"X">>>) -> !hl.lvalue<si32>
    x.a = 5;
}
