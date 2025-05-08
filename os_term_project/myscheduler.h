#ifndef MYSCHEDULER_H
#define MYSCHEDULER_H

#define TIME_QUANTUM 2

#include "myqueue.h"

void non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count, Process** running, int* terminated_count, int* timer, int (*compare)(Process*, Process*));
void preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count, Process** running, int* terminated_count, int* timer, int (*compare)(Process*, Process*));
void FCFS(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count, Process** running, int* terminated_count, int* timer);
void SJF_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count, Process** running, int* terminated_count, int* timer);
void SJF_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count, Process** running, int* terminated_count, int* timer);
void RR(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count, Process** running, int* terminated_count, int* timer);
void Priority_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count, Process** running, int* terminated_count, int* timer);
void Priority_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count, Process** running, int* terminated_count, int* timer);

#endif