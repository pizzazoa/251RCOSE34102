#ifndef MYGANTT_H
#define MYGANTT_H

#include "myprocess.h"

#define MAX_TIME 50
#define MAX_LOG 2000

enum {
    CPU = 0,
    READY_QUEUE,
    WAITING_QUEUE,
    LEVEL_1_QUEUE,
    LEVEL_2_QUEUE,
    LEVEL_3_QUEUE
};

enum{
    ARRIVE = 0,
    TERMINATED,
    IO_REQUEST,
    PREEMPTED,
    TIMEOUT,
    IO_COMPLETE
};

typedef struct {
    int pid;           // 프로세스 ID (0이면 idle 상태)
    int start_time;    // 시작 시간
    int end_time;      // 종료 시간
    char where[30];     // 프로세스가 실행된 위치 (CPU, I/O 등)
    char state[20];     // 프로세스 상태
} GanttBlock;

typedef struct {
    GanttBlock blocks[MAX_LOG];
    int size;
} GanttChart;

void initGanttChart(GanttChart* chart);
void addBlock(GanttChart* chart, int pid, int start_time, int end_time, int where_flag, int state_flag);
void printGanttLog(GanttChart* chart, Process* processes, int process_count);
void printGanttChart(GanttChart* chart, Process* processes, int process_count);

#endif