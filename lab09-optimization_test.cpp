#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int ARRAY_SIZE = 10000000;


void compute(vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = (arr[i] * 3 + 5) % 100000;
    }
}

int main() {
    vector<int> arr(ARRAY_SIZE, 1); 

    auto start = high_resolution_clock::now();
    compute(arr);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Execution Time: " << duration.count() << " ms" << endl;

    return 0;
}
