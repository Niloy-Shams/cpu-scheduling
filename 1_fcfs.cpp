#include<bits/stdc++.h>
using namespace std;

struct Process {
    int id;
    int burst_time; // time taken by process to complete on CPU
    int arrival_time;   // time at which process arrives in the ready queue
    int start_time; // time at which process starts executing on CPU
    int completion_time;    // time at which process completes execution on CPU
    int waiting_time;   // turnaround_time - burst_time
    int turnaround_time;    // completion_time - arrival_time
};

bool compareArrival(Process p1, Process p2) {
    if(p1.arrival_time == p2.arrival_time) {
        if(p1.burst_time == p2.burst_time) {
            return p1.id < p2.id;
        }
        return p1.burst_time < p2.burst_time;
    }
    return p1.arrival_time < p2.arrival_time;
}

int32_t main(){

    int n;
    cout << "Number of process, n: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        cout<<"\n";
        processes[i].id = i + 1;
        cout << "Enter the burst time of P" << i + 1 << ": ";
        cin >> processes[i].burst_time;
        cout << "Enter the arrival time of P" << i + 1 << ": ";
        cin >> processes[i].arrival_time;
    }

    sort(processes.begin(), processes.end(), compareArrival);

    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        processes[i].start_time = current_time;
        processes[i].completion_time = current_time + processes[i].burst_time;
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
        
        current_time = processes[i].completion_time;
    }

    cout << "\nGantt Chart\n";
    current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            cout << current_time << " Idle ";
            current_time = processes[i].arrival_time;
        }
        cout << current_time << " P" << processes[i].id << " ";
        current_time += processes[i].burst_time;
    }
    cout << current_time << "\n";

    for (int i = 0; i < n; i++) {
        cout << "Process: P" << processes[i].id << " Start time: " << processes[i].start_time 
             << " Waiting time: " << processes[i].waiting_time 
             << " Turnaround time: " << processes[i].turnaround_time << "\n";
    }

    double total_waiting_time = 0, total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    printf("Average waiting time : %.2f\n", total_waiting_time / n);
    printf("Average turnaround time : %.2f\n", total_turnaround_time / n);

    return 0;
}