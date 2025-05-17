#ifndef MYEVALUATOR_H
#define MYEVALUATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "myprocess.h"
#include "mygantt.h"

void evaluator(Process* processes, int process_count);
void record_min_avg_waiting_time(Process* processes, int process_count, 
    float* min_avg_waiting_time, int* flag, int _flag);
void record_min_avg_turnaround_time(Process* processes, int process_count, 
    float* min_avg_turnaround_time, int* flag, int _flag);
void evaluator_all(Process* processes, int process_count, 
    float* min_avg_waiting_time, float* min_avg_turnaround_time, int* flag_wt, int* flag_tt, int _flag);

void calculate_cpu_utilization(GanttChart* chart, int process_count);

#endif