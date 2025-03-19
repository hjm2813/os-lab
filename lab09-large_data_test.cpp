#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;


const int SMALL_SIZE = 1000;
const int MEDIUM_SIZE = 100000;
const int LARGE_SIZE = 10000000;


vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
    return arr;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int i = 0; i < n2; i++) R[i] = arr[m + 1 + i];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}


void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}


void measureSortingTime(void (*sortFunc)(vector<int>&, int, int), vector<int> arr, string name) {
    auto start = high_resolution_clock::now();
    sortFunc(arr, 0, arr.size() - 1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << name << " Time: " << duration.count() << " ms" << endl;
}

void measureSortingTime(void (*sortFunc)(vector<int>&), vector<int> arr, string name) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << name << " Time: " << duration.count() << " ms" << endl;
}

int main() {
    vector<int> small = generateRandomArray(SMALL_SIZE);
    vector<int> medium = generateRandomArray(MEDIUM_SIZE);
    vector<int> large = generateRandomArray(LARGE_SIZE);

    cout << "Small Dataset: " << endl;
    measureSortingTime(insertionSort, small, "Insertion Sort");
    measureSortingTime(quickSort, small, "Quick Sort");
    measureSortingTime(mergeSort, small, "Merge Sort");

    cout << "\nMedium Dataset: " << endl;
    measureSortingTime(quickSort, medium, "Quick Sort");
    measureSortingTime(mergeSort, medium, "Merge Sort");

    cout << "\nLarge Dataset: " << endl;
    measureSortingTime(quickSort, large, "Quick Sort");
    measureSortingTime(mergeSort, large, "Merge Sort");

    return 0;
}
