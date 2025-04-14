# RUN: %pyxx %s -ir | FileCheck %s

def f1():
  a = 2
  b = 4
  c = 6
  d = a*b+c
  return d
  
# CHECK: define i32 @f1() {
# CHECK: entry:
# CHECK:   %0 = alloca i32, align 4
# CHECK:   %1 = alloca i32, align 4
# CHECK:   %2 = alloca i32, align 4
# CHECK:   %3 = alloca i32, align 4
# CHECK:   store i32 2, ptr %0, align 4
# CHECK:   store i32 4, ptr %1, align 4
# CHECK:   store i32 6, ptr %2, align 4
# CHECK:   %4 = load i32, ptr %0, align 4
# CHECK:   %5 = load i32, ptr %1, align 4
# CHECK:   %6 = mul i32 %4, %5
# CHECK:   %7 = load i32, ptr %2, align 4
# CHECK:   %8 = add i32 %6, %7
# CHECK:   store i32 %8, ptr %3, align 4
# CHECK:   %9 = load i32, ptr %3, align 4
# CHECK:   ret i32 %9
# CHECK: }