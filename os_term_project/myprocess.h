#ifndef MYPROCESS_H
#define MYPROCESS_H

#define MAX_PROCESS 30
#define MAX_IO_COUNT 5

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct{
    int pid;
    int arrival_time;

    int cpu_burst_time;
    int cpu_remaining_time;

    int io_count;
    int io_burst_time;
    int io_remaining_time;
    bool* io_request_time;

    int waiting_time;
    int turnaround_time;

    int ready_time; // Ready Queue에 들어간 시간
    long long int come_time; // Ready Queue에 들어간 시간

    int priority;
} Process;

Process make_process(int pid);
void reset_processes(Process processes[], int process_count);
int make_dummy_processes(Process processes[]);

#endif