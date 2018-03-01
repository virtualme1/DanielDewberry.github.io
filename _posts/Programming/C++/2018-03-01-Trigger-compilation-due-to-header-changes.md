---
layout: default
title: Triggering compilation of source files due to header changes
meta: Sometimes we wish to recompile source files when their respective header has changed.  Let's discuss a couple of ways to do so. 
categories: [programming,cpp,make,makefiles]
---

# Triggering compilation of source files due to header changes

Sometimes we wish to include headerfiles in the compilation of source files, sometimes we don't!

Consider the following exaxmple:

Our source code has compiled successfully.  Now we add further function declarations and documentation to the header files and perhaps change a small number of the source files. Compiling everything at this stage does not make sense, as only the source updates require compilation, not header-only updates.

When function (or class) prototypes are changed, they will need to be forward declared (by including the `.h` file) in the source file to meet any interdependence that may exist.


Let's investigate two methods for managing these situations from one [`makefile`](https://www.gnu.org/software/make/). 



## Method one: Clean/ purge
Create a makefile as such:

`makefile`:
```makefile
{% include Examples/cpp/trigger/makefile_1 %}
```

* Invoke `$ make` to update the object files and executable based on source file changes
* Invoke `$ make clean; make` to to update the object files with function and class prototypes

(`$ make purge` could be used instead of `$ make clean` to remove the executable as well.)



This method is wastful when you have multiple source files and only a few need to be recompiled due to the header updates.

## Method two: Makefile conditionals

Let us create a `TRIGGER` variable in the source file and use an ifeq statement to compare the value of it to the status `on`.
In the conditional statement, we define `message_H` and assign it either the value `message.h` or nothing depending on the outcome of `ifeq`.

We then add `$(message_h)` to any dependency line which would be dependent upon `message.h` to use the variable.
 
`makefile`:
```makefile
{% include Examples/cpp/trigger/makefile_2 %}
```

By replacing `TRIGGER=on` with `TRIGGER=off` we can switch this functionality on or off!
