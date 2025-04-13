# RUN: %pyxx %s -ir | FileCheck %s

def a():
  return 1

# CHECK: define i32 @a() {
# CHECK: entry:
# CHECK:   ret i32 1
# CHECK: }

def b():
  a = 1
  b = 2
  return a*b
  
# TODO: check optimized version
# CHECK: define i32 @b() {
# CHECK: entry:
# CHECK:   %0 = alloca i32, align 4 
# CHECK:   %1 = alloca i32, align 4 
# CHECK:   store i32 1, ptr %0, align 4 
# CHECK:   store i32 2, ptr %1, align 4 
# CHECK:   %2 = load i32, ptr %0, align 4 
# CHECK:   %3 = load i32, ptr %1, align 4 
# CHECK:   %4 = mul i32 %2, %3 
# CHECK:   ret i32 %4 
# CHECK: }