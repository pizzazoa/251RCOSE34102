#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "myscheduler.h"


Process processes[MAX_PROCESS];
Queue ready_queue;
Queue waiting_queue;
Process* running;
int process_count = 0;  // 프로세스 개수
int terminated_count = 0;  // 종료된 프로세스 개수
int timer = 0;  // 현재 시간
int pid = 1;

int main(){

    srand(time(NULL));

    int choice;
    // printf("=====CPU SCHEDULING SIMULATOR=====\n");
    // printf("Enter the number of processes: ");
    // scanf("%d", &process_count);
    // if (process_count > MAX_PROCESS) {
    //     printf("Maximum number of processes is %d\n", MAX_PROCESS);
    //     process_count = MAX_PROCESS;
    // }

    // for (int i = 0; i < process_count; i++) {
    //     processes[i] = make_process(pid++);
    //     Process *p = &processes[i];

    //     // 기본 정보
    //     printf("Process %d: arrival_time=%d, cpu_burst_time=%d, io_burst_time=%d, I/O @ ticks: ",
    //            p->pid, p->arrival_time, p->cpu_burst_time, p->io_burst_time);

    //     // io_request_time 배열 중 true인 인덱스만 출력
    //     bool any = false;
    //     for (int t = 0; t < p->cpu_burst_time; t++) {
    //         if (p->io_request_time && p->io_request_time[t]) {
    //             if (any) printf(", ");
    //             printf("%d", t);
    //             any = true;
    //         }
    //     }
    //     if (!any) {
    //         printf("none");
    //     }
    //     printf("\n");
    // }

    process_count = make_dummy_processes(processes);

    /* 확인용 출력 */
    for (int i = 0; i < process_count; ++i) {
        Process *pr = &processes[i];
        printf("P%d: AT=%2d | CPU=%2d | I/O @ ticks: ",
               pr->pid, pr->arrival_time, pr->cpu_burst_time);

        if (pr->io_count == 0) {
            printf("none");
        } else {
            bool first = true;
            for (int t = 0; t < pr->cpu_burst_time; ++t) {
                if (pr->io_request_time[t]) {
                    if (!first) printf(", ");
                    printf("%d", t);
                    first = false;
                }
            }
        }

        printf(" | IOburst=%2d | Pri=%d\n",
               pr->io_burst_time, pr->priority);
    }

    do{
        printf("1. FCFS\n2. SJF (non-preemptive)\n3. SJF (preemptive)\n4. RR\n"
            "5. Priority (non-preemptive)\n6. Priority (preemptive)\n7. Compare all\n"
            "8. make other processes set\n9. Exit\n");
        printf("Enter the number: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                printf("FCFS Scheduling\n");
                FCFS(&ready_queue, &waiting_queue, 
                    processes, process_count, &running, &terminated_count, &timer);
                reset_processes(processes, process_count);
                break;
            case 2:
                printf("SJF (non-preemptive) Scheduling\n");
                SJF_non_preemptive(&ready_queue, &waiting_queue, 
                    processes, process_count, &running, &terminated_count, &timer);
                reset_processes(processes, process_count);
                break;
            case 3:
                printf("SJF (preemptive) Scheduling\n");
                SJF_preemptive(&ready_queue, &waiting_queue, 
                    processes, process_count, &running, &terminated_count, &timer);
                reset_processes(processes, process_count);
                break;
            case 4:
                printf("RR Scheduling\n");
                RR(&ready_queue, &waiting_queue, 
                    processes, process_count, &running, &terminated_count, &timer);  // time quantum = 2
                reset_processes(processes, process_count);
                break;
            case 5:
                printf("Priority (non-preemptive) Scheduling\n");
                Priority_non_preemptive(&ready_queue, &waiting_queue, 
                    processes, process_count, &running, &terminated_count, &timer);
                reset_processes(processes, process_count);
                break;
            case 6:
                printf("Priority (preemptive) Scheduling\n");
                Priority_preemptive(&ready_queue, &waiting_queue, 
                    processes, process_count, &running, &terminated_count, &timer);
                reset_processes(processes, process_count);
                break;
            case 7:
                printf("Compare all\n");
                break;
            case 8:
                printf("make other process\n");
                break;
            case 9:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while(choice != 9);
    

    return 0;
}