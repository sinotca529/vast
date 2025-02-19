// RUN: vast-cc --from-source %s | vast-opt --vast-hl-splice-trailing-scopes --vast-hl-lower-types | FileCheck %s

// CHECK-LABEL: hl.func external @main () -> si32
int main()
{
    // CHECK: hl.var "a" : !hl.lvalue<si32>
    int a;

    // CHECK: hl.var "b" : !hl.lvalue<si32> = {
    // CHECK:   [[V1:%[0-9]+]] = hl.const #hl.integer<1> : si32
    // CHECK:   hl.value.yield [[V1]] : si32
    // CHECK: }
    int b = 1;

    // CHECK: hl.var "ub" : !hl.lvalue<ui32> = {
    unsigned int ub = 1U;

    // CHECK: hl.var "c" : !hl.lvalue<si32> = {
    // CHECK:   [[V2:%[0-9]+]] = hl.const #hl.integer<1> : si32
    // CHECK:   hl.value.yield [[V2]] : si32
    // CHECK: }
    int c( 1 );

    int ni = -1;

    // CHECK:   [[V4:%[0-9]+]] = hl.implicit_cast [[V5:%[0-9]+]] IntegralCast : si32 -> si64
    long nl = -1;
}
