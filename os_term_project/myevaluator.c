#include "myevaluator.h"

enum {
    FCFS = 1,
    SJF_NON_PREEMPTIVE,
    SJF_PREEMPTIVE,
    RR,
    PRIORITY_NON_PREEMPTIVE,
    PRIORITY_PREEMPTIVE
};

void evaluator(Process* processes, int process_count) {
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < process_count; ++i) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / process_count);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / process_count);
    printf("-------------------------\n");
    reset_processes(processes, process_count);
}

void record_min_avg_waiting_time(Process* processes, int process_count, 
    float* min_avg_waiting_time, int* flag, int _flag) {
    int total_waiting_time = 0;

    for (int i = 0; i < process_count; ++i) {
        total_waiting_time += processes[i].waiting_time;
    }
    
    if(*min_avg_waiting_time > (float)total_waiting_time / process_count){
        *min_avg_waiting_time = (float)total_waiting_time / process_count;
        *flag = _flag;
    }
}

void record_min_avg_turnaround_time(Process* processes, int process_count, 
    float* min_avg_turnaround_time, int* flag, int _flag) {
    int total_turnaround_time = 0;

    for (int i = 0; i < process_count; ++i) {
        total_turnaround_time += processes[i].turnaround_time;
    }
    
    if(*min_avg_turnaround_time > (float)total_turnaround_time / process_count){
        *min_avg_turnaround_time = (float)total_turnaround_time / process_count;
        *flag = _flag;
    }
}

void evaluator_all(Process* processes, int process_count, 
    float* min_avg_waiting_time, float* min_avg_turnaround_time, int* flag, int _flag) {
    record_min_avg_waiting_time(processes, process_count, min_avg_waiting_time, flag, _flag);
    record_min_avg_turnaround_time(processes, process_count, min_avg_turnaround_time, flag, _flag);
    reset_processes(processes, process_count);
}