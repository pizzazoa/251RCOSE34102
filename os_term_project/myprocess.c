#include "myprocess.h"

Process make_process(int pid){
    Process process;

    process.pid = pid;
    process.arrival_time = rand() % 20;  // 0~19 사이의 도착 시간

    process.cpu_burst_time = (rand() % 16) + 5;  // 5~20 사이의 CPU 처리 시간
    process.cpu_remaining_time = process.cpu_burst_time;
    process.current_cpu_burst_time = 0;

    process.io_count = (rand() % MAX_IO_COUNT + 1);  // 1~5 사이의 I/O 요청 개수
    process.io_burst_time = calloc(process.cpu_burst_time + 1, sizeof(int));
    process.io_remaining_time = calloc(process.cpu_burst_time + 1, sizeof(int));
    process.io_request_time = calloc(process.cpu_burst_time + 1, sizeof(bool));
    process.total_io_burst_time = 0;

    int set = process.io_count;
    while(set > 0){
        int idx = (rand() % process.cpu_burst_time) + 1; // 1~cpu_burst_time 사이의 인덱스
        if (idx == process.cpu_burst_time) continue;  // 마지막 인덱스는 I/O 요청 불가
        if (!process.io_request_time[idx]) {
            process.io_request_time[idx] = true;
            process.io_burst_time[idx] = (rand() % 8) + 1;  // 1~8 사이의 I/O 처리 시간
            process.total_io_burst_time += process.io_burst_time[idx];
            process.io_remaining_time[idx] = process.io_burst_time[idx];
            set--;
        }
    }

    process.total_io_remaining_time = process.total_io_burst_time;
    process.waiting_time = 0;
    process.turnaround_time = 0;
    process.priority = (rand() % 10) + 1;  // 1~10 사이의 우선순위
    process.dynamic_priority = process.priority;
    if(process.io_count > 1) process.level = 1; // I/O 요청이 2개 이상인 경우 레벨 1
    else if(process.cpu_burst_time < 11) process.level = 2; // cpu burst time이 10 이하인 경우 레벨 2
    else process.level = 3;  // MLFQ 레벨 (1~3)

    return process;
}

void reset_processes(Process processes[], int process_count) {
    for (int i = 0; i < process_count; ++i) {
        processes[i].cpu_remaining_time = processes[i].cpu_burst_time;
        processes[i].current_cpu_burst_time = 0;
        processes[i].waiting_time    = 0;
        processes[i].turnaround_time = 0;
        processes[i].dynamic_priority = processes[i].priority;
        processes[i].level = processes[i].priority % 3 + 1;
        processes[i].total_io_remaining_time = processes[i].total_io_burst_time;
        for(int j = 0; j < processes[i].cpu_burst_time; j++) {
            processes[i].io_remaining_time[j] = processes[i].io_burst_time[j];
        }
    }
}

void free_processes(Process processes[], int process_count) {
    for (int i = 0; i < process_count; ++i) {
        free(processes[i].io_burst_time);
        free(processes[i].io_remaining_time);
        free(processes[i].io_request_time);
    }
}

void print_processes(Process processes[], int process_count) {
    for (int i = 0; i < process_count; i++) {
        printf("Process %d: arrival_time=%d, cpu_burst_time=%d, I/O request/burst: ",
               processes[i].pid, processes[i].arrival_time, processes[i].cpu_burst_time);

        if (processes[i].io_count == 0) {
            printf("none");
        } else {
            bool first = true;
            for (int t = 0; t < processes[i].cpu_burst_time; t++) {
                if (processes[i].io_request_time[t]) {
                    if (!first) printf(", ");
                    printf("%d/%d", t, processes[i].io_burst_time[t]);
                    first = false;
                }
            }
        }

        printf(" , Pri=%d , level=%d\n", processes[i].priority, processes[i].level);
    }
}

int make_dummy_processes(Process p[]) {
    const int n = 5;

    p[0].pid = 1;
    p[0].arrival_time = 0;
    p[0].cpu_burst_time = 8;
    p[0].cpu_remaining_time = 8;
    p[0].current_cpu_burst_time = 0;
    p[0].io_count = 2;
    p[0].io_burst_time = calloc(p[0].cpu_burst_time + 1, sizeof(int));
    p[0].io_remaining_time = calloc(p[0].cpu_burst_time + 1, sizeof(int));
    p[0].io_request_time = calloc(p[0].cpu_burst_time + 1, sizeof(bool));
    p[0].io_burst_time[3] = 1;
    p[0].io_remaining_time[3] = 1;
    p[0].io_burst_time[5] = 3;
    p[0].io_remaining_time[5] = 3;
    p[0].io_request_time[3] = true;  // 3번째 tick에서 I/O 요청
    p[0].io_request_time[5] = true;  // 5번째 tick에서 I/O 요청
    p[0].waiting_time = 0;
    p[0].turnaround_time = 0;
    p[0].ready_time = 0;
    p[0].priority = 2;
    p[0].dynamic_priority = 2;
    p[0].level = 3;
    p[0].total_io_burst_time = 4;
    p[0].total_io_remaining_time = 4;

    p[1].pid = 2;
    p[1].arrival_time = 1;
    p[1].cpu_burst_time = 4;
    p[1].cpu_remaining_time = 4;
    p[1].current_cpu_burst_time = 0;
    p[1].io_count = 0;
    p[1].io_burst_time = calloc(p[1].cpu_burst_time + 1, sizeof(int));
    p[1].io_remaining_time = calloc(p[1].cpu_burst_time + 1, sizeof(int));
    p[1].io_request_time = calloc(p[1].cpu_burst_time + 1, sizeof(bool));
    p[1].waiting_time = 0;
    p[1].turnaround_time = 0;
    p[1].ready_time = 0;
    p[1].priority = 1;
    p[1].dynamic_priority = 1;
    p[1].level = 2;
    p[1].total_io_burst_time = 0;
    p[1].total_io_remaining_time = 0;

    p[2].pid = 3;
    p[2].arrival_time = 2;
    p[2].cpu_burst_time = 12;
    p[2].cpu_remaining_time = 12;
    p[2].current_cpu_burst_time = 0;
    p[2].io_count = 1;
    p[2].io_burst_time = calloc(p[2].cpu_burst_time + 1, sizeof(int));
    p[2].io_remaining_time = calloc(p[2].cpu_burst_time + 1, sizeof(int));
    p[2].io_request_time = calloc(p[2].cpu_burst_time + 1, sizeof(bool));
    p[2].io_burst_time[5] = 2;
    p[2].io_remaining_time[5] = 2;
    p[2].io_request_time[5] = true;  // 5번째 tick에서 I/O 요청
    p[2].waiting_time = 0;
    p[2].turnaround_time = 0;
    p[2].ready_time = 0;
    p[2].priority = 4;
    p[2].dynamic_priority = 4;
    p[2].level = 2;
    p[2].total_io_burst_time = 2;
    p[2].total_io_remaining_time = 2;

    p[3].pid = 4;
    p[3].arrival_time = 4;
    p[3].cpu_burst_time = 6;
    p[3].cpu_remaining_time = 6;
    p[3].current_cpu_burst_time = 0;
    p[3].io_count = 0;
    p[3].io_burst_time = calloc(p[3].cpu_burst_time + 1, sizeof(int));
    p[3].io_remaining_time = calloc(p[3].cpu_burst_time + 1, sizeof(int));
    p[3].io_request_time = calloc(p[3].cpu_burst_time + 1, sizeof(bool));
    p[3].waiting_time = 0;
    p[3].turnaround_time = 0;
    p[3].ready_time = 0;
    p[3].priority = 3;
    p[3].dynamic_priority = 3;
    p[3].level = 1;
    p[3].total_io_burst_time = 0;
    p[3].total_io_remaining_time = 0;

    p[4].pid = 5;
    p[4].arrival_time = 6;
    p[4].cpu_burst_time = 3;
    p[4].cpu_remaining_time = 3;
    p[4].current_cpu_burst_time = 0;
    p[4].io_count = 2;
    p[4].io_burst_time = calloc(p[4].cpu_burst_time + 1, sizeof(int));
    p[4].io_remaining_time = calloc(p[4].cpu_burst_time + 1, sizeof(int));
    p[4].io_request_time = calloc(p[4].cpu_burst_time + 1, sizeof(bool));
    p[4].io_burst_time[1] = 1;
    p[4].io_remaining_time[1] = 1;
    p[4].io_burst_time[2] = 3;
    p[4].io_remaining_time[2] = 3;
    p[4].io_request_time[1] = true;  // 1번째 tick에서 I/O 요청
    p[4].io_request_time[2] = true;  // 2번째 tick에서 I/O 요청
    p[4].waiting_time = 0;
    p[4].turnaround_time = 0;
    p[4].ready_time = 0;
    p[4].priority = 2;
    p[4].dynamic_priority = 2;
    p[4].level = 3;
    p[4].total_io_burst_time = 4;
    p[4].total_io_remaining_time = 4;


    return n;
}