# RUN: %pyxx %s -ir | FileCheck %s

def a():
  a = 1 if True else 2
  return a

# CHECK: define i32 @a() {
# CHECK: entry:
# CHECK:   %0 = alloca i32, align 4
# CHECK:   store i32 1, ptr %0, align 4
# CHECK:   %1 = load i32, ptr %0, align 4
# CHECK:   ret i32 %1
# CHECK: }

def b(): 
  return 2 if False else 3

# CHECK: define i32 @b() {
# CHECK: entry:
# CHECK:   ret i32 3
# CHECK: }