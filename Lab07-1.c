#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct P {
    int pid;
    int burst;
    int executed = 0;
    int turnaround = 0;
    int wait = 0;
};

int main() {
    int n, q_time;
    
    cout << "Enter the number of processes: ";
    cin >> n;
    cout << "Enter the quantum time: ";
    cin >> q_time;
    
    vector<P> processes(n);
    queue<int> q;  

    cout << "Enter Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        cout << "P" << processes[i].pid << ": ";
        cin >> processes[i].burst;
        q.push(i);
    }
    
    int time = 0;
    vector<int> exec_order;  

    while (!q.empty()) {
        int idx = q.front();
        q.pop();
        
        if (processes[idx].executed < processes[idx].burst) {
            exec_order.push_back(processes[idx].pid);
            
            int exec_time = min(q_time, processes[idx].burst - processes[idx].executed);
            time += exec_time;
            processes[idx].executed += exec_time;

            if (processes[idx].executed == processes[idx].burst) {
                processes[idx].turnaround = time;
                processes[idx].wait = processes[idx].turnaround - processes[idx].burst;
            } else {
                q.push(idx);
            }
        }
    }

    cout << "\nExecution Order: ";
    for (int pid : exec_order) {
        cout << "P" << pid << " ";
    }
    cout << endl;

    cout << "\nPID\tTAT\tWT\n";
    float avg_wt = 0;
    for (const auto &p : processes) {
        cout << "P" << p.pid << "\t" << p.turnaround << "\t" << p.wait << endl;
        avg_wt += p.wait;
    }

    cout << "Average Waiting Time: " << avg_wt / n << " ms\n";

    return 0;
}
