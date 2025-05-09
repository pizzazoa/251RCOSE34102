#ifndef MYEVALUATOR_H
#define MYEVALUATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "myprocess.h"

void evaluator(Process* processes, int process_count);
void record_min_avg_waiting_time(Process* processes, int process_count, 
    float* min_avg_waiting_time, int* flag, int _flag);
void record_min_avg_turnaround_time(Process* processes, int process_count, 
    float* min_avg_turnaround_time, int* flag, int _flag);
void evaluator_all(Process* processes, int process_count, 
    float* min_avg_waiting_time, float* min_avg_turnaround_time, int* flag, int _flag);

#endif