#include "message_buffer_semaphore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

int shmid;
void *memory_segment=NULL;

int semid;
union semun sem_union;

void init_sem() {
    /*---------------------------------------*/
    /* TODO 1 : init semaphore               */
    if((semid = semget(SEM_KEY, 1, IPC_CREAT|IPC_EXCL|0666)) == -1){
        if((semid = semget(SEM_KEY, 0, 0)) == -1) return;
    } else{
        sem_union.val = 1;
        semctl(semid, 0, SETVAL, sem_union);
    }
    /* TODO 1 : END                          */
    /*---------------------------------------*/
    printf("init semid : %d\n", semid);
}

void destroy_sem() {
    /*---------------------------------------*/
    /* TODO 2 : destroy semaphore            */
    if((semid = semget(SEM_KEY, 0, 0)) == -1) return;
    if(semctl(semid, 0, IPC_RMID) == -1) return;
    /* TODO 2 : END                          */
    /*---------------------------------------*/
}

void s_wait() {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;
    semid = semget(SEM_KEY, 0, 0);

    if (semop(semid, &buf, 1) == -1) {
        printf("<s_wait> semop error!\n");
	return;
    }
}

void s_quit() {
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;
    semid = semget(SEM_KEY, 0, 0);

    if (semop(semid, &buf, 1) == -1) {
        printf("<s_quit> semop error!\n");
	return;
    }
}

/*---------------------------------------------*/
/* TODO 3 : use s_quit() and s_wait()          */
/* (1) clone from assignment 1-1               */
/* (2) use semaphore                           */
/* (3) report explanation (ex. 2020020000.pdf) */
int init_buffer(MessageBuffer **buffer) {
    /*---------------------------------------*/
    /* TODO 1 : init buffer                  */
    if((shmid = shmget(SHM_KEY, sizeof(MessageBuffer), IPC_CREAT|0666)) == -1) return -1;
    if((memory_segment = shmat(shmid, NULL, 0)) == (void*)-1) return -1;
    *buffer = (MessageBuffer *)memory_segment;
    (*buffer)->is_empty = 1;
    (*buffer)->account_id = -1;
    memset((*buffer)->messages, 0, sizeof(Message) * BUFFER_SIZE);
    /* TODO 1 : END                          */
    /*---------------------------------------*/

    printf("init buffer\n");
    return 0;
}

int attach_buffer(MessageBuffer **buffer) {
    /*---------------------------------------*/
    /* TODO 2 : attach buffer                */
    /* do not consider "no buffer situation" */
    if((shmid = shmget(SHM_KEY, sizeof(MessageBuffer), 0666)) == -1) return -1;
    if((memory_segment = shmat(shmid, NULL, 0)) == (void*)-1) return -1;
    *buffer = (MessageBuffer *)memory_segment;
    /* TODO 2 : END                          */
    /*---------------------------------------*/

    printf("attach buffer\n");
    printf("\n");
    return 0;
}

int detach_buffer() {
    if (shmdt(memory_segment) == -1) {
        printf("shmdt error!\n\n");
        return -1;
    }

    printf("detach buffer\n\n");
    return 0;
}

int destroy_buffer() {
    if(shmctl(shmid, IPC_RMID, NULL) == -1) {
        printf("shmctl error!\n\n");
        return -1;
    }
    printf("destroy shared_memory\n\n");
    return 0;
}

int produce(MessageBuffer **buffer, int sender_id, int data, int account_id) {

    /*---------------------------------------*/
    /* TODO 3 : produce message              */
    s_wait();
	(*buffer)->messages[account_id].sender_id = sender_id;
    (*buffer)->messages[account_id].data += data;
    (*buffer)->account_id = account_id;
    (*buffer)->is_empty = 0;
    s_quit();
    /* TODO 3 : END                          */
    /*---------------------------------------*/

    printf("produce message\n");

    return 0;
}

int consume(MessageBuffer **buffer, Message **message) {

    /*---------------------------------------*/
    /* TODO 4 : consume message              */
    s_wait();
    if((*buffer)->is_empty == 1){
        s_quit();
        return -1;
    }
    *message = &((*buffer)->messages[(*buffer)->account_id]);
    (*buffer)->is_empty = 1;
    s_quit();
    /* TODO 4 : END                          */
    /*---------------------------------------*/
    return 0;
}

/* TODO 3 : END                                          */
/*-------------------------------------------------------*/
