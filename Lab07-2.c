#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

struct Job {
    int id;
    int priority;
    int burst;
    int done = 0;
    int turnaround = 0;
    int wait = 0;
};

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Job> jobs(n);
    cout << "Enter Priority and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        jobs[i].id = i + 1;
        cout << "P" << jobs[i].id << " (Priority, Burst Time): ";
        cin >> jobs[i].priority >> jobs[i].burst;
    }

    int time = 0, completed = 0;
    vector<int> execution_order;

    while (completed < n) {
        int min_pri = INT_MAX;
        int selected = -1;

        for (int i = 0; i < n; i++) {
            if (!jobs[i].done && jobs[i].priority < min_pri) {
                min_pri = jobs[i].priority;
                selected = i;
            }
        }

        if (selected != -1) {
            execution_order.push_back(jobs[selected].id);
            time += jobs[selected].burst;
            jobs[selected].turnaround = time;
            jobs[selected].wait = jobs[selected].turnaround - jobs[selected].burst;
            jobs[selected].done = 1;
            completed++;
        }
    }

    cout << "\nExecution Order: ";
    for (int id : execution_order) {
        cout << "P" << id << " ";
    }
    cout << endl;

    cout << "\nPID\tPriority\tTAT\tWT\n";
    float avg_wt = 0;
    for (const auto &job : jobs) {
        cout << "P" << job.id << "\t" << job.priority << "\t\t" << job.turnaround << "\t" << job.wait << endl;
        avg_wt += job.wait;
    }

    cout << "Average Waiting Time: " << avg_wt / n << " ms\n";

    return 0;
}
