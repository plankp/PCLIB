# PCLIB

Plankp's C library

## What does this library provide

I planned on making this a library that focuses on data structures.
It might (most likely will) not be efficient, but it should be easy to use.

If you need any of the following, this is the right place for you:

*  String buffers
*  Array lists
*  Hash maps
*  Bit arrays
*  Binary tree (set, multiset, map, multimap)
*  Linked list (single and double)
*  Optional type (null wrapper)
*  Closures (including consumers, predicates, and suppliers)
*  Named param function

## How to build this library

Not tested on windows. CMake and a C99 compiler is needed.

```
mkdir build
cd build
cmake ..
make
```

This will create a library called `libpclib.a` which can be linked against your application.

------

Released under the LGPL3 license
