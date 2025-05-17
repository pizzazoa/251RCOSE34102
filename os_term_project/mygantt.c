#include "mygantt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void initGanttChart(GanttChart* chart) {chart->size = 0;}

void addBlock(GanttChart* chart, int pid, int start_time, int end_time, int state_flag) {
    if (chart->size >= MAX_PROCESS * 10) return;
    GanttBlock block;

    block.pid = pid;
    block.start_time = start_time;
    block.end_time = end_time;
    switch(state_flag){
        case 0:
            strcpy(block.state, "TERMINATED");
            break;
        case 1:
            strcpy(block.state, "WAITING");
            break;
        case 2:
            strcpy(block.state, "PREEMPTIVED");
            break;
        case 3:
            strcpy(block.state, "TIMEOUT");
            break;
    }

    
    chart->blocks[chart->size++] = block;
}

// 간트차트 출력
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

void printGanttLog(GanttChart* chart, Process* processes, int process_count) {
    if (chart->size == 0) {
        printf("Ganttchart is empty.\n");
        return;
    }
    
    printf("===Gantt Log===\n");

    for(int i = 0; i < chart->size; i++) {
        GanttBlock block = chart->blocks[i];
        int pid = block.pid;
        int start_time = block.start_time;
        int end_time = block.end_time;

        printf("P%d: running from %d to %d -> %s\n", pid, start_time, end_time, block.state);
    }
    printf("===End of Gantt Log===\n");
}