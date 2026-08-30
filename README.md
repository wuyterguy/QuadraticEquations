<<h1 align="center"> quadratic equations </h1>

---

<span style="font-size: 18px;">

### table of contents

* [overview](#overview)
* [features](#features)
* [demonstration](#demonstration)
* [option management](#usage)
* [used tools](#libraries)

---

#### overview
<a id="overview"></a>

This program is designed to solve quadratic equations. User has to provide coefficients of quadratic equation to the program, and it outputs the roots.

---

#### features
<a id="features"></a>

* The program supports input from the console and from file. You can set the mod by command line flag or the program ask you about it at runtime.
* The program capable of printing with a type writer effect.
* It is possible to conduct testing of function that calculate the roots at start of program launch.
* Contains a function that plots a graph of a quadratic equation.
* There is color printing.

---

#### demonstration
<a id="features"></a>

* __console input__

![console_input.png](console_input.png)


* __file input__

![file_input.png](file_input.png)


* __testing__ (in example expected value is incorrect)

![testing.png](testing.png)

* __graphic__

![graphic.png](graphic.png)

---

#### usage
<a id="usage"></a>

__launch__
User has to compile "___main.c___" by __c++__ compiler and run resulting file to launch the program.

__program options__
* Use command line flag (hereinafter referred to as CLF) ___-c___ or ___-f___
to set console or file input respectively.
if you specify both flags program will ask you to set input at runtime.
* Use CLF ___-a___
to activate printing with typewriter effect.
* Use CLF ___-t___
to conduct preliminary testing using tests from file specified by macro ___TESTFILE___.
* Use CLF ___-g___
to see a graphic of the quadratic equation with entered coefficients at the end.
* Change value of macro ___COLOR_SWITCH___ to ___ON___ or ___OFF___ to switch color printing

__for developers__
all function are documented in the file "title.c" using _doxygen_ syntax

---

#### used tools

__libraries__
The following __C/C++__ libraries were used in the program

* [__TXLib.h__](http://storage.ded32.net.ru/Lib/TX/TXUpdate/Doc/HTML.ru/)
for graphic (all _tx_-functions)
* __stdio.h__
for basic functions (_printf_, _fopen_, etc.)
* __stdlib.h__
for basic functions (_calloc_)
* __string.h__
for work with string (_strncpy_)
* __math.h__
for mathematical operations (_sqrt_)
* __ctype.h__

</span>
