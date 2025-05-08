#include "myscheduler.h"

void non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count,  int (*compare)(Process*, Process*)){
    config(ready_queue, waiting_queue, compare);
    Process* running = NULL;
    int terminated_count = 0;
    int timer = 0;

    while(terminated_count < process_count){
        updateQueue(ready_queue, waiting_queue, processes, process_count, timer);

        // Ready Queue에서 프로세스 가져오기
        if (running == NULL && !isQueueEmpty(ready_queue)) {
            running = dequeue(ready_queue);
            running->waiting_time += timer - running->ready_time;
        }

        // IO 처리
        ioOperation(waiting_queue);

        // 프로세스 실행
        if (running != NULL) {
            running->cpu_remaining_time--;

            // 프로세스 완료 처리
            if (running->cpu_remaining_time == 0) {
                running->turnaround_time = timer + 1 - running->arrival_time;
                terminated_count++;
                printf("process %d, turnaround time: %d, waiting time: %d\n", 
                    running->pid, running->turnaround_time, running->waiting_time);
                running = NULL;
            }
            
            // IO 요청 처리
            else if (running->io_request_time[running->cpu_burst_time - running->cpu_remaining_time]) {
                enqueue(waiting_queue, running);
                running = NULL;
            }
        }

        ++timer;
    }
}

void preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count, int (*compare)(Process*, Process*)){
    config(ready_queue, waiting_queue, compare);
    Process* running = NULL;
    int terminated_count = 0;
    int timer = 0;

    while(terminated_count < process_count){
        updateQueue(ready_queue, waiting_queue, processes, process_count, timer);

        // Ready Queue에서 프로세스 가져오기 (진행 중인 프로세스가 있다면 선점 비교)
        if (!isQueueEmpty(ready_queue)) {
            if(running == NULL) {
                running = dequeue(ready_queue);
                running->waiting_time += timer - running->ready_time;
            } else if (compare(running, ready_queue->process[0]) > 0) {
                Process* temp = running;
                running = dequeue(ready_queue);
                running->waiting_time += timer - running->ready_time;
                temp->ready_time = timer;
                enqueue(ready_queue, temp);
            }
        }

        // IO 처리
        ioOperation(waiting_queue);

        // 프로세스 실행
        if (running != NULL) {
            running->cpu_remaining_time--;

            // 프로세스 완료 처리
            if (running->cpu_remaining_time == 0) {
                running->turnaround_time = timer + 1 - running->arrival_time;
                (terminated_count)++;
                printf("process %d, turnaround time: %d, waiting time: %d\n", 
                    running->pid, running->turnaround_time, running->waiting_time);
                running = NULL;
            }
            
            // IO 요청 처리
            else if (running->io_request_time[running->cpu_burst_time - running->cpu_remaining_time]) {
                enqueue(waiting_queue, running);
                running = NULL;
            }
        }

        ++timer;
    }
}

void FCFS(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    non_preemptive(ready_queue, waiting_queue, processes, process_count, compare_come_time);
}

void SJF_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    non_preemptive(ready_queue, waiting_queue, processes, process_count, compare_cpu_remaining_time);
}

void SJF_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    preemptive(ready_queue, waiting_queue, processes, process_count, compare_cpu_remaining_time);
}

void Priority_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    non_preemptive(ready_queue, waiting_queue, processes, process_count, compare_priority);
}

void Priority_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    preemptive(ready_queue, waiting_queue, processes, process_count, compare_priority);
}

void RR(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    config(ready_queue, waiting_queue, compare_come_time);
    Process* running = NULL;
    int terminated_count = 0;
    int timer = 0;
    int q = TIME_QUANTUM;

    while(terminated_count < process_count){
        updateQueue(ready_queue, waiting_queue, processes, process_count, timer);

        // time out 처리
        if (running != NULL && q == 0) {
            running->ready_time = timer;
            enqueue(ready_queue, running);
            running = NULL;
        }

        // Ready Queue에서 프로세스 가져오기
        if (running == NULL && !isQueueEmpty(ready_queue)) {
            running = dequeue(ready_queue);
            q = TIME_QUANTUM;
            running->waiting_time += timer - running->ready_time;
        }

        // IO 처리
        ioOperation(waiting_queue);

        // 프로세스 실행
        if (running != NULL) {
            running->cpu_remaining_time--;
            q--;

            // 프로세스 완료 처리
            if (running->cpu_remaining_time == 0) {
                running->turnaround_time = timer + 1 - running->arrival_time;
                (terminated_count)++;
                printf("process %d, turnaround time: %d, waiting time: %d\n", 
                    running->pid, running->turnaround_time, running->waiting_time);
                running = NULL;
            }
            
            // IO 요청 처리
            else if (running->io_request_time[running->cpu_burst_time - running->cpu_remaining_time]) {
                enqueue(waiting_queue, running);
                running = NULL;
            }
        }

        ++timer;
    }
}