# CSC3510 Spring 2021 - Project 1

The goal of this project is to familiarize yourself with developing AARCH64 assembly language applications using Visual Studio Code, SFTP, Debian Buster running in a QEMU virtual machine.

In my [book](https://github.com/CCCSBuffet/pk_do_not_be_afraid), you saw a deconstruction of the following program:

```c++
#include <iostream>

using namespace std;

int main(int argc, char * argv[]) {
	while (*argv) {
		cout << *(argv++) << endl;
	}
	return 0;
}
```

into AARCH64 assembly language. This program enumerates the command line arguments present in the invocation of the program.

You will hand in **two** deliverables.

1. You will take the C++ program given to you and fully **deconstruct** it to simplified C.
2. You will render the deconstructed C into **hand crafted** assembly language.

## Defining *deconstruction*

Deconstruction is taken to mean the elimination of all syntactic constructs besides `goto`, function calls, and `if` statements.

The above C++ program was deconstructed to this:

```c
#include <stdio.h>

int main(int argc, char * argv[]) {
	top:
		if (*argv == NULL)
			goto bottom;
		puts(*(argv++));
		goto top;

	bottom:
		return 0;
}
```

Notice the absence of any C++ syntax as well as the absence of the `while` loop. All that remains are the `if` and some `goto` statements plus function calls and a boolean condition check.

Specifically, no more than one brace pair may be used per function definition in the deconstructed source code.

## Original source code

You are given this program as the original C++ implementation from which you will first derive the fully deconstructed C code and then the hand crafted assembly language implementation.

```c++
#include <iostream>

using namespace std;

int MyStrlen(char * s) {
	int retval = 0;
	while (*(s++) != 0)
		retval++;
	return retval;
}

int main(int argc, char * argv[]) {
	while (*argv) {
		cout << "[" << MyStrlen(*argv) << "] [" << *(argv++) << "]" << endl;
	}
	return 0;
}
```

Here is an example of this program's execution:

```text
user@comporg:~/p1$ ./a.out harry barry and smelfish
[7] [./a.out]
[5] [harry]
[5] [barry]
[3] [and]
[8] [smelfish]
user@comporg:~/p1$ 
```

## Must use C-based `printf`

This line:

```c++
cout << "[" << MyStrlen(*argv) << "] [" << *(argv++) << "]" << endl;
```

cannot be rendered directly into C as streams do not exist in C. Instead you will use the equivalent:

```c
printf("[%d] [%s]\n", MyStrlen(*argv), *(argv++));
```

## Must implement `MyStrlen`

Certainly, there is a standard CRT (C runtime library) implementation of `strlen`. You may not use it. Instead, you must implement `MyStrlen`.

## Assembly language must be commented

You must comment your assembly language extensively. Comments are introduced with `//`.

## Assembly language hints

### `MyStrlen` is an int

You are expected to respect this and return a 32 bit value.

### Remember to backup `x29` and `x30` if necessary

If your function calls any other function, you must backup and restore `x29` and `x30`.

### If you use `x19` through `x28`, they must be backed up too

If you use any register from 19 through 28, you must backup and restore their contents.

### Dereferencing a pointer

All pointers must be in `x` registers because pointers are 64 bit entities.

How you dereference the pointer depends on what type of pointer it is.

```text
// Assume x0 holds a pointer
ldr	    x1, [x0]    // fetches a 64 bit value
ldr	    w1, [x0]    // fetches a 32 bit value
ldrh    w1, [x0]    // fetches a 16 bit value
ldrb    w1, [x0]    // fetches an 8 bit value
```

In this program, there is a place where you must dereference a pointer to a byte and also a place where you must dereference a pointer to a pointer.

### Loading the address of a C string

If `xxx` is a symbol for some ASCII text, then getting its address into `x0` is done with:

```text
    ldr    x0, =xxx
```

### Using `printf`

`x0` will contain the format string (see previous for getting its address). Each value to be printed will be in sequentially ordered register. 64 bit values are stored in `x` registers. All other integer sizes are stored in `w` registers. There are other register types for `float` and `double`.

### Special case comparisons

Rather than using the combination of compare then branch conditional statements, if you're testing against zero versus non-zero, there are simpler `cbz` and `cbnz` instructions.

## Setting expectations

It is my practice to reveal to you how many lines of code my implementations require. This is not a contest or a challenge. I provide these numbers only to set your expectations. For example, if I say I coded a solution in 10 lines and your's is approaching 900, you did something wrong.

My deconstructed C implementation is 26 lines including appropriate blank lines (and no comments).

My assembly language implementation is 39 lines including appropriate blank lines, directives such as `.global`, `.text`, `.align`, `.data` and `.end` but no comments.

## Work rules

All work is done solo.

## What to hand in

In schoology, you will upload one ZIP file containing two files: a .c source code file and a .s source code file.
