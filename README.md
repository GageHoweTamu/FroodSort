# FroodSort
FroodSort is an adaptive sorting algorithm that automatically selects between various sorting strategies based on input characteristics:

* O(n) counting sort for dense integer ranges
* Best-case O(n) insertion sort for small arrays and nearly-sorted data
* O(n) radix sort for floats or very large ranges of integers
* O(n log n) std::sort as fallback
* O(1) algorithm selection overhead

This algorithm is optimized for integer arrays with small ranges relative to size, nearly-sorted sequences, quirks of various datatypes, and modern CPU architectures. It's still a work in progress as I'm not familiar with library development and type gymnastics in c++. Also, there's probably a better implementation out there I haven't seen; this is mostly for my learning purposes anyway.

## Benchmarks

Compiled with `g++ -O3 -Wall .\froodsort.cpp`, the current version outputs something similar to this:

```
Nearly sorted array of 10000000 ints:

Choosing counting sort
Adaptive sort: 300.75ms
std::sort:    1498.94ms

Small range array of 10000000 ints, range 1-100):

Choosing counting sort
Adaptive sort: 209.65ms
std::sort:    778.12ms

Random array of 10000000 ints, range 1-1M):

Choosing counting sort
Adaptive sort: 264.94ms
std::sort:    2601.44ms

BigType1 array of 100000 elements

Falling back to std::sort
Adaptive sort: 42.22ms
std::sort:    36.35ms
1 1 1 1 1

Nearly sorted array of 10000000 floats

Falling back to std::sort
Adaptive sort: 1411.46ms
std::sort:    1336.72ms

Large array with wide range (20000000 elements, range 0 to 1073741823):

Choosing radix sort
Adaptive sort: 2788.92ms
std::sort:    6291.99ms
First few elements after sorting: 43 106 290 308 324 
```

* These benchmarks seem too good to be true, more testing is required.

* Large datatypes are pretty slow and fall back to std::sort, need to look into this.
