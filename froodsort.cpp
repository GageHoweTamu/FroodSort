#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <type_traits>  // Add this for type traits


template<typename T>
void quicksort(std::vector<T>& arr, int left, int right) {
    // Use insertion sort for small subarrays
    if (right - left < 16) {
        for (int i = left + 1; i <= right; i++) {
            T key = arr[i];
            int j = i - 1;
            while (j >= left && !(key < arr[j])) {  // Using only < operator
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
        return;
    }
    
    // Median-of-three pivot selection
    int mid = (left + right) / 2;
    if (arr[mid] < arr[left])
        std::swap(arr[left], arr[mid]);
    if (arr[right] < arr[left])
        std::swap(arr[left], arr[right]);
    if (arr[mid] < arr[right])
        std::swap(arr[mid], arr[right]);
        
    T pivot = arr[right];
    int i = left - 1;
    
    // Partition using only < operator
    for (int j = left; j < right; j++) {
        if (!(pivot < arr[j])) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[right]);
    
    int partition = i + 1;
    
    if (partition - left < right - partition) {
        quicksort(arr, left, partition - 1);
        quicksort(arr, partition + 1, right);
    } else {
        quicksort(arr, partition + 1, right);
        quicksort(arr, left, partition - 1);
    }
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

template<typename T>
void insertion_sort(std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        T key = arr[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && !(key < arr[j])) {  // Using only < operator
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

template<typename T>
void adaptive_sort(std::vector<T>& arr) {
    if (arr.size() <= 1) return;
    
    if (arr.size() < 50) {
        std::cout << "Choosing insertion sort for small array\n";
        insertion_sort(arr);
        return;
    }
    
    size_t check_size = std::min<size_t>(100, arr.size() - 1);
    int inversions = 0;
    for (size_t i = 0; i < check_size; i++) {
        if (arr[i + 1] < arr[i]) inversions++;  // Using only < operator
    }
    
    if (inversions < 10) {
        if (arr.size() < 10000) {
            insertion_sort(arr);
        } else {
            std::random_device rd;
            std::mt19937 gen(rd());
            for (size_t i = 0; i < arr.size()/100; i++) {
                size_t idx1 = gen() % arr.size();
                size_t idx2 = gen() % arr.size();
                std::swap(arr[idx1], arr[idx2]);
            }
            quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
        }
        return;
    }
    
    if constexpr (std::is_integral<T>::value) {
        T max_val = *std::max_element(arr.begin(), arr.end());
        if (max_val < static_cast<T>(arr.size() * 10)) {
            std::vector<int> temp(arr.begin(), arr.end());
            counting_sort(temp);
            std::copy(temp.begin(), temp.end(), arr.begin());
            return;
        }
    }
    
    quicksort(arr, 0, static_cast<int>(arr.size()) - 1);
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

template<typename T>
double time_sort(std::vector<T>& arr, bool use_std_sort) {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (use_std_sort) {
        std::sort(arr.begin(), arr.end());
    } else {
        adaptive_sort(arr);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

struct BigType1 {

    std::vector<char> data;
    int number = 0;

    bool operator<(const BigType1& other) const {
        return number < other.number;
    }
    bool operator>(const BigType1& other) const {
        return number > other.number;
    }

    // 1MB * 1024
    BigType1() : data(1024) {}
};

int main() {
    const int SIZE = 10000000;
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
    
    // Test 4: BigType1 array
    {
        const int BIG_SIZE = 100000; // Smaller size due to memory constraints
        std::vector<BigType1> arr4(BIG_SIZE), std_arr4(BIG_SIZE);
        std::uniform_int_distribution<> dis4(1, 100);
        
        // Initialize with random numbers
        for (int i = 0; i < BIG_SIZE; i++) {
            arr4[i].number = std_arr4[i].number = dis4(gen);
        }
        
        double adaptive_time = time_sort(arr4, false);
        double std_time = time_sort(std_arr4, true);
        std::cout << "\nBigType1 array (" << BIG_SIZE << " elements):\n";
        std::cout << "Adaptive sort: " << adaptive_time << "ms\n";
        std::cout << "std::sort:    " << std_time << "ms\n";
        
        // Verify first few elements
        std::cout << "First few numbers from adaptive sort: ";
        for (int i = 0; i < std::min(5, BIG_SIZE); i++) {
            std::cout << arr4[i].number << " ";
        }
        std::cout << "\n";
        
        bool is_sorted = std::is_sorted(arr4.begin(), arr4.end());
        std::cout << "Correctly sorted: " << (is_sorted ? "Yes" : "No") << "\n";
    }

    return 0;
}
