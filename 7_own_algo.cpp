#include<bits/stdc++.h>
using namespace std;

struct Process {
    int id;
    int burst_time; // time taken by process to complete on CPU
    int arrival_time;   // time at which process arrives in the ready queue
    int start_time; // time at which process starts executing on CPU
    int completion_time; // time at which process completes execution on CPU
    int turnaround_time;    // completion_time - arrival_time
    int waiting_time;   // turnaround_time - burst_time
    int response_time;  // start_time - arrival_time
    int remaining_time; // preemitive
    bool started = false; // to check if process started first time for response time
    bool is_ready = false; // to check if process is in ready queue
};

double priority_calculator(Process &p, int current_time, double alpha) {
    int waiting_time = current_time - p.arrival_time;
    return (double)(waiting_time + p.burst_time) / p.burst_time + alpha * waiting_time;
}

int32_t main(){
    int n;
    cout << "Number of processes, n: ";
    cin >> n;

    vector<Process> processes(n);

    for(int i=0; i<n; i++){
        processes[i].id = i+1;
        cout << "Enter burst time of P" << i+1 << ": ";
        cin >> processes[i].burst_time;
        cout << "Enter arrival time of P" << i+1 << ": ";
        cin >> processes[i].arrival_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    double alpha = 0.05;
    int current_time = 0;
    int completed = 0;
    int idx = -1; // to keep track of the last process executed
    
    cout<<"Gantt Chart:\n";
    cout<<"0 ";

    while(completed != n){
        double max_priority = - 1.0;
        
        for(int i=0; i<n; i++){
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time) {
                double priority = priority_calculator(processes[i], current_time, alpha);
                if (priority > max_priority) {
                    max_priority = priority;
                    idx = i;
                }
            }
        }


        if(idx != -1){
            Process &p = processes[idx];

            if(!p.is_ready){
                p.is_ready = true;
                p.start_time = current_time;
                p.response_time = p.start_time - p.arrival_time;
            }

            cout << "P" << processes[idx].id << " ";

            current_time++;
            processes[idx].remaining_time--;

            if (p.remaining_time == 0) {
                p.completion_time = current_time;
                p.turnaround_time = p.completion_time - p.arrival_time;
                p.waiting_time = p.turnaround_time - p.burst_time;
                completed++;
            }
        }
        else{
            // CPU is idle
            cout << "Idle ";
            current_time++;
        }
        cout << current_time << " ";
    }

    cout << "\n";

    double total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;

    cout<<"\nProcess Details:\n";
    for(int i=0; i<n; i++){
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_response_time += processes[i].response_time;

        cout<<"Process: P"<<processes[i].id
            <<" Start time: "<<processes[i].start_time
            <<" End time: "<<processes[i].completion_time
            <<" Response time: "<<processes[i].response_time
            <<" Waiting time: "<<processes[i].waiting_time
            <<" Turnaround time: "<<processes[i].turnaround_time<<"\n";
    }

    cout<<"\nAverage Waiting Time: "<<total_waiting_time/n<<"\n";
    cout<<"Average Turnaround Time: "<<total_turnaround_time/n<<"\n";
    cout<<"Average Response Time: "<<total_response_time/n<<"\n";

    return 0;
}
