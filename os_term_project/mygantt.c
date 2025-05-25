#include "mygantt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initGanttChart(GanttChart* chart) {chart->size = 0;}

void addBlock(GanttChart* chart, int pid, int start_time, int end_time, int where_flag, int state_flag) {
    if (chart->size >= MAX_LOG) return;

    GanttBlock block;
    block.pid = pid;
    block.start_time = start_time;
    block.end_time = end_time;
    switch(where_flag) {
        case CPU:
            strcpy(block.where, "CPU");
            break;
        case READY_QUEUE:
            strcpy(block.where, "Ready Queue");
            break;
        case WAITING_QUEUE:
            strcpy(block.where, "Waiting Queue");
            break;
        case LEVEL_1_QUEUE:
            strcpy(block.where, "Level 1 Queue (Priority)");
            break;
        case LEVEL_2_QUEUE:
            strcpy(block.where, "Level 2 Queue (SJF)");
            break;
        case LEVEL_3_QUEUE:
            strcpy(block.where, "Level 3 Queue (FCFS)");
            break;
        
    }
    switch(state_flag){
        case ARRIVE:
            strcpy(block.state, "ARRIVE");
            break;
        case TERMINATED:
            strcpy(block.state, "TERMINATED");
            break;
        case IO_REQUEST:
            strcpy(block.state, "IO_REQUEST");
            break;
        case PREEMPTED:
            strcpy(block.state, "PREEMPTED");
            break;
        case TIMEOUT:
            strcpy(block.state, "TIMEOUT");
            break;
        case IO_COMPLETE:
            strcpy(block.state, "IO_COMPLETE");
            break;
    }

    chart->blocks[chart->size++] = block;
}

void printGanttLog(GanttChart* chart, Process* processes, int process_count) {
    if (chart->size == 0) {
        printf("Ganttchart is empty.\n");
        return;
    }
    
    printf("\n=======Gantt Log=======\n");

    for(int i = 0; i < chart->size; i++) {
        GanttBlock block = chart->blocks[i];
        int pid = block.pid;
        int start_time = block.start_time;
        int end_time = block.end_time;
        if(start_time == end_time)
            printf("time [%d]: (%s) P%d -> %s\n", start_time, block.where, pid, block.state);
        else
            printf("time [%d~%d]: (%s) P%d -> %s\n", start_time, end_time, block.where, pid, block.state);
    }
    printf("=======================\n\n");
}

// 간트차트 출력 (최대 50초 출력)
void printGanttChart(GanttChart* chart, Process* processes, int process_count) {
    if (chart->size == 0) {
        printf("Ganttchart is empty.\n");
        return;
    }
    
    printf("===Gantt Chart===\n");
    int last_end_time = chart->blocks[chart->size - 1].end_time;
    int timer = -1;

    // 윗줄
    for(int i = 0; i < last_end_time; i++) {
        printf("+"); printf("--");
    }
    printf("+\n");

    // 가운데 줄
    for(int i = 0; i < chart->size; i++) {
        GanttBlock block = chart->blocks[i];
        int pid = block.pid;
        int start_time = block.start_time;
        int end_time = block.end_time;

        // idle 상태일 경우
        if(start_time != 0 && timer < start_time){
            printf("|");
            for(int k = timer; k < start_time; k++){
                printf("   ");
            }
            //한 칸 공백 제거
            printf("\b");
            timer--;
        }
        printf("|");
        for(int j = start_time; j < end_time; j++){
            timer++;
            if(j == start_time){
                printf("P%d", pid);
            }
            else{
                if(pid > 9){
                    printf("  ");
                }
                else{
                    printf("   ");
                }
            } 
        }
        timer = end_time;
    }
    printf("|\n");

    //아랫줄
    for(int i = 0; i < last_end_time; i++) {
        printf("+"); printf("--");
    }
    printf("+\n");

    // 시간
    for(int i = 0; i < last_end_time; i++) {
        if(i > 9){
            printf("%d ", i);
        }
        else{
            printf("%d  ", i);
        }
    }
    printf("%d\n", last_end_time);
}
