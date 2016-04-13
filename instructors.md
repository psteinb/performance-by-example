---
layout: page
title: Performance By Example
subtitle: Instructor's Guide
---

## Learner Prerequisites

* writing C code (variables, for loops, functions) 
* writing a C++ class (member variables, member functions)
* using inheritance to related 2 C++ classes
* using STL containers
* (using algorithms and functors)

## Frequently Argued Issues (FAI)

None so far.

## Where to contribute

1. Interlude between 2 and 3 to introduce/recap functors in the STL

2. More Exercises:
    * short write this code from scratch
    * MCQs
    * fill-in-the-blanks
    * Parsons Problems (rearrange lines of given code, so they work)
    * tweak this code

3. Port to other OSes
   * OSX (should be straightforward)
   * Windows (NMake?)

4. Add more chapters (meta programming, vectorisation ...)

5. Add learner pre-workshop assessment evaluation to check their level. 


## Solutions to exercises

Can be found in dedicated unit test files in the `code/` directory of the [repo](https://github.com/psteinb/performance-by-example/tree/gh-pages/code).

To package the code for distribution before a workshop, use the `package` target in `code/Makefile`.

```
$ make -C code/ package
```

## Teaching Methods

Teaching methods used are based on the experiences made within the [software carpentry collaboration](http://software-carpentry.org). To know more about the motivation behind it, see [their recommended reading](http://software-carpentry.org/reading/) or attend their [instructor training](http://software-carpentry.org/join/). Or get in touch with me through [this repository](https://github.com/psteinb/performance-by-example).
