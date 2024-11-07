# FroodSort
FroodSort is an adaptive sorting algorithm that automatically selects between various sorting strategies based on input characteristics:

* O(n) counting sort for dense integer ranges
* O(n) insertion sort for nearly-sorted data
* O(n log n) quicksort as fallback
* O(1) algorithm selection overhead

This algorithm is optimized for integer arrays with small ranges relative to size, nearly-sorted sequences, quirks of various datatypes, and modern CPU architectures. It's still a work in progress as I'm not familiar with library development and type gymnastics in c++. Also, there's probably a better implementation out there I haven't seen; this is mostly for my learning purposes anyway.

## Benchmarks

Compiled with `g++ -O3 -Wall .\froodsort.cpp`, the current version outputs something similar to this:

```
Nearly sorted array of 10000000 ints:

Choosing counting sort
Adaptive sort: 314.20ms
std::sort:    1373.37ms

Small range array of 10000000 ints, range 1-100):

Choosing counting sort
Adaptive sort: 229.69ms
std::sort:    797.58ms

Random array of 10000000 ints, range 1-1M):

Choosing counting sort
Adaptive sort: 278.06ms
std::sort:    2654.74ms

BigType1 array of100000 elements

Falling back to std::sort
Adaptive sort: 31.42ms
std::sort:    31.17ms
1 1 1 1 1

Nearly sorted array of 10000000 floats

Falling back to std::sort
Adaptive sort: 1293.19ms
std::sort:    1282.04ms
```

* These benchmarks seem too good to be true, I'm only using integers though. More testing is required.

* Large datatypes are extremely slow, need to look into this.
