#include "myevaluator.h"
#include <string.h>

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
    float* min_avg_waiting_time, float* min_avg_turnaround_time, int* flag_wt, int* flag_tt, int _flag) {
    record_min_avg_waiting_time(processes, process_count, min_avg_waiting_time, flag_wt, _flag);
    record_min_avg_turnaround_time(processes, process_count, min_avg_turnaround_time, flag_tt, _flag);
    reset_processes(processes, process_count);
}

void calculate_cpu_utilization(GanttChart* chart, int process_count) {
    if (chart->size == 0) return;
    
    int total_time = chart->blocks[chart->size - 1].end_time;
    int busy_time = 0;
    
    // 간트차트에서 cpu burst time만 계산
    for (int i = 0; i < chart->size; i++) {
        if(strcmp(chart->blocks[i].where, "CPU") == 0){
            if (chart->blocks[i].start_time < chart->blocks[i].end_time) {
                busy_time += chart->blocks[i].end_time - chart->blocks[i].start_time;
            }
        }
    }
    
    float utilization = ((float)busy_time / total_time) * 100;
    printf("CPU Utilization: %.2f%%\n", utilization);
}