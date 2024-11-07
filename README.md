# FroodSort
FroodSort is an adaptive sorting algorithm that automatically selects between various sorting strategies based on input characteristics:

* O(n) counting sort for dense integer ranges
* O(n) insertion sort for nearly-sorted data
* O(n log n) quicksort as fallback
* O(1) algorithm selection overhead

Optimized for:

* Integer arrays with small ranges relative to size
* Nearly-sorted sequences
* Modern CPU architectures
