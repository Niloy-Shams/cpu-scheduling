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
    bool started; // to check if process started first time for response time
    bool is_ready; // to check if process is in ready queue
};

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
        processes[i].started = false;
        processes[i].is_ready = false;
    }

    int time_quantum;
    cout << "Enter Time Quantum: ";
    cin >> time_quantum;

    int current_time = 0;
    int completed = 0;
    int last_process = -1; // to keep track of the last process executed
    queue<int> ready_queue; // queue to store indexes of processes
    
    cout<<"Gantt Chart:\n";
    cout<<"0 ";

    while(completed != n){
        int least_arrival_time = INT_MAX;
        // Add newly arrived processes to ready queue
        for(int i=0; i<n; i++){
            if(processes[i].is_ready == false){
                least_arrival_time = min(least_arrival_time, processes[i].arrival_time);
                if(processes[i].arrival_time <= current_time && processes[i].remaining_time > 0){
                    ready_queue.push(i);
                    processes[i].is_ready = true;
                }
            }
        }
        if(last_process != -1){
            // If the last process executed is still in the queue, push it back to the queue
            ready_queue.push(last_process);
        }

        if(!ready_queue.empty()){
            int idx = ready_queue.front();
            ready_queue.pop();

            if(!processes[idx].started){
                processes[idx].start_time = current_time;
                processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
                processes[idx].started = true;
            }

            int exec_time = min(time_quantum, processes[idx].remaining_time);

            cout << "P" << processes[idx].id << " ";

            current_time += exec_time;
            processes[idx].remaining_time -= exec_time;

            if(processes[idx].remaining_time == 0){
                completed++;
                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                last_process = -1;
            }
            else{
                last_process = idx; // update last process executed
            }
        }
        else{
            // CPU is idle
            cout << "Idle ";
            current_time=least_arrival_time;
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
