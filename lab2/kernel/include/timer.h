#ifndef	_TIMER_H
#define	_TIMER_H

#include "utils.h"

typedef void (*timer_callback)(char *message);

typedef struct timer
{
    struct task_timer   *next;
    struct task_timer   *prev;
    char                *data;
    uint64              expiry_time;
    timer_callback      callback;
}task_timer;

extern task_timer *timer_head;

void add_timer(timer_callback callback, uint32 sec, char *msg);
void send_message(char *message);

#endif