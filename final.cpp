#include<bits/stdc++.h>
using namespace std;

struct Process {
    int id;
    int priority;   // for priority sceduling
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

// 1. fcfs
bool compareArrival(Process p1, Process p2) {
    if(p1.arrival_time == p2.arrival_time) {
        if(p1.burst_time == p2.burst_time) {
            return p1.id < p2.id;
        }
        return p1.burst_time < p2.burst_time;
    }
    return p1.arrival_time < p2.arrival_time;
}

void fcfs(vector<Process> processes, bool detail){
    sort(processes.begin(), processes.end(), compareArrival);

    int current_time = 0;
    int n = processes.size();
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

    if(detail) cout << "\nGantt Chart\n";
    current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            if(detail) cout << current_time << " Idle ";
            current_time = processes[i].arrival_time;
        }
        if(detail) cout << current_time << " P" << processes[i].id << " ";
        current_time += processes[i].burst_time;
    }
    if(detail){   
        cout << current_time << "\n";

        for (int i = 0; i < n; i++) {
            cout << "Process: P" << processes[i].id << " Start time: " << processes[i].start_time 
                << " Waiting time: " << processes[i].waiting_time 
                << " Turnaround time: " << processes[i].turnaround_time << "\n";
        }
    }

    double total_waiting_time = 0, total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    printf("Average response time : %.2f\n", total_waiting_time / n);
    printf("Average waiting time : %.2f\n", total_waiting_time / n);
    printf("Average turnaround time : %.2f\n", total_turnaround_time / n);
}

// 2. sjf
void sjf(vector<Process> processes, bool detail){
    int n = processes.size();

    int current_time = 0; // current time in the system
    int completed = 0; // number of processes completed

    // min heap to store processes based on burst time
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> ready_queue; 
    vector<int> execution_order;

    if(detail) cout<<"Gantt Chart: \n";
    if(detail) cout<<"0 ";
    while(completed!=processes.size()){
        // Add processes to the ready queue that have arrived
        int least_arrival_time = INT_MAX;
        for(int i=0; i<processes.size(); i++){
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

            if(detail) cout<<"P"<<processes[idx].id<<" ";
            processes[idx].start_time = current_time;
            processes[idx].completion_time = current_time+processes[idx].burst_time;
            current_time = processes[idx].completion_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        }
        else{
            current_time = least_arrival_time;
            if(detail) cout<<"Idle ";
        }
        if(detail) cout<<current_time<<" ";
    }

    if(detail) cout<<"\n";
    double waiting_sum=0, turnaround_sum=0;

    for(int i=0; i<processes.size(); i++){
        waiting_sum+=processes[i].waiting_time;
        turnaround_sum+=processes[i].turnaround_time;
        int j = execution_order[i];
        if(detail) cout<<"Process: P"<<processes[j].id
            <<" Start time : "<<processes[j].start_time
            <<" Waiting time: "<<processes[j].waiting_time
            <<" Turnaround time: "<<processes[j].turnaround_time<<"\n";
    }
    cout<<"Average response time: "<<waiting_sum/n<<"\n";
    cout<<"Average waiting time: "<<waiting_sum/n<<"\n";
    cout<<"Average turnaround time: "<<turnaround_sum/n<<"\n";       
}

// 3. Preemptive SJF
void preemptive_sjf(vector<Process> processes, bool detail){
    int n = processes.size();
    int current_time = 0; // current time in the system
    int completed = 0; // number of processes completed

    // min heap to store processes based on burst time (arrival time for tie-breaking) {burst_time, arrival_time, index}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> ready_queue; 
    vector<int> execution_order;

    if(detail) cout<<"Gantt Chart: \n";
    if(detail) cout<<"0 ";
    while(completed!=n){
        // Add processes to the ready queue that have arrived
        int least_arrival_time = INT_MAX;
        for(int i=0; i<n; i++){
            if(processes[i].is_ready == false){
                least_arrival_time = min(least_arrival_time, processes[i].arrival_time);
                if(processes[i].arrival_time<=current_time){
                    ready_queue.push({processes[i].remaining_time, processes[i].arrival_time, i});
                    processes[i].is_ready = true;
                }
            }
        }

        // If there are processes in the ready queue,
        if(!ready_queue.empty()){
            int idx = get<2>(ready_queue.top());
            ready_queue.pop();
            
            if(!processes[idx].started){
                processes[idx].start_time = current_time;
                processes[idx].started = true;
                processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
            }
            
            processes[idx].remaining_time--;
            current_time++;
            
            if(processes[idx].remaining_time==0){
                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                completed++;
                execution_order.push_back(idx);
            }
            else{
                ready_queue.push({processes[idx].remaining_time, processes[idx].arrival_time, idx});
            }

            if(detail) cout<<"P"<<processes[idx].id<<" ";
        }
        else{
            current_time = least_arrival_time;
            if(detail) cout<<"Idle ";
        }
        if(detail) cout<<current_time<<" ";
    }

    if(detail) cout<<"\n";
    double waiting_sum=0, turnaround_sum=0, response_sum=0;

    for(int i=0; i<n; i++){
        waiting_sum+=processes[i].waiting_time;
        turnaround_sum+=processes[i].turnaround_time;
        response_sum+=processes[i].response_time;
        int j = execution_order[i];
        if(detail) cout<<"Process: P"<<processes[j].id
            <<" Start time : "<<processes[j].start_time
            <<" End time : "<<processes[j].completion_time
            <<" Response time : "<<processes[j].response_time
            <<" Waiting time: "<<processes[j].waiting_time
            <<" Turnaround time: "<<processes[j].turnaround_time<<"\n";
    }
    cout<<"Average response time: "<<response_sum/n<<"\n";
    cout<<"Average waiting time: "<<waiting_sum/n<<"\n";
    cout<<"Average turnaround time: "<<turnaround_sum/n<<"\n";
}

// 4. Non-Preemptive Priority
void priority(vector<Process> processes, bool detail){
    int n = processes.size();
    int current_time = 0; // current time in the system
    int completed = 0; // number of processes completed

    // min heap to store processes based on burst time 
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> ready_queue; 
    vector<int> execution_order;

    if(detail) cout<<"Gantt Chart: \n";
    if(detail) cout<<"0 ";
    while(completed!=n){
        // Add processes to the ready queue that have arrived
        int least_arrival_time = INT_MAX;
        for(int i=0; i<n; i++){
            if(processes[i].is_ready == false){
                least_arrival_time = min(least_arrival_time, processes[i].arrival_time);
                if(processes[i].arrival_time<=current_time){
                    ready_queue.push({processes[i].priority, i});
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

            if(detail) cout<<"P"<<processes[idx].id<<" ";
            processes[idx].start_time = current_time;
            processes[idx].completion_time = current_time+processes[idx].burst_time;
            current_time = processes[idx].completion_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
        }
        else{
            current_time = least_arrival_time;
            if(detail) cout<<"Idle ";
        }
        if(detail) cout<<current_time<<" ";
    }

    if(detail) cout<<"\n";
    double waiting_sum=0, turnaround_sum=0;

    for(int i=0; i<n; i++){
        waiting_sum+=processes[i].waiting_time;
        turnaround_sum+=processes[i].turnaround_time;
        int j = execution_order[i];
        if(detail) cout<<"Process: P"<<processes[j].id
            <<" Start time : "<<processes[j].start_time
            <<" Waiting time: "<<processes[j].waiting_time
            <<" Turnaround time: "<<processes[j].turnaround_time<<"\n";
    }
    cout<<"Average response time: "<<waiting_sum/n<<"\n";
    cout<<"Average waiting time: "<<waiting_sum/n<<"\n";
    cout<<"Average turnaround time: "<<turnaround_sum/n<<"\n";
}

// 5. Preemptive Priority
void preemptive_priority(vector<Process> processes, bool detail){
    int n = processes.size();
    int current_time = 0; // current time in the system
    int completed = 0; // number of processes completed

    // min heap to store processes based on priority (arrival time for tie-breaking) {priority, arrival_time, index}
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> ready_queue; 
    vector<int> execution_order;

    if(detail) cout<<"Gantt Chart: \n";
    if(detail) cout<<"0 ";
    while(completed!=n){
        // Add processes to the ready queue that have arrived
        int least_arrival_time = INT_MAX;
        for(int i=0; i<n; i++){
            if(processes[i].is_ready == false){
                least_arrival_time = min(least_arrival_time, processes[i].arrival_time);
                if(processes[i].arrival_time<=current_time){
                    ready_queue.push({processes[i].priority, processes[i].arrival_time, i});
                    processes[i].is_ready = true;
                }
            }
        }

        // If there are processes in the ready queue,
        if(!ready_queue.empty()){
            int idx = get<2>(ready_queue.top());
            ready_queue.pop();
            
            if(!processes[idx].started){
                processes[idx].start_time = current_time;
                processes[idx].started = true;
                processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
            }
            
            processes[idx].remaining_time--;
            current_time++;
            
            if(processes[idx].remaining_time==0){
                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                completed++;
                execution_order.push_back(idx);
            }
            else{
                ready_queue.push({processes[idx].priority, processes[idx].arrival_time, idx});
            }

            if(detail) cout<<"P"<<processes[idx].id<<" ";
        }
        else{
            current_time = least_arrival_time;
            if(detail) cout<<"Idle ";
        }
        if(detail) cout<<current_time<<" ";
    }

    if(detail) cout<<"\n";
    double waiting_sum=0, turnaround_sum=0, response_sum=0;
    for(int i=0; i<n; i++){
        waiting_sum+=processes[i].waiting_time;
        turnaround_sum+=processes[i].turnaround_time;
        response_sum+=processes[i].response_time;
        int j = execution_order[i];
        if(detail) cout<<"Process: P"<<processes[j].id
            <<" Start time : "<<processes[j].start_time
            <<" End time : "<<processes[j].completion_time
            <<" Response time : "<<processes[j].response_time
            <<" Waiting time: "<<processes[j].waiting_time
            <<" Turnaround time: "<<processes[j].turnaround_time<<"\n";
    }
    cout<<"Average response time: "<<response_sum/n<<"\n";
    cout<<"Average waiting time: "<<waiting_sum/n<<"\n";
    cout<<"Average turnaround time: "<<turnaround_sum/n<<"\n";
}

// 6. Round Robin
void round_robin(vector<Process> processes, int time_quantum, bool detail){
    int n = processes.size();
    int current_time = 0; // current time in the system
    int completed = 0; // number of processes completed
    int last_process = -1; // to keep track of the last process executed
    queue<int> ready_queue; // queue to store indexes of processes

    if(detail) cout<<"Gantt Chart: \n";
    if(detail) cout<<"0 ";
    while(completed!=n){
        // Add processes to the ready queue that have arrived
        int least_arrival_time = INT_MAX;
        for(int i=0; i<n; i++){
            if(processes[i].is_ready == false){
                least_arrival_time = min(least_arrival_time, processes[i].arrival_time);
                if(processes[i].arrival_time<=current_time){
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

            if(detail) cout<<"P"<<processes[idx].id<<" ";

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
            if(detail) cout<<"Idle ";
            current_time=least_arrival_time;
        }
        if(detail) cout<<current_time<<" ";
    }

    if(detail) cout << "\n";

    double total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;

    if(detail) cout<<"\nProcesss:\n";
    for(int i=0; i<n; i++){
        total_waiting_time+=processes[i].waiting_time;
        total_turnaround_time+=processes[i].turnaround_time;
        total_response_time+=processes[i].response_time;
        
        if(detail) cout<<"Process: P"<<processes[i].id
            <<" Start time: "<<processes[i].start_time
            <<" End time: "<<processes[i].completion_time
            <<" Response time: "<<processes[i].response_time
            <<" Waiting time: "<<processes[i].waiting_time
            <<" Turnaround time: "<<processes[i].turnaround_time<<"\n";
    }   
    cout<<"Average response time: "<<total_response_time/n<<"\n";
    cout<<"Average waiting time: "<<total_waiting_time/n<<"\n";
    cout<<"Average turnaround time: "<<total_turnaround_time/n<<"\n";
}

// 7. Your Own Algorithm
double priority_calculator(Process &p, int current_time, double alpha) {
    int waiting_time = current_time - p.arrival_time;
    return (double)(waiting_time + p.burst_time) / p.burst_time + alpha * waiting_time;
}

void own_algo(vector<Process> processes, double alpha, bool detail){
    int n = processes.size();
    int current_time = 0; // current time in the system
    int completed = 0; // number of processes completed
    int idx = -1; // to keep track of the last process executed

    if(detail) cout<<"Gantt Chart: \n";
    if(detail) cout<<"0 ";

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

            if(detail) cout << "P" << processes[idx].id << " ";

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
            if(detail) cout << "Idle ";
            current_time++;
        }
        if(detail) cout << current_time << " ";
    }
    if(detail) cout << "\n";

    double total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;

    if(detail) cout<<"\nProcess Details:\n";
    for(int i=0; i<n; i++){
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        total_response_time += processes[i].response_time;

        if(detail) cout<<"Process: P"<<processes[i].id
            <<" Start time: "<<processes[i].start_time
            <<" End time: "<<processes[i].completion_time
            <<" Response time: "<<processes[i].response_time
            <<" Waiting time: "<<processes[i].waiting_time
            <<" Turnaround time: "<<processes[i].turnaround_time<<"\n";
    }

    cout<<"Average Response Time: "<<total_response_time/n<<"\n";
    cout<<"Average Waiting Time: "<<total_waiting_time/n<<"\n";
    cout<<"Average Turnaround Time: "<<total_turnaround_time/n<<"\n";
}

int32_t main(){
    int choice;
    do {
        cout << "1: FCFS\n"<<
            "2: Non-Preemptive-SJF\n"<<
            "3: Preemptive-SJF\n"<<
            "4: Non-Preemptive-Priority\n"<<
            "5: Preemptive-Priority\n"<<
            "6: Round-Robin\n"<<
            "7: Your-Own-Algorithm\n"<<
            "8: Compare-All\n"<<
            "9: Exit\n"<<
            "Input your Choice: ";

        cin >> choice;

        int n;

        if (choice==1 || choice==2 || choice==3){
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
                processes[i].remaining_time = processes[i].burst_time;
            }
            if(choice==1) fcfs(processes, true);
            else if(choice==2) sjf(processes, true);
            else if(choice==3) preemptive_sjf(processes, true);
        }
        else if (choice==4 || choice==5){
            cout << "Number of process, n: ";
            cin >> n;

            vector<Process> processes(n);

            for (int i = 0; i < n; i++) {
                cout<<"\n";
                processes[i].id = i + 1;
                cout << "Enter the burst time of P" << i + 1 << ": ";
                cin >> processes[i].burst_time;
                cout << "Enter arrival time of P" << i + 1 << ": ";
                cin >> processes[i].arrival_time;
                cout << "Enter the priority of P" << i + 1 << ": ";
                cin >> processes[i].priority;
                processes[i].remaining_time = processes[i].burst_time;
            }
            if(choice==4) priority(processes, true);
            if(choice==5) preemptive_priority(processes, true);
        }
        if(choice==6){
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

            int time_quantum;
            cout << "Enter Time Quantum: ";
            cin >> time_quantum;
            round_robin(processes, time_quantum, true);
        }
        if(choice==7){
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

            cout << "Alpha controls how much priority is given to waiting time (aging):\n";
            cout << "  - Lower alpha (e.g., 0.05) -> favors short jobs, may reduce average response time.\n";
            cout << "  - Medium alpha (e.g., 0.3 - 0.6) -> balanced approach, may optimize waiting and turnaround time.\n";
            cout << "  - Higher alpha (e.g., >0.7) -> favors long-waiting jobs, helps prevent starvation, but may increase response time.\n";

            double alpha;
            cout << "Enter the value of alpha: ";
            cin >> alpha;

            while(alpha < 0.0 || alpha > 1.0) {
                cout << "Invalid alpha! Please enter a value between 0 and 1: ";
                cin >> alpha;
            }
            own_algo(processes, alpha, true);
        }
        if(choice==8){
            cout << "Number of processes, n: ";
            cin >> n;

            vector<Process> processes(n);

            for (int i = 0; i < n; i++) {
                cout<<"\n";
                processes[i].id = i + 1;
                cout << "Enter the burst time of P" << i + 1 << ": ";
                cin >> processes[i].burst_time;
                cout << "Enter arrival time of P" << i + 1 << ": ";
                cin >> processes[i].arrival_time;
                cout << "Enter the priority of P" << i + 1 << ": ";
                cin >> processes[i].priority;
                processes[i].remaining_time = processes[i].burst_time;
                processes[i].started = false;
                processes[i].is_ready = false;
            }

            int time_quantum;
            cout << "Enter Time Quantum: ";
            cin >> time_quantum;

            cout << "Alpha controls how much priority is given to waiting time (aging):\n";
            cout << "  - Lower alpha (e.g., 0.05) -> favors short jobs, may reduce average response time.\n";
            cout << "  - Medium alpha (e.g., 0.3 - 0.6) -> balanced approach, may optimize waiting and turnaround time.\n";
            cout << "  - Higher alpha (e.g., >0.7) -> favors long-waiting jobs, helps prevent starvation, but may increase response time.\n";

            double alpha;
            cout << "Enter the value of alpha: ";
            cin >> alpha;

            while(alpha < 0.0 || alpha > 1.0) {
                cout << "Invalid alpha! Please enter a value between 0 and 1: ";
                cin >> alpha;
            }

            cout<< "\nAlgo : FCFS\n";
            fcfs(processes, false);
            cout<< "\nAlgo : Non-Preemptive SJF\n";
            sjf(processes, false);
            cout<< "\nAlgo : Preemptive SJF\n";
            preemptive_sjf(processes, false);
            cout<< "\nAlgo : Non-Preemptive Priority\n";
            priority(processes, false);
            cout<< "\nAlgo : Preemptive Priority\n";
            preemptive_priority(processes, false);
            cout<< "\nAlgo : Round Robin\n";
            round_robin(processes, time_quantum, false);
            cout<< "\nAlgo : Your Own Algorithm\n";
            own_algo(processes, alpha, false);
        }
    } while (choice != 9);
    cout<<"Exiting...\n";

    return 0;
}