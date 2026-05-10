This project has been created as part of the 42 curriculum by jdelorme (Volmer42)

# libasm

## Description
The **libasm** project is an introduction to x86_64 assembly language. The goal is to rewrite a set of basic `libc` functions in Assembly (NASM syntax) to run exactly as their standard counterparts in macOS. This project provides a deeper understanding of how C functions interact with memory, the stack, and system calls under the hood.

The following functions have been implemented:
* `ft_strlen`: Calculates the length of a string.
* `ft_strcpy`: Copies a string to a destination buffer.
* `ft_strcmp`: Compares two strings lexicographically.
* `ft_write`: Writes data to a file descriptor using the `sys_write` syscall.
* `ft_read`: Reads data from a file descriptor using the `sys_read` syscall.
* `ft_strdup`: Duplicates a string using dynamic memory allocation (`malloc`).

## Instructions
To compile the library and test the functions, follow these steps:

1. Clone the repository and navigate to the project directory:
   ```bash
   cd libasm
   ```

2. Compile the static library (`libasm.a`):
   ```bash
   make
   ```

3. Clean object files:
   ```bash
   make clean
   ```

4. Remove all compiled files (including the library):
   ```bash
   make fclean
   ```

5. Recompile everything from scratch:
   ```bash
   make re
   ```

6. Run the provided tests to compare the assembly functions against the standard C library:
   ```bash
   ./run_tests.sh
   ```

## Resources
* [NASM Tutorial](https://cs.lmu.edu/~dr/gcc/nasm.html)
* [macOS System Calls list](https://opensource.apple.com/source/xnu/xnu-1504.3.12/bsd/kern/syscalls.master)
* [Intel x86_64 Architecture Software Developer's Manual](https://software.intel.com/content/www/us/en/develop/articles/intel-sdm.html)
* [System V ABI (x86-64)](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf) - Understanding registers and calling conventions.

## AI usage
In this project, AI was utilized **strictly as a mentor and tester**. It assisted in:
* Reviewing and explaining logic issues or segmentation faults in the assembly code.
* Explaning new concepts
* Structuring and completing the `main.c` testing suite to effectively validate the correctness of the custom assembly functions compared to libc.
* Creating documentation like this README.md