#include "taskQueue.h"
#include "mini_uart.h"
#include "utils.h"
#include "memalloc.h"

taskQueue *task_head = NULL;

void create_task(taskCallBack callback, uint8 priority)
{
    taskQueue *task = (taskQueue*)malloc(sizeof(taskQueue));

    if(!task)
    {
        uart_send_string("create task fail!\r\n");
        return;
    }

    task->next = NULL;
    task->prev = NULL;
    task->callback = callback;
    task->priority = priority;

    enqueue_task(task);
}

void enqueue_task(taskQueue *new_task)
{
    taskQueue *temp_task = NULL;

    if(task_head == NULL)
    {
        task_head = new_task;
        task_head->prev = NULL;
        task_head->next = NULL;
    }
    else
    {
        temp_task = task_head;

        while(temp_task != NULL)
        {
            if(new_task->priority < temp_task->priority)
            {
                //insert before temp_task
                new_task->next = temp_task;
                new_task->prev = temp_task->prev;
                temp_task->prev = new_task;

                if(temp_task == task_head)
                    task_head = new_task;
                return;
            }
            else
            {
                if(temp_task->next == NULL)
                    break;
                temp_task = temp_task->next;
            }
            
        }
        temp_task->next = new_task;
        new_task->prev = temp_task;
    }
}

void execute_task()
{
    //execute all interrupt tasks before return to main kernel.
    while(task_head)
    {
        task_head->callback();
        task_head = task_head->next;
    }
}