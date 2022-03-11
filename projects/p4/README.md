# Floating point instructions (and ncurses)

In this project you will make use of floating point instructions as well as the floating point register set. Your calculations will be rendered to the terminal window using the library `ncurses`.

## Install ncurses

Become root using ```su```. Then use apt to install ncurses. The password is "a".

```text
su
apt install libncurses-dev
```

## What the program does

This program calculates the values of a sine wave (-1 to 1). It scales and converts the sine values to a range of 0 to 9. This value is used as in index into an array of `char`. This is to choose a single ASCII character approximating a brightness level. Putting this in motion, you'll get a crude sine wave animation on screen.

The program is terminated when you abort it with CONTROL-C.

For faster performance, use a terminal window that is small - such as 80 by 24. Having fewer characters to animate will increase the speed of animation. For examining your lovely results in detail, make your terminal window large and run the program again.

## The C version

You are given a deconstructed version of the program written in `C` in the style of assembly language.

Compile with:

```text
gcc main.c -lcurses -lm
```

The above command will also be used for building your assembly language version (with your assembly language version replacing the `C` version, of course).

The first link library is to get the curses code. The second is to get the math code (providing *sin*).

A working (*though not necessarily current*) version is printed here:

```c
#include <curses.h>
#include <unistd.h>
#include <math.h>

char * levels = " .:-=+*#%@";

int main()
{
	initscr();
	double tpi = 3.14159265359 * 2;
	double phase = 0;
	double theta = 0;
	double increment = tpi / (double) COLS;
	int l;
	int c;

top:	
	erase();
	phase += increment;
	l = 0;
sinner:	
	if (l >= LINES) goto bottom;
	theta = 0;
	c = 0;
tinner:
	if (c >= COLS) goto binner;
	int intensity = (int) ((sin(phase + theta) + 1.0) / 2.0 * 10);
	mvaddch(l, c, levels[intensity]);
	theta += increment;
	c++;
	goto tinner;

binner:	
	l++;
	goto sinner;

bottom:	
	box(stdscr, 0, 0);
	refresh();
	goto top;

	endwin();
	return 0;
}
```

Notice there are no higher level constructs such as for loops or braces (apart from the beginning and ending brace).

The execution of the program is terminated by aborting it with Control-C.

## Project

You are to translate this C version into AARCH64 assembly language.

You *must* use floating point instructions with the exception of the `sin` calculation. The AARCH64 ISA doesn't have a *sin* instruction so instead use the math library version. Calculating `sin` on ARM processors typically involved a lookup table. See [here](https://github.com/PaulStoffregen/arm_math/blob/master/src/arm_sin_cos_f32.c) for an example.

You must use floating point registers (S registers and / or D registers) in order to make use of the floating point instruction set. S is single precesion (float). D is double precision (double). We're using all doubles in this project.

Correct backing up and restoring of registers are required.

## HINT HINT HINT

Note the characters in this C-string:

```c++
char * levels = " .:-=+*#%@";
```

The first character is a space.

If your output shows ANY character other than these, you have a bug in your math causing a bad index into this string.

## SECOND HINT

If your code runs too quickly for you to see all the output you can do two things:

1. You can make your terminal screen larger before running the program. With more characters to render, your program will run slower.

2. You can temporarily pause out by striking `^s`. You can resume output with `^q`.

## Some rubric items

```text
70	(flat score) Crashes reliably however substantial work is demonstrated
-10	Backup / Restore register mismatch
-10	Incorrect output but substantially all work present
-5	Math error - pattern is not quite right but close
-?  Other problems but the above are the most common
```

## Reference

AARCH64 floating point instruction reference:

[Here](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0802b/a64_float_alpha.html)

Instructions you'll care about include the obvious add, divide and multiple. But also consider that you are converting from float to int at least once. There is an instruction for this purpose.

Information about floating point registers can be found:

[Here](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.den0024a/ch04s06s01.html)

[and here](https://developer.arm.com/documentation/100076/0100/a64-instruction-set-reference)

## Lecture on floating point operations

Our lecture on the topic of floating point operations was a few weeks ago. See *reference* if you didn't get enough understanding from when we discussed them.

## Due date

You have the week of Spring Break and then another week.

## Partner Rules

No partners. All work is solo.
