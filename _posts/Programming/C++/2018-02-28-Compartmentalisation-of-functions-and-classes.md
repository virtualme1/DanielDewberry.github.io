---
layout: default
title: Compartmentalising C++ projects
meta: Split up your C++ projects and use a makefile in order to boost efficiency 
categories: [programming,cpp,make,makefiles]
---

# Compartmentalising functions and classes for reuse and build efficiency

In this post, I'll use [clang++](https://clang.llvm.org/) to compile source files.  If you would prefer to use [g++](https://gcc.gnu.org/) or [another compiler](http://www.stroustrup.com/compilers.html), please feel free to do so.

Given a project such as the following:

`message_printer.cpp`:

```cpp
{% include Examples/cpp/compartmentalisation/message_printer.cpp %}
```

compilation is often trivial.

```bash
$ clang++ message_printer.cpp -o message_printer
$ chmod 755 message_printer
```


## Compartmentalising

When a project has one or more user defined function (UDF) and/or class (UDC), it is often prudent to move them to their own file(s) for future use by other programs or inclusion in a library.  To do so, we declare them in header files which end with `.h` and place the definition (contents) into a source (`.cpp`) file 

`main.cpp`:
```cpp
{% include Examples/cpp/compartmentalisation/main.cpp %}
```

`message.h`:
```cpp
{% include Examples/cpp/compartmentalisation/message.h %}
```
Take notice that there are pre-processor directives which constitute a 'header guard', in the header file.  They prevent the header being copied into the executable more than once.

`message.cpp`:
```cpp
{% include Examples/cpp/compartmentalisation/message.cpp %}
```
Including the header file in the relative source file allows us to forward declare the prototypes which is essential if we have multiple interdependent functions in one sourcefile.

<br />
To compile the source code we run the following commands:

```bash
$ clang++ -c message.cpp -o message.o
$ clang++ -c main.cpp -o main.o
$ clang++ main.o message.o -o message_printer
```
This system is useful for reusing source code but there is a distinct advantage when there are many 'included' functions and classes.  We can compile only the source files which have changed, and create the executable from them.  In a project which includes many tens or hundreds of UDFs or user defined classes, rebuilding all of the object files and linking them can incur a sizeable time penalty to the user.  Compiling only the updated files will speed this process up. 

If I were to alter `message.cpp` in some way, then `message.o`, `main.o`  and the executable `message_printer` would have to be recompiled.

## Makefile

[`make`](https://www.gnu.org/software/make/)  compares the timestamps of files to their dependencies, and takes action accordingly.  If a source file has been updated since it was last compiled, it will be recompiled as will any target which is dependent upon it.


A [`makefile`](https://www.gnu.org/software/make/)   further speeds up and simpilies the process by running with a single command invokation and performing the necessary work.

`makefile`:
```makefile
{% include Examples/cpp/compartmentalisation/makefile %}
```

The header files have been commented out as it may not be desireable to compile the source file if you have only made changes to the header by adding comments or further declarations.  Removing the `#` will make the targets depend upon `message.h` and therefore trigger compilation of `message.cpp` and `main.cpp` (and by extension, build the executable from `message_printer.cpp`).  It is possible to use [`makefile`](https://www.gnu.org/software/make/)  conditionals to create a switch which toggles this functionality.  That will be coverred in a separate article.


To invoke the process, run [`make`](https://www.gnu.org/software/make/)  from the working directory.

```bash
$ make
clang++ -c main.cpp -o main.o
clang++ -c message.cpp -o message.o
clang++  main.o message.o -o message_printer
```

I included two `.PHONY` targets, targets which are not files, which allow us to remove the object (`.o`) files by invoking `make clean` or remove the object files and the executable by invoking `make purge`.


## Summary 

In projects as small as this, the usefulness is not apparent however when you reach a very large code-base which take tens of minutes or hours to compile, it becomes efficient to only rebuild the necessary components, rather than the everything.  This is when the benefit is most tangible.
