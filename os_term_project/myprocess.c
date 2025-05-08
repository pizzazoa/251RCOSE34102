#include "myprocess.h"

Process make_process(int pid){
    Process process;

    process.pid = pid;
    process.arrival_time = rand() % 30;  // 0~29 사이의 도착 시간

    process.cpu_burst_time = (rand() % 21) + 5;  // 5~25 사이의 CPU 처리 시간
    process.cpu_remaining_time = process.cpu_burst_time;

    process.io_count = (rand() % MAX_IO_COUNT + 1);  // 1~5 사이의 I/O 요청 개수
    process.io_request_time = calloc(process.cpu_burst_time, sizeof(bool));
    if(process.io_count != 0){
        process.io_burst_time = (rand() % 8) + 1;  // 1~8 사이의 I/O 처리 시간
        process.io_remaining_time = process.io_burst_time;

        int set = 0;
        while (set < process.io_count) {
            int idx = (rand() % process.cpu_burst_time - 1) + 1; // 1~cpu_burst_time-1 사이의 인덱스
            if (!process.io_request_time[idx]) {
                process.io_request_time[idx] = true;
                set++;
            }
        }
    }
    else{
        process.io_burst_time = 0;
        process.io_remaining_time = 0;
    }
    process.waiting_time = 0;
    process.turnaround_time = 0;
    process.priority = (rand() % 15) + 1;  // 1~15 사이의 우선순위
    
    return process;
}

void reset_processes(Process process[], int process_count) {
    for (int i = 0; i < process_count; ++i) {
        process[i].cpu_remaining_time = process[i].cpu_burst_time;
        process[i].io_remaining_time  = process[i].io_burst_time;
        process[i].waiting_time    = 0;
        process[i].turnaround_time = 0;
    }
}

int make_dummy_processes(Process p[]) {
    const int n = 5;
    int arrival[]   = { 0, 1,  2,  4,  6 };
    int cpu_burst[] = { 8, 4, 12,  6,  3 };
    int io_req[]    = { 3, 0,  5,  0,  1 };  // 0이면 I/O 없음
    int io_burst[]  = { 1, 0,  2,  0,  1 };
    int priority[]  = { 2, 1,  4,  3,  2 };

    for (int i = 0; i < n; ++i) {
        p[i].pid                = i + 1;
        p[i].arrival_time       = arrival[i];

        p[i].cpu_burst_time     = cpu_burst[i];
        p[i].cpu_remaining_time = cpu_burst[i];

        // I/O 이벤트 개수 (io_req[i] > 0 이면 1회, 아니면 0회)
        p[i].io_count           = io_req[i] > 0 ? 1 : 0;
        p[i].io_burst_time      = io_burst[i];
        p[i].io_remaining_time  = io_burst[i];

        // cpu_burst_time 길이만큼 플래그 배열 할당 & 초기화
        p[i].io_request_time    = calloc(p[i].cpu_burst_time, sizeof(bool));
        if (p[i].io_count == 1) {
            // 지정된 tick에서만 true로 세팅
            p[i].io_request_time[ io_req[i] ] = true;
        }

        p[i].waiting_time    = 0;
        p[i].turnaround_time = 0;
        p[i].ready_time      = 0;
        p[i].priority        = priority[i];
    }

    return n;
}