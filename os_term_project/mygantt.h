#ifndef MYGANTT_H
#define MYGANTT_H

#include "myprocess.h"

#define MAX_TIME 50

typedef struct {
    int pid;           // 프로세스 ID (0이면 idle 상태)
    int start_time;    // 시작 시간
    int end_time;      // 종료 시간
    char state[11];
} GanttBlock;

typedef struct {
    GanttBlock blocks[MAX_PROCESS * 10];
    int size;
} GanttChart;

// 간트차트 초기화
void initGanttChart(GanttChart* chart);

// 새로운 CPU 블록 추가
void addBlock(GanttChart* chart, int pid, int start_time, int end_time, int state_flag);

// 간트차트 출력
void printGanttChart(GanttChart* chart, Process* processes, int process_count);

// 간트차트 로그 출력
void printGanttLog(GanttChart* chart, Process* processes, int process_count);

#endif