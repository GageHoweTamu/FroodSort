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
Nearly sorted array (100000000 elements):
Adaptive sort: 583.46ms
std::sort:    1829.45ms
Adaptive sort - correctly sorted: Yes
First few elements: 0 1 2 3 4
std::sort - correctly sorted: Yes
First few elements: 0 1 2 3 4

Small range array (100000000 elements, range 1-100):
Adaptive sort: 138.11ms
std::sort:    2625.31ms
Adaptive sort - correctly sorted: Yes
First few elements: 1 1 1 1 1
std::sort - correctly sorted: Yes
First few elements: 1 1 1 1 1

Random array (100000000 elements, range 1-1M):
Adaptive sort: 179.10ms
std::sort:    5352.72ms
Adaptive sort - correctly sorted: Yes
First few elements: 1 1 1 1 1
std::sort - correctly sorted: Yes
First few elements: 1 1 1 1 1
```

These benchmarks seem too good to be true, I'm only using integers though. More testing is required.
