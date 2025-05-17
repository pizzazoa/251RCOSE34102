#ifndef MYQUEUE_H
#define MYQUEUE_H

#define MAX_QUEUE_SIZE 40

#include "myprocess.h"

typedef struct{
    Process* process[MAX_QUEUE_SIZE];
    int size;
    long long int come_time;
    int (*compare)(Process*, Process*);
} Queue;

void initQueue(Queue* queue, int (*compare)(Process*, Process*));
void heapify(Queue* queue);
void enqueue(Queue* queue, Process* process);
Process* dequeue(Queue* queue);
void updateQueue(Queue* ready_queue, Queue* waiting_queue, 
    Process* processes, int process_count, int timer);
void agingQueue(Queue* ready_queue, int timer);
void ioOperation(Queue* waiting_queue);
void config(Queue* ready_queue, Queue* waiting_queue, int (*compare)(Process*, Process*));

bool isQueueEmpty(Queue* queue);
bool isQueueFull(Queue* queue);
int compare_cpu_remaining_time(Process* a, Process* b);
int compare_priority(Process* a, Process* b);
int compare_come_time(Process* a, Process* b);
int compare_io_remaining_time(Process* a, Process* b);
int compare_deadline(Process* a, Process* b);
int compare_io_count(Process* a, Process* b);


#endif