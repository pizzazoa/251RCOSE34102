#ifndef MYSCHEDULER_H
#define MYSCHEDULER_H

#define TIME_QUANTUM 2

#include "myqueue.h"
#include "myevaluator.h"
#include "mygantt.h"
#include <float.h>

void non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count,  int (*compare)(Process*, Process*));
void preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count,  int (*compare)(Process*, Process*));
void FCFS(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void SJF_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void SJF_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void RR(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void Priority_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void Priority_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);

void compare_all(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);

void RR_q(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void EDF_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void EDF_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void IO_count_first_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void IO_count_first_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void aging_priority_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count);
void MLFQ(Queue* waiting_queue, Process* processes, int process_count);

#endif