#include "myqueue.h"

void initQueue(Queue* queue, int (*compare)(Process*, Process*)) {
    queue->size = 0;
    queue->come_time = 0;
    queue->compare = compare;
}

void initMultiLevelQueue(MultiLevelQueue* mlq) {
    initQueue(&mlq->queues[0], compare_priority);   // 편의상 1~3레벨이라 안 쓸 예정
    initQueue(&mlq->queues[1], compare_priority);   // 가장 높은 레벨: Priority
    initQueue(&mlq->queues[2], compare_cpu_remaining_time); // 중간 레벨: SJF
    initQueue(&mlq->queues[3], compare_come_time);  // 가장 낮은 레벨: FCFS
    mlq->current_level = 1;
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
    
    process->come_time = queue->come_time++;
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

void updateQueue(Queue* ready_queue, Queue* waiting_queue, Process* processes, GanttChart* chart, int process_count, int timer) {
    // 새로 도착한 프로세스
    for (int i = 0; i < process_count; i++) {
        if (processes[i].arrival_time == timer && processes[i].cpu_remaining_time > 0) {
            processes[i].ready_time = timer;
            enqueue(ready_queue, &processes[i]);
            addBlock(chart, processes[i].pid, timer, timer, READY_QUEUE, ARRIVE);
        }
    }

    // IO 완료된 프로세스
    if (!isQueueEmpty(waiting_queue)) {
        while(waiting_queue->size > 0 && 
              waiting_queue->process[0]->io_remaining_time[waiting_queue->process[0]->current_cpu_burst_time] <= 0) {
            Process* temp = dequeue(waiting_queue);
            addBlock(chart, temp->pid, timer - temp->io_burst_time[temp->current_cpu_burst_time], timer, WAITING_QUEUE, IO_COMPLETE);
            temp->ready_time = timer;
            enqueue(ready_queue, temp);
        }
    }
}

void updateMLQ(MultiLevelQueue* mlq, Queue* waiting_queue, Process* processes, int process_count, int timer) {
    // 새로 도착한 프로세스
    for (int i = 0; i < process_count; i++) {
        if (processes[i].arrival_time == timer && processes[i].cpu_remaining_time > 0) {
            processes[i].ready_time = timer;
            if (processes[i].level == 1) {enqueue(&mlq->queues[1], &processes[i]); } 
            else if (processes[i].level == 2) {enqueue(&mlq->queues[2], &processes[i]); } 
            else if (processes[i].level == 3) {enqueue(&mlq->queues[3], &processes[i]); }
        }
    }

    // IO 완료된 프로세스
    if (!isQueueEmpty(waiting_queue)) {
        while(waiting_queue->size > 0 && 
              waiting_queue->process[0]->io_remaining_time[waiting_queue->process[0]->current_cpu_burst_time] <= 0) {
            Process* temp = dequeue(waiting_queue);
            temp->ready_time = timer;

            if (temp->level == 1) {enqueue(&mlq->queues[1], temp); } 
            else if (temp->level == 2) {enqueue(&mlq->queues[2], temp); } 
            else if (temp->level == 3) {enqueue(&mlq->queues[3], temp); }
        }
    }
}

void agingQueue(Queue* ready_queue, int timer) {
    for (int i = 0; i < ready_queue->size; i++) {
        // 10초 이상 대기한 프로세스 우선순위 증가
        if (ready_queue->process[i]->come_time < timer - 10) {
            ready_queue->process[i]->dynamic_priority--;
        }
    }
    heapify(ready_queue);
}

void FeedbackMLQ(MultiLevelQueue* mlq, int timer) {
    // 2레벨에서 1레벨로 승격
    for (int i = 0; i < mlq->queues[2].size; ) {
        Process* temp = mlq->queues[2].process[i];
        // 10초 이상 대기한 프로세스 우선순위 증가
        if (temp->come_time < timer - 10) {
            temp->level--;
            enqueue(&mlq->queues[1], temp);
            mlq->queues[2].process[i] = mlq->queues[2].process[mlq->queues[2].size - 1];
            mlq->queues[2].size--;
            heapify(&mlq->queues[2]);
        } else {
            i++;
        }
    }

    // 3레벨에서 2레벨로 승격
    for (int i = 0; i < mlq->queues[3].size; i++) {
        Process* temp = mlq->queues[3].process[i];
        // 10초 이상 대기한 프로세스 우선순위 증가
        if (temp->come_time < timer - 10) {
            temp->level--;
            enqueue(&mlq->queues[2], temp);
            mlq->queues[3].process[i] = mlq->queues[3].process[mlq->queues[3].size - 1];
            mlq->queues[3].size--;
            heapify(&mlq->queues[3]);
        } else {
            i++;
        }
    }
}

void ioOperation(Queue* waiting_queue) {
    if (!isQueueEmpty(waiting_queue)){
        for (int i = 0; i < waiting_queue->size; i++) {
            waiting_queue->process[i]->io_remaining_time[waiting_queue->process[i]->current_cpu_burst_time]--;
            waiting_queue->process[i]->total_io_remaining_time--;
        }
    }
}

void config(Queue* ready_queue, Queue* waiting_queue, int (*compare)(Process*, Process*)){
    initQueue(ready_queue, compare);
    initQueue(waiting_queue, compare_io_remaining_time);
}

bool isQueueEmpty(Queue* queue){return queue->size == 0;}
bool isQueueFull(Queue* queue){return queue->size == MAX_QUEUE_SIZE;}

int compare_come_time(Process* a, Process* b) {return a->come_time - b->come_time;} // FCFS용
int compare_cpu_remaining_time(Process* a, Process* b) {
    if (a->cpu_remaining_time == b->cpu_remaining_time) {
        return a->come_time - b->come_time; // 힙 정렬의 stability 보장
    }
    return a->cpu_remaining_time - b->cpu_remaining_time;
}  // SJF용
int compare_priority(Process* a, Process* b) {
    if (a->priority == b->priority) {
        return a->come_time - b->come_time; // 힙 정렬의 stability 보장
    }
    return a->priority - b->priority;
}    // Priority용
int compare_io_remaining_time(Process* a, Process* b) {
    if (a->io_remaining_time[a->current_cpu_burst_time] == b->io_remaining_time[b->current_cpu_burst_time]) {
        return a->come_time - b->come_time; // 힙 정렬의 stability 보장
    }
    return a->io_remaining_time[a->current_cpu_burst_time] - b->io_remaining_time[b->current_cpu_burst_time];
} // waiting 큐용
int compare_io_count(Process* a, Process* b) {
    if (a->io_count == b->io_count) {
        return a->come_time - b->come_time; // 힙 정렬의 stability 보장
    }
    return b->io_count - a->io_count;
}  // I/O 요청 개수용
int compare_dynamic_priority(Process* a, Process* b) {
    if (a->dynamic_priority == b->dynamic_priority) {
        return a->come_time - b->come_time; // 힙 정렬의 stability 보장
    }
    return a->dynamic_priority - b->dynamic_priority;
}  // 동적 우선순위용
int compare_total_io_remaining_time(Process* a, Process* b) {
    if (a->total_io_remaining_time == b->total_io_remaining_time) {
        return a->come_time - b->come_time; // 힙 정렬의 stability 보장
    }
    return a->total_io_remaining_time - b->total_io_remaining_time;
}  // 총 I/O remaining time용
int compare_hrrn(Process* a, Process* b){
    double rr_a = (double)a->ready_time / a->cpu_remaining_time;
    double rr_b = (double)b->ready_time / b->cpu_remaining_time;
    double diff = rr_a - rr_b;
    if (diff < 0.000001 && diff > -0.000001){
        return a->come_time - b->come_time; // 힙 정렬의 stability 보장
    }
    else if(diff < 0){
        return -1; // a가 더 우선
    }
    else{
        return 1; // b가 더 우선
    }
}  // HRRN용