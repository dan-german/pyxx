# RUN: %pyxx %s -ir | FileCheck %s

def a():
  a = 1 if True else 2
  return a

# CHECK: define i32 @a() {
# CHECK: entry:
# CHECK:  %0 = alloca i32, align 4
# CHECK:  %1 = select i32 1, i32 1, i32 2
# CHECK:  store i32 %1, ptr %0, align 4
# CHECK:  %2 = load i32, ptr %0, align 4
# CHECK:  ret i32 %2
# CHECK:  }

def b():
  return 2 if False else 3

# CHECK: define i32 b() {
# CHECK: entry:
# CHECK:   %0 = select i32 0, i32 1, i32 3
# CHECK: }