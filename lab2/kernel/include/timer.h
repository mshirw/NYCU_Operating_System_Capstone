#ifndef	_TIMER_H
#define	_TIMER_H

#include "utils.h"

typedef void (*timer_callback)(char *message);

typedef struct timer
{
    char                *data;
    uint64              expiry_time;
    timer_callback      callback;
    struct task_timer   *next;
}task_timer;

extern task_timer *timer_head;

void add_timer(timer_callback callback, uint32 timeout);
void send_message(char *message);
void create_timer();

#endif