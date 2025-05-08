#include "myqueue.h"

void initQueue(Queue* queue, int (*compare)(Process*, Process*)) {
    queue->size = 0;
    queue->compare = compare;
}

void heapify(Queue* queue) {
    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        int parent = i;
        while (1) {
            int left = 2 * parent + 1;
            int right = 2 * parent + 2;
            int smallest = parent;

            if (left < queue->size && queue->compare(queue->process[left], queue->process[smallest]) < 0) {
                smallest = left;
            }
            if (right < queue->size && queue->compare(queue->process[right], queue->process[smallest]) < 0) {
                smallest = right;
            }

            if (smallest != parent) {
                Process* temp = queue->process[parent];
                queue->process[parent] = queue->process[smallest];
                queue->process[smallest] = temp;
                parent = smallest;
            } else {
                break;
            }
        }
    }
}

void enqueue(Queue* queue, Process* process) {
    if (isQueueFull(queue)) {return;}
    
    queue->process[queue->size] = process;
    queue->size++;
    heapify(queue);
}

Process* dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {return NULL;}
    
    Process* top = queue->process[0];
    queue->process[0] = queue->process[queue->size - 1];
    queue->size--;
    heapify(queue);
    return top;
}

void updateQueue(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count, int timer) {
    // 새로 도착한 프로세스
    for (int i = 0; i < process_count; i++) {
        if (processes[i].arrival_time == timer && processes[i].cpu_remaining_time > 0) {
            processes[i].ready_time = timer;
            enqueue(ready_queue, &processes[i]);
        }
    }

    // IO 완료된 프로세스
    if (!isQueueEmpty(waiting_queue)) {
        for (int i = 0; i < waiting_queue->size; ) {
            if(waiting_queue->process[i]->io_remaining_time <= 0) {
                Process* temp = waiting_queue->process[i];
                temp->io_remaining_time = temp->io_burst_time;
                temp->ready_time = timer;

                waiting_queue->process[i] = waiting_queue->process[waiting_queue->size - 1];
                waiting_queue->size--;
                enqueue(ready_queue, temp);
            }
            else {i++;}
        }
        heapify(waiting_queue);
    }
}

void ioOperation(Queue* waiting_queue) {
    if (!isQueueEmpty(waiting_queue)){
        for (int i = 0; i < waiting_queue->size; i++) {
            waiting_queue->process[i]->io_remaining_time--;
        }
    }
}

void config(Queue* ready_queue, Queue* waiting_queue, int (*compare)(Process*, Process*), Process** running, int* terminated_count, int* timer){
    initQueue(ready_queue, compare);
    initQueue(waiting_queue, compare_io_remaining_time);
    *running = NULL;
    *terminated_count = 0;
    *timer = 0;
}

bool isQueueEmpty(Queue* queue){return queue->size == 0;}
bool isQueueFull(Queue* queue){return queue->size == MAX_QUEUE_SIZE;}
int compare_arrival_time(Process* a, Process* b) {return a->arrival_time - b->arrival_time;}    // IO 때문에 못 씀
int compare_cpu_remaining_time(Process* a, Process* b) {return a->cpu_remaining_time - b->cpu_remaining_time;}  // SJF용
int compare_priority(Process* a, Process* b) {return a->priority - b->priority;}    // Priority용
int compare_ready_time(Process* a, Process* b) {return a->ready_time - b->ready_time;}  // FCFS용
int compare_io_remaining_time(Process* a, Process* b) {return a->io_remaining_time - b->io_remaining_time;} // waiting 큐용