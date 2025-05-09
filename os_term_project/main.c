#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "myscheduler.h"
#include "myevaluator.h"


Process processes[MAX_PROCESS];
Queue ready_queue;
Queue waiting_queue;
int process_count = 0;  // 프로세스 개수
int pid = 1;

enum{
    FCFS_ = 1,
    SJF_NON_PREEMPTIVE_,
    SJF_PREEMPTIVE_,
    RR_,
    PRIORITY_NON_PREEMPTIVE_,
    PRIORITY_PREEMPTIVE_,
    COMPARE_,
    MAKE_OTHER_PROCESSES_SET_,
    EXIT_
};

int main(){

    srand(time(NULL));

    int choice;
    printf("=====CPU SCHEDULING SIMULATOR=====\n");
    printf("Processes are generating...\n");
    // process_count = (rand() % (MAX_PROCESS + 1)) + 1;  // 1~MAX_PROCESS 사이의 프로세스 개수
    // for (int i = 0; i < process_count; i++) {
    //     processes[i] = make_process(pid++);
    // }
    // print_processes(processes, process_count);
    // printf("Processes are generated.\n");

    process_count = make_dummy_processes(processes);

    /* 확인용 출력 */
    print_processes(processes, process_count);

    do{
        printf("%d. FCFS\n%d. SJF (non-preemptive)\n%d. SJF (preemptive)\n%d. RR\n"
            "%d. Priority (non-preemptive)\n%d. Priority (preemptive)\n%d. Compare all\n"
            "%d. make other processes set\n%d. Exit\n", FCFS_, SJF_NON_PREEMPTIVE_,
            SJF_PREEMPTIVE_, RR_, PRIORITY_NON_PREEMPTIVE_, PRIORITY_PREEMPTIVE_,
            COMPARE_, MAKE_OTHER_PROCESSES_SET_, EXIT_);
        printf("Select Menu: ");
        scanf("%d", &choice);

        switch(choice){
            case FCFS_:
                printf("-------------------------\n");
                FCFS(&ready_queue, &waiting_queue, processes, process_count);
                evaluator(processes, process_count);
                break;
            case SJF_NON_PREEMPTIVE_:
                printf("-------------------------\n");
                SJF_non_preemptive(&ready_queue, &waiting_queue, processes, process_count);
                evaluator(processes, process_count);
                break;
            case SJF_PREEMPTIVE_:
                printf("-------------------------\n");
                SJF_preemptive(&ready_queue, &waiting_queue, processes, process_count);
                evaluator(processes, process_count);                
                break;
            case RR_:
                printf("-------------------------\n");
                RR(&ready_queue, &waiting_queue, processes, process_count);  // time quantum = 2
                evaluator(processes, process_count);                
                break;
            case PRIORITY_NON_PREEMPTIVE_:
                printf("-------------------------\n");
                Priority_non_preemptive(&ready_queue, &waiting_queue, processes, process_count);
                evaluator(processes, process_count);                
                break;
            case PRIORITY_PREEMPTIVE_:
                printf("-------------------------\n");
                Priority_preemptive(&ready_queue, &waiting_queue, processes, process_count);
                evaluator(processes, process_count);                
                break;
            case COMPARE_:
                printf("-------------------------\n");
                printf("Compare all\n");
                compare_all(&ready_queue, &waiting_queue, processes, process_count);
                break;
            case MAKE_OTHER_PROCESSES_SET_:
                printf("-------------------------\n");
                printf("make other process set!\n");
                printf("Processes are generating...\n");
                process_count = (rand() % (MAX_PROCESS + 1)) + 1;  // 1~MAX_PROCESS 사이의 프로세스 개수
                for (int i = 0; i < process_count; i++) {
                    processes[i] = make_process(pid++);
                }
                print_processes(processes, process_count);
                printf("Processes are generated.\n");
                break;
            case EXIT_:
                printf("-------------------------\n");
                printf("Exiting...\n");
                break;
            default:
                printf("-------------------------\n");
                printf("Invalid choice! Please try again.\n");
        }
    } while(choice != EXIT_);
    

    return 0;
}