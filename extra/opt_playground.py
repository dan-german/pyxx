import subprocess
import tempfile
import os

def opt(ir_string, flags=None):
    try:
        # Create a temporary file to hold the IR code.
        with tempfile.NamedTemporaryFile(mode='w+', delete=False, suffix=".ll") as temp_file:
            temp_file.write(ir_string)
            temp_file_name = temp_file.name

        opt_command = ["opt"]
        if flags:
            opt_command.extend(flags)
        opt_command.append(temp_file_name)

        return subprocess.run(opt_command, capture_output=True, text=True, check=True).stdout

    except subprocess.CalledProcessError as e:
        print(f"Error running opt: {e}")
        print(f"Stderr: {e.stderr}")
        return None
    except FileNotFoundError:
        print("Error: llvm opt not found. Make sure it is installed and in your PATH.")
        return None
    finally:
        if 'temp_file_name' in locals() and os.path.exists(temp_file_name):
            os.remove(temp_file_name)

input = """
define i32 @main() {
entry:
  %0 = alloca i32, align 4
  store i32 1, ptr %0, align 4
  %1 = load i32, ptr %0, align 4
  %2 = load i32, ptr %0, align 4
  %3 = icmp eq i32 %1, %2
  br i1 %3, label %then, label %else

then:                                             ; preds = %entry
  store i32 2, ptr %0, align 4
  br label %endif

else:                                             ; preds = %entry
  store i32 3, ptr %0, align 4
  br label %endif

endif:                                            ; preds = %else, %then
  %4 = load i32, ptr %0, align 4
  ret i32 %4
}
"""

optimized_ir = opt(input, flags=["-S", "-passes=mem2reg"])

if optimized_ir:
    print(optimized_ir)