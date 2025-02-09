#ifndef	_TASKQUEUE_H
#define	_TASKQUEUE_H
#include "utils.h"

typedef void (*taskCallBack)(void);

typedef struct taskQ
{
    struct taskQ *next;
    struct taskQ *prev;
    taskCallBack callback;
    uint8 priority;
}taskQueue;

extern taskQueue *task_head;

void execute_task();
void create_task(taskCallBack callback, uint8 priority);
void enqueue_task(taskQueue *new_task);

#endif