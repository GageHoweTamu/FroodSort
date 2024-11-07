#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>

void quicksort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int pivot = arr[(left + right) / 2];
    int i = left, j = right;
    
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++; j--;
        }
    }
    if (left < j) quicksort(arr, left, j);
    if (i < right) quicksort(arr, i, right);
}

void counting_sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    int max_val = *std::max_element(arr.begin(), arr.end());
    std::vector<int> count(max_val + 1);
    for (int x : arr) count[x]++;
    
    int idx = 0;
    for (int i = 0; i <= max_val; i++)
        for (int j = 0; j < count[i]; j++)
            arr[idx++] = i;
}

void insertion_sort(std::vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void adaptive_sort(std::vector<int>& arr) {
    if (arr.size() <= 1) return;
    
    if (arr.size() < 50) {
        insertion_sort(arr);
        return;
    }
    
    size_t check_size = std::min<size_t>(100, arr.size() - 1);
    int inversions = 0;
    for (size_t i = 0; i < check_size; i++) {
        if (arr[i] > arr[i + 1]) inversions++;
    }
    if (inversions < 10) {
        insertion_sort(arr);
        return;
    }
    
    int max_val = *std::max_element(arr.begin(), arr.end());
    if (max_val < static_cast<int>(arr.size() * 10)) {
        counting_sort(arr);
        return;
    }
    
    quicksort(arr, 0, arr.size() - 1);
}

void verify_sort(const std::vector<int>& arr, const std::string& name) {
    bool is_sorted = std::is_sorted(arr.begin(), arr.end());
    std::cout << name << " - correctly sorted: " << (is_sorted ? "Yes" : "No") << "\n";
    std::cout << "First few elements: ";
    for(int i = 0; i < std::min(5, static_cast<int>(arr.size())); i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

double time_sort(std::vector<int>& arr, bool use_std_sort) {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (use_std_sort) {
        std::sort(arr.begin(), arr.end());
    } else {
        adaptive_sort(arr);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    const int SIZE = 100000000;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Test 1: Nearly sorted array
    {
        std::vector<int> arr1(SIZE), std_arr1(SIZE);
        for (int i = 0; i < SIZE; i++) arr1[i] = std_arr1[i] = i;
        for (int i = 0; i < SIZE/20; i++) {
            int idx1 = gen() % SIZE;
            int idx2 = gen() % SIZE;
            std::swap(arr1[idx1], arr1[idx2]);
            std::swap(std_arr1[idx1], std_arr1[idx2]);
        }
        
        double adaptive_time = time_sort(arr1, false);
        double std_time = time_sort(std_arr1, true);
        
        std::cout << "\nNearly sorted array (" << SIZE << " elements):\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Adaptive sort: " << adaptive_time << "ms\n";
        std::cout << "std::sort:    " << std_time << "ms\n";
        verify_sort(arr1, "Adaptive sort");
        verify_sort(std_arr1, "std::sort");
    }
    
    // Test 2: Small range array
    {
        std::vector<int> arr2(SIZE), std_arr2(SIZE);
        std::uniform_int_distribution<> dis2(1, 100);
        for (int i = 0; i < SIZE; i++) 
            arr2[i] = std_arr2[i] = dis2(gen);
        
        double adaptive_time = time_sort(arr2, false);
        double std_time = time_sort(std_arr2, true);
        
        std::cout << "\nSmall range array (" << SIZE << " elements, range 1-100):\n";
        std::cout << "Adaptive sort: " << adaptive_time << "ms\n";
        std::cout << "std::sort:    " << std_time << "ms\n";
        verify_sort(arr2, "Adaptive sort");
        verify_sort(std_arr2, "std::sort");
    }
    
    // Test 3: Random array
    {
        std::vector<int> arr3(SIZE), std_arr3(SIZE);
        std::uniform_int_distribution<> dis3(1, 1000000);
        for (int i = 0; i < SIZE; i++) 
            arr3[i] = std_arr3[i] = dis3(gen);
        
        double adaptive_time = time_sort(arr3, false);
        double std_time = time_sort(std_arr3, true);
        
        std::cout << "\nRandom array (" << SIZE << " elements, range 1-1M):\n";
        std::cout << "Adaptive sort: " << adaptive_time << "ms\n";
        std::cout << "std::sort:    " << std_time << "ms\n";
        verify_sort(arr3, "Adaptive sort");
        verify_sort(std_arr3, "std::sort");
    }
    
    return 0;
}
