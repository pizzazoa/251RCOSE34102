#include "myscheduler.h"

enum {
    FCFS_ = 1,
    SJF_NON_PREEMPTIVE_,
    SJF_PREEMPTIVE_,
    RR_,
    RR_Q_,
    PRIORITY_NON_PREEMPTIVE_,
    PRIORITY_PREEMPTIVE_,
    ICF_NON_PREEMPTIVE_,
    ICF_PREEMPTIVE_,
    AGING_PRIORITY_NON_PREEMPTIVE_,
    MLQ_,
    MLFQ_,
    LIF_NON_PREEMPTIVE_,
    LIF_PREEMPTIVE_,
    HRRN_
};

void print_scheduler(int flag) {
    switch(flag){
        case FCFS_:
            printf("FCFS\n");
            break;
        case SJF_NON_PREEMPTIVE_:
            printf("SJF (non-preemptive)\n");
            break;
        case SJF_PREEMPTIVE_:
            printf("SJF (preemptive)\n");
            break;
        case RR_:
            printf("RR\n");
            break;
        case PRIORITY_NON_PREEMPTIVE_:
            printf("Priority (non-preemptive)\n");
            break;
        case PRIORITY_PREEMPTIVE_:
            printf("Priority (preemptive)\n");
            break;
        case RR_Q_:
            printf("RR (custom time quantum)\n");
            break;
        case ICF_NON_PREEMPTIVE_:
            printf("IO Count First (non-preemptive)\n");
            break;
        case ICF_PREEMPTIVE_:
            printf("IO Count First (preemptive)\n");
            break;
        case AGING_PRIORITY_NON_PREEMPTIVE_:
            printf("Aging Priority (non-preemptive)\n");
            break;
        case MLQ_:
            printf("Multi-Level Queue (MLQ)\n");
            break;
        case MLFQ_:
            printf("MLFQ\n");
            break;
        case LIF_NON_PREEMPTIVE_:
            printf("LIF (non-preemptive)\n");
            break;
        case LIF_PREEMPTIVE_:
            printf("LIF (preemptive)\n");
            break;
        case HRRN_:
            printf("HRRN\n");
            break;
    }
}

void scheduling(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count,  int (*compare)(Process*, Process*), int flag){
    config(ready_queue, waiting_queue, compare);
    Process* running = NULL;
    int terminated_count = 0;
    int timer = 0;

    // 간트차트 초기화
    GanttChart chart;
    initGanttChart(&chart);
    int cpu_start_time = -1;

    while(terminated_count < process_count){
        updateQueue(ready_queue, waiting_queue, processes, &chart, process_count, timer);

        // Ready Queue에서 프로세스 가져오기
        if(flag == NON_PREEMPTIVE_) {
            if (running == NULL && !isQueueEmpty(ready_queue)) {
                running = dequeue(ready_queue);
                running->waiting_time += timer - running->ready_time;
                cpu_start_time = timer;
            }
        } else if (flag == PREEMPTIVE_) {
            if (!isQueueEmpty(ready_queue)) {
                if(running == NULL) {
                    running = dequeue(ready_queue);
                    running->waiting_time += timer - running->ready_time;
                    cpu_start_time = timer;
                } else if (compare(running, ready_queue->process[0]) > 0) {
                    Process* temp = running;
                    addBlock(&chart, running->pid, cpu_start_time, timer, CPU, PREEMPTED);
                    running = dequeue(ready_queue);
                    running->waiting_time += timer - running->ready_time;
                    cpu_start_time = timer;
                    temp->ready_time = timer;
                    enqueue(ready_queue, temp);
                }
            }
        }

        // IO 처리
        ioOperation(waiting_queue);

        // 프로세스 실행
        if (running != NULL) {
            running->cpu_remaining_time--;
            running->current_cpu_burst_time++;

            // 프로세스 완료 처리
            if (running->cpu_remaining_time == 0) {
                running->turnaround_time = timer + 1 - running->arrival_time;
                terminated_count++;
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, TERMINATED);
                running = NULL;
            }
            
            // IO 요청 처리
            else if (running->io_request_time[running->current_cpu_burst_time]) {
                enqueue(waiting_queue, running);
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, IO_REQUEST);
                running = NULL;
            }
        }
        ++timer;
    }
    // 간트차트 출력
    //printGanttChart(&chart, processes, process_count);
    // 간트로그 출력
    printGanttLog(&chart, processes, process_count);
    // process 별로 turnaround time과 waiting time 출력
    for(int i = 0; i < process_count; i++){
        printf("process %d, turnaround time: %d, waiting time: %d\n", 
            processes[i].pid, processes[i].turnaround_time, processes[i].waiting_time);
    }
    // CPU 사용률 계산
    calculate_cpu_utilization(&chart, process_count);
    
}

void FCFS(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(FCFS_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_come_time, NON_PREEMPTIVE_);
}

void SJF_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(SJF_NON_PREEMPTIVE_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_cpu_remaining_time, NON_PREEMPTIVE_);
}

void SJF_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(SJF_PREEMPTIVE_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_cpu_remaining_time, PREEMPTIVE_);
}

void Priority_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(PRIORITY_NON_PREEMPTIVE_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_priority, NON_PREEMPTIVE_);
}

void Priority_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(PRIORITY_PREEMPTIVE_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_priority, PREEMPTIVE_);
}

void RR(Queue* ready_queue, Queue* waiting_queue, Process* processes, 
    int process_count){
    print_scheduler(RR_);
    int time_quantum = 0;
    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);
    if(time_quantum <= 0){
        printf("Invalid time quantum. Using default value of %d.\n", TIME_QUANTUM);
        time_quantum = TIME_QUANTUM;
    }
    config(ready_queue, waiting_queue, compare_come_time);
    Process* running = NULL;
    int terminated_count = 0;
    int timer = 0;
    int q = time_quantum;

    // 간트차트 초기화
    GanttChart chart;
    initGanttChart(&chart);
    int cpu_start_time = -1;

    while(terminated_count < process_count){
        updateQueue(ready_queue, waiting_queue, processes, &chart, process_count, timer);

        // time out 처리
        if (running != NULL && q == 0) {
            running->ready_time = timer;
            enqueue(ready_queue, running);
            addBlock(&chart, running->pid, cpu_start_time, timer, CPU, TIMEOUT);
            running = NULL;
        }

        // Ready Queue에서 프로세스 가져오기
        if (running == NULL && !isQueueEmpty(ready_queue)) {
            running = dequeue(ready_queue);
            q = time_quantum;
            running->waiting_time += timer - running->ready_time;
            cpu_start_time = timer;
        }

        // IO 처리
        ioOperation(waiting_queue);

        // 프로세스 실행
        if (running != NULL) {
            running->cpu_remaining_time--;
            running->current_cpu_burst_time++;
            q--;

            // 프로세스 완료 처리
            if (running->cpu_remaining_time == 0) {
                running->turnaround_time = timer + 1 - running->arrival_time;
                terminated_count++;
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, TERMINATED);
                running = NULL;
            }
            
            // IO 요청 처리
            else if (running->io_request_time[running->current_cpu_burst_time]) {
                enqueue(waiting_queue, running);
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, IO_REQUEST);
                running = NULL;
            }
        }

        ++timer;
    }
    // 간트차트 출력
    //printGanttChart(&chart, processes, process_count);
    // 간트로그 출력
    printGanttLog(&chart, processes, process_count);
    // process 별로 turnaround time과 waiting time 출력
    for(int i = 0; i < process_count; i++){
        printf("process %d, turnaround time: %d, waiting time: %d\n", 
            processes[i].pid, processes[i].turnaround_time, processes[i].waiting_time);
    }
    // CPU 사용률 계산
    calculate_cpu_utilization(&chart, process_count);
}

// I/O 요청 수가 많은 프로세스 우선
// I/O 디바이스도 최대한 효율적으로 사용하길 기대하는 스케줄러
void IO_count_first_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(ICF_NON_PREEMPTIVE_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_io_count, NON_PREEMPTIVE_);
}

void IO_count_first_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(ICF_PREEMPTIVE_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_io_count, PREEMPTIVE_);
}

void aging_priority_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(AGING_PRIORITY_NON_PREEMPTIVE_);
    config(ready_queue, waiting_queue, compare_dynamic_priority);
    Process* running = NULL;
    int terminated_count = 0;
    int timer = 0;

    // 간트차트 초기화
    GanttChart chart;
    initGanttChart(&chart);
    int cpu_start_time = -1;

    while(terminated_count < process_count){
        updateQueue(ready_queue, waiting_queue, processes, &chart, process_count, timer);

        // Ready Queue에서 프로세스 가져오기
        if (running == NULL && !isQueueEmpty(ready_queue)) {
            running = dequeue(ready_queue);
            running->dynamic_priority = running->priority;
            running->waiting_time += timer - running->ready_time;
            cpu_start_time = timer;
        }

        // IO 처리
        ioOperation(waiting_queue);

        // 프로세스 실행
        if (running != NULL) {
            running->cpu_remaining_time--;
            running->current_cpu_burst_time++;

            // 프로세스 완료 처리
            if (running->cpu_remaining_time == 0) {
                running->turnaround_time = timer + 1 - running->arrival_time;
                terminated_count++;
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, TERMINATED);
                running = NULL;
            }
            
            // IO 요청 처리
            else if (running->io_request_time[running->current_cpu_burst_time]) {
                enqueue(waiting_queue, running);
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, IO_REQUEST);
                running = NULL;
            }
        }
        ++timer;
        agingQueue(ready_queue, timer);

    }
    // 간트차트 출력
    //printGanttChart(&chart, processes, process_count);
    // 간트로그 출력
    printGanttLog(&chart, processes, process_count);
    // process 별로 turnaround time과 waiting time 출력
    for(int i = 0; i < process_count; i++){
        printf("process %d, turnaround time: %d, waiting time: %d\n", 
            processes[i].pid, processes[i].turnaround_time, processes[i].waiting_time);
    }
    // CPU 사용률 계산
    calculate_cpu_utilization(&chart, process_count);
}

void MLQ(Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(MLQ_);

    MultiLevelQueue mlq;
    initMultiLevelQueue(&mlq);
    initQueue(waiting_queue, compare_io_remaining_time);
    Process* running = NULL;
    int terminated_count = 0;
    int timer = 0;
    int q = 0;

    // 간트차트 초기화
    GanttChart chart;
    initGanttChart(&chart);
    int cpu_start_time = -1;

    while(terminated_count < process_count){
        updateMLQ(&mlq, waiting_queue, processes, process_count, timer);

        // time out 처리
        if (running != NULL && q == 0) {
            running->ready_time = timer;
            enqueue(&mlq.queues[running->level], running);
            addBlock(&chart, running->pid, cpu_start_time, timer, CPU, TIMEOUT);
            running = NULL;
        }

        // Ready Queue에서 프로세스 가져오기
        if (running == NULL && !isQueueEmpty(&mlq.queues[1])) {
            running = dequeue(&mlq.queues[1]);
            q = running->level * 3;
            running->waiting_time += timer - running->ready_time;
            cpu_start_time = timer;
        }
        else if (running == NULL && !isQueueEmpty(&mlq.queues[2])) {
            running = dequeue(&mlq.queues[2]);
            q = running->level * 3;
            running->waiting_time += timer - running->ready_time;
            cpu_start_time = timer;
        }
        else if (running == NULL && !isQueueEmpty(&mlq.queues[3])) {
            running = dequeue(&mlq.queues[3]);
            q = running->level * 3;
            running->waiting_time += timer - running->ready_time;
            cpu_start_time = timer;
        }

        // IO 처리
        ioOperation(waiting_queue);

        // 프로세스 실행
        if (running != NULL) {
            running->cpu_remaining_time--;
            running->current_cpu_burst_time++;
            q--;

            // 프로세스 완료 처리
            if (running->cpu_remaining_time == 0) {
                running->turnaround_time = timer + 1 - running->arrival_time;
                terminated_count++;
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, TERMINATED);
                running = NULL;
            }
            
            // IO 요청 처리
            else if (running->io_request_time[running->current_cpu_burst_time]) {
                enqueue(waiting_queue, running);
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, IO_REQUEST);
                running = NULL;
            }
        }
        ++timer;
    }
    // 간트차트 출력
    //printGanttChart(&chart, processes, process_count);
    // 간트로그 출력
    printGanttLog(&chart, processes, process_count);
    // process 별로 turnaround time과 waiting time 출력
    for(int i = 0; i < process_count; i++){
        printf("process %d, turnaround time: %d, waiting time: %d\n", 
            processes[i].pid, processes[i].turnaround_time, processes[i].waiting_time);
    }
    // CPU 사용률 계산
    calculate_cpu_utilization(&chart, process_count);
}

void MLFQ(Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(MLFQ_);

    MultiLevelQueue mlq;
    initMultiLevelQueue(&mlq);
    initQueue(waiting_queue, compare_io_remaining_time);
    Process* running = NULL;
    int terminated_count = 0;
    int timer = 0;
    int q = 0;

    // 간트차트 초기화
    GanttChart chart;
    initGanttChart(&chart);
    int cpu_start_time = -1;

    while(terminated_count < process_count){
        updateMLQ(&mlq, waiting_queue, processes, process_count, timer);

        // time out 처리
        if (running != NULL && q == 0) {
            running->ready_time = timer;
            if(running->level < 3) running->level++;    // 타임 퀀텀이 더 길게 필요하다고 판단.
            enqueue(&mlq.queues[running->level], running);
            addBlock(&chart, running->pid, cpu_start_time, timer, CPU, TIMEOUT);
            running = NULL;
        }

        // Ready Queue에서 프로세스 가져오기
        if (running == NULL && !isQueueEmpty(&mlq.queues[1])) {
            running = dequeue(&mlq.queues[1]);
            q = running->level * 3;
            running->waiting_time += timer - running->ready_time;
            cpu_start_time = timer;
        }
        else if (running == NULL && !isQueueEmpty(&mlq.queues[2])) {
            running = dequeue(&mlq.queues[2]);
            q = running->level * 3;
            running->waiting_time += timer - running->ready_time;
            cpu_start_time = timer;
        }
        else if (running == NULL && !isQueueEmpty(&mlq.queues[3])) {
            running = dequeue(&mlq.queues[3]);
            q = running->level * 3;
            running->waiting_time += timer - running->ready_time;
            cpu_start_time = timer;
        }

        // IO 처리
        ioOperation(waiting_queue);

        // 프로세스 실행
        if (running != NULL) {
            running->cpu_remaining_time--;
            running->current_cpu_burst_time++;
            q--;

            // 프로세스 완료 처리
            if (running->cpu_remaining_time == 0) {
                running->turnaround_time = timer + 1 - running->arrival_time;
                terminated_count++;
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, TERMINATED);
                running = NULL;
            }
            
            // IO 요청 처리
            else if (running->io_request_time[running->current_cpu_burst_time]) {
                enqueue(waiting_queue, running);
                addBlock(&chart, running->pid, cpu_start_time, timer + 1, CPU, IO_REQUEST);
                running = NULL;
            }
        }
        ++timer;
        FeedbackMLQ(&mlq, timer);
    }
    // 간트차트 출력
    //printGanttChart(&chart, processes, process_count);
    // 간트로그 출력
    printGanttLog(&chart, processes, process_count);
    // process 별로 turnaround time과 waiting time 출력
    for(int i = 0; i < process_count; i++){
        printf("process %d, turnaround time: %d, waiting time: %d\n", 
            processes[i].pid, processes[i].turnaround_time, processes[i].waiting_time);
    }
    // CPU 사용률 계산
    calculate_cpu_utilization(&chart, process_count);
}

// Longest IO First (LIF) 스케줄러
void LIF_non_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(LIF_NON_PREEMPTIVE_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_total_io_remaining_time, NON_PREEMPTIVE_);
}

void LIF_preemptive(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(LIF_PREEMPTIVE_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_total_io_remaining_time, PREEMPTIVE_);
}

void HRRN(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    print_scheduler(HRRN_);
    scheduling(ready_queue, waiting_queue, processes, process_count, compare_hrrn, NON_PREEMPTIVE_);
}

void compare_all(Queue* ready_queue, Queue* waiting_queue, Process* processes, int process_count){
    float min_avg_waiting_time = FLT_MAX;
    float min_avg_turnaround_time = FLT_MAX;
    int flag_wt = 0;
    int flag_tt = 0;

    printf("--Find optimal scheduler with WT and TT--\n");
    printf("-------FCFS-------\n");
    FCFS(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, FCFS_);

    printf("----SJF (non-preemptive)----\n");
    SJF_non_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, SJF_NON_PREEMPTIVE_);
    
    printf("----SJF (preemptive)----\n");
    SJF_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, SJF_PREEMPTIVE_);
    
    printf("----RR----\n");
    RR(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, RR_);
    
    printf("----Priority (non-preemptive)----\n");
    Priority_non_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, PRIORITY_NON_PREEMPTIVE_);
    
    printf("----Priority (preemptive)----\n");
    Priority_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, PRIORITY_PREEMPTIVE_);

    printf("----IO Count First (non-preemptive)----\n");
    IO_count_first_non_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, ICF_NON_PREEMPTIVE_);

    printf("----IO Count First (preemptive)----\n");
    IO_count_first_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, ICF_PREEMPTIVE_);
    
    printf("----Aging Priority (non-preemptive)----\n");
    aging_priority_non_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, AGING_PRIORITY_NON_PREEMPTIVE_);

    printf("----Multi-Level Queue (MLQ)----\n");
    MLQ(waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, MLQ_);

    printf("----Multi-Level Feedback Queue (MLFQ)----\n");
    MLFQ(waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, MLFQ_);

    printf("----LIF (non-preemptive)----\n");
    LIF_non_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, LIF_NON_PREEMPTIVE_);

    printf("----LIF (preemptive)----\n");
    LIF_preemptive(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, LIF_PREEMPTIVE_);

    printf("----HRRN----\n");
    HRRN(ready_queue, waiting_queue, processes, process_count);
    evaluator_all(processes, process_count, &min_avg_waiting_time, &min_avg_turnaround_time, 
        &flag_wt, &flag_tt, HRRN_);
    
    printf("---------------------------------\n");
    printf("optimal waiting time: %.2f with ", min_avg_waiting_time);
    print_scheduler(flag_wt);
    printf("optimal turnaround time: %.2f with ", min_avg_turnaround_time);
    print_scheduler(flag_tt);
    printf("---------------------------------\n");
}