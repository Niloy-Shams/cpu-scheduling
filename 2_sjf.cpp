#include<bits/stdc++.h>
using namespace std;

struct Process {
    int id;
    int burst_time; // time taken by process to complete on CPU
    int arrival_time; // time at which process arrives in the ready queue
    int start_time; // time at which process starts executing on CPU
    int completion_time; // time at which process completes execution on CPU
    int turnaround_time; // completion_time - arrival_time
    int waiting_time; // turnaround_time - burst_time
    bool is_ready;
};

int32_t main(){
    int n;
    cout << "Number of processes, n: ";
    cin >> n;

    vector<Process> processes(n);
    
    // Input burst time and arrival time for each process
    for(int i=0; i<n; i++){
        processes[i].id = i+1;
        cout << "Enter burst time of P" << i+1 << ": ";
        cin >> processes[i].burst_time;
        cout << "Enter arrival time of P" << i+1 << ": ";
        cin >> processes[i].arrival_time;
    }

    int current_time = 0; // current time in the system
    int completed = 0; // number of processes completed

    // min heap to store processes based on burst time
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> ready_queue; 
    vector<int> execution_order;

    cout<<"Gantt Chart: \n";
    cout<<"0 ";
    while(completed!=n){
        // Add processes to the ready queue that have arrived
        int least_arrival_time = INT_MAX;
        for(int i=0; i<n; i++){
            if(processes[i].is_ready == false){
                least_arrival_time = min(least_arrival_time, processes[i].arrival_time);
                if(processes[i].arrival_time<=current_time){
                    ready_queue.push({processes[i].burst_time, i});
                    processes[i].is_ready = true;
                }
            }
        }

        // If there are processes in the ready queue,
        if(!ready_queue.empty()){
            int idx = ready_queue.top().second;
            ready_queue.pop();
            completed++;
            execution_order.push_back(idx);

            cout<<"P"<<processes[idx].id<<" ";
            processes[idx].start_time = current_time;
            processes[idx].completion_time = current_time+processes[idx].burst_time;
            current_time = processes[idx].completion_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        }
        else{
            current_time = least_arrival_time;
            cout<<"Idle ";
        }
        cout<<current_time<<" ";
    }

    cout<<"\n";
    double waiting_sum=0, turnaround_sum=0;

    for(int i=0; i<n; i++){
        waiting_sum+=processes[i].waiting_time;
        turnaround_sum+=processes[i].turnaround_time;
        int j = execution_order[i];
        cout<<"Process: P"<<processes[j].id
            <<" Start time : "<<processes[j].start_time
            <<" Waiting time: "<<processes[j].waiting_time
            <<" Turnaround time: "<<processes[j].turnaround_time<<"\n";
    }
    cout<<"Average waiting time: "<<waiting_sum/n<<"\n";
    cout<<"Average turnaround time: "<<turnaround_sum/n<<"\n";

    return 0;
}