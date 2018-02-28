---
layout: default
title: Compartmentalising C++ projects
meta: Split up your C++ projects and use a makefile in order to boost efficiency 
category: programming
---

# Compartmentalising functions and classes for reuse and build efficiency

## Example C++ project

message_printer.cpp:

```cpp
#include <iostream>

// forward declared function prototype
void print_message();

int main(){
    print_message()
    return 0;
}

// function definition
void print_message(){
    std::cout << "Hello world" << std::endl;
}
```

## Compiling
```
clang++ message_printer.cpp -o message_printer
chmod 755 message_printer
```

## Compartmentalising

main.cpp:
```cpp
#include "message.h"
int main(){
    print_message();
    return 0;
}
```

message.h:
```cpp
#ifndef __MESSAGE__
#define __MESSAGE__
#include <iostream>
void print_message();
#endif
```

message.cpp:
```cpp
#include "message.h"
void print_message(){
    std::cout << "Hello world" << std::endl;
}
```

## Makefile
```makefile
CC=clang++

message_printer : main.o message.o
	$(CC) main.o message.o -o message_printer


main.o : main.cpp message.cpp message.h
	$(CC) -c main.cpp -o main.o
    
mesage.o: message.cpp message.h
	$(CC) -c message.cpp -o message.o
	
.phony : purge

.PHONY : purge clean

clean:
	rm *.o
	
purge:
	- rm *.o
	- rm message_printer
```
## Summary 

In projects as small as this, the usefulness is not apparent.  When you get to very large code-bases which take tens of minutes or hours to compile, it becomes efficient to only rebuild the necessary components, rather than the everything.
