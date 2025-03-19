#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;


const int NUM_OPERATIONS = 100000000;


void performComputation(int thread_id, int num_threads) {
    long long sum = 0;
    for (int i = thread_id; i < NUM_OPERATIONS; i += num_threads) {
        sum += i % 100;  
    }
}


void runMultiThreaded(int num_threads) {
    vector<thread> threads;
    
    auto start = high_resolution_clock::now();  

    
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(performComputation, i, num_threads);
    }

    
    for (auto& t : threads) {
        t.join();
    }

    auto stop = high_resolution_clock::now();  
    auto duration = duration_cast<milliseconds>(stop - start);
    
    cout << "Threads: " << num_threads << " - Execution Time: " << duration.count() << " ms" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <num_threads>" << endl;
        return 1;
    }

    int num_threads = stoi(argv[1]);  
    runMultiThreaded(num_threads);

    return 0;
}
