#ifndef MYQUEUE_H
#define MYQUEUE_H

#define MAX_QUEUE_SIZE 20
#define MAX_QUEUE_LEVEL 4   // 편의를 위해 1~3레벨만 사용(0레벨 사용 안 함)

#include "myprocess.h"
#include "mygantt.h"

typedef struct{
    Process* process[MAX_QUEUE_SIZE];
    int size;
    long long int come_time;
    int (*compare)(Process*, Process*);
} Queue;

typedef struct{
    Queue queues[MAX_QUEUE_LEVEL];
    int current_level;
} MultiLevelQueue;

void initQueue(Queue* queue, int (*compare)(Process*, Process*));
void initMultiLevelQueue(MultiLevelQueue* mlq);
void heapify(Queue* queue);
void enqueue(Queue* queue, Process* process);
Process* dequeue(Queue* queue);
void updateQueue(Queue* ready_queue, Queue* waiting_queue, 
    Process* processes, GanttChart* chart, int process_count, int timer);
void updateMLQ(MultiLevelQueue* mlq, Queue* waiting_queue, 
    Process* processes, int process_count, int timer);
void agingQueue(Queue* ready_queue, int timer);
void FeedbackMLQ(MultiLevelQueue* mlq, int timer);
void ioOperation(Queue* waiting_queue);
void config(Queue* ready_queue, Queue* waiting_queue, int (*compare)(Process*, Process*));

bool isQueueEmpty(Queue* queue);
bool isQueueFull(Queue* queue);
int compare_cpu_remaining_time(Process* a, Process* b);
int compare_priority(Process* a, Process* b);
int compare_come_time(Process* a, Process* b);
int compare_io_remaining_time(Process* a, Process* b);
int compare_io_count(Process* a, Process* b);
int compare_dynamic_priority(Process* a, Process* b);
int compare_total_io_remaining_time(Process* a, Process* b);
int compare_hrrn(Process* a, Process* b);

#endif