import subprocess

def dump_asm(code): 
    args = ["clang++", "-x", "c++", "-", "-S", "-o", "-", "-O3","-Xclang", "-funwind-tables=0"]
    subprocess.run(args, input=code, text=True)

def dump_ast(code):
    args = ["clang++", "-Xclang", "-ast-dump", "-fsyntax-only", "-x", "c", "-"]
    subprocess.run(args, input=code, text=True)

def dump_llvmir(code): 
    args = ["clang++", "-Xclang", "-emit-llvm", "-S", "-x", "c++", "-", "-o", "-", "-O0"]
    subprocess.run(args, input=code, text=True)

# dump post preprocessor
def dump_pp(code): 
    subprocess.run("clang -E -", input=code, shell=True,text=True)

def run(code, output="a.out"):
    subprocess.run(["clang", "-x", "c", "-", "-o", output], input=code, text=True, check=True)
    subprocess.run(["./" + output], check=True)

if __name__ == "__main__":
    code = """
    int f() {
        int a = 1;
        return a ? 1 : 2;
    }
    """

# define i32 @_Z1fiii(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
#   %4 = alloca i32, align 4
#   %5 = alloca i32, align 4
#   %6 = alloca i32, align 4
#   store i32 %0, ptr %4, align 4
#   store i32 %1, ptr %5, align 4
#   store i32 %2, ptr %6, align 4
#   %7 = load i32, ptr %4, align 4
#   %8 = load i32, ptr %5, align 4
#   %9 = add nsw i32 %7, %8
#   %10 = load i32, ptr %6, align 4
#   %11 = add nsw i32 %9, %10
#   ret i32 %11
# }

    dump_llvmir(code)
    dump_asm(code)