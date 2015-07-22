/*Scheduler module
  Developer: Marcelo Lorenzati
  License: GPL v3
 */

/*Required Header Files*/
#include "msp430g2553.h"

/*Scheduler header file*/
#include "sched.h"
#include "timer.h"

/*Constants*/

/*Global Variable definition*/
struct tSchedData sched;

/*Methods*/
/*Scheduler Module Initialization*/
void schedInit( void )
{
    taskCountType taskCnt;
    eventCountType eventCnt; 
    taskType initialStateTask;

    enableWatchdog(OFF);
    initialStateTask.status = sch_destroyed;
    initialStateTask.taskPointer = NULL;
    initialStateTask.eventPointer = NULL;
    initialStateTask.delay = 0;    
        
    /*Sweep TCBs to initialize*/
    for (taskCnt = 0; taskCnt < SCHED_TASK_MAX; taskCnt++)
    {
        sched.task[taskCnt] = initialStateTask;
    }

    /*Sweep ECBs to initialize*/
    for (eventCnt = 0; eventCnt < SCHED_EVENT_MAX; eventCnt++)
    {
        sched.event[eventCnt] = 0;
    }
 //   enableWatchdog(ON);
}

void schedule( void )
{
    static int noneExecuted; // Aca se modificó el tipo "bit" por "int"
    taskCountType taskCnt;
    tickType tickShadow;
    eventType eventShadow;
    taskType *currentTask;

    /*1.Check for events in the Tasks*/
    for (taskCnt = 0; taskCnt < SCHED_TASK_MAX; taskCnt++)
    {
        currentTask = &sched.task[taskCnt];
        /*The task waits for an event*/

        intProtectSem();
        eventShadow = sched.event[(currentTask->eventPointer) - 1];

        if ((currentTask->status == sch_waiting) && ((currentTask->eventPointer) > 0) && (eventShadow > 0))
        {
            currentTask->status = sch_running;
            (sched.event[(currentTask->eventPointer) - 1])--;
        }
        intUnProtectSem();
    }

    /*Copyprotect the system tick*/
    intProtectTick();
    tickShadow = timer.ticks;
    intUnProtectTick();

    /*2.Check delays*/
    if (tickShadow > 0)
    {
        for (taskCnt = 0; taskCnt < SCHED_TASK_MAX; taskCnt++)
        {
            currentTask = &sched.task[taskCnt];
            /*Is elegible to decrease delay?*/
            if ((currentTask->delay > 0) && ((currentTask->status == sch_waiting) || (currentTask->status == sch_delayed)))
            {
                if (tickShadow > currentTask->delay) {currentTask->delay = 0;} else { (currentTask->delay) -= tickShadow; }
                /*The time has expired, pass the task to running*/
                if (currentTask->delay == 0) { currentTask->status = sch_running; }
            }
        }
        
        /*Update the system Tick*/
        intProtectTick();
        timer.ticks -= tickShadow;
        intUnProtectTick();
        clrwdt();
    }
    
    /*3.Run elegible task to run*/
    noneExecuted = 1;
    if (sched.currentTCB >= SCHED_TASK_MAX) {  sched.currentTCB = 0; };
    do
    {
        currentTask = &sched.task[sched.currentTCB];
        /*Is there a Task Loaded?*/
        if (currentTask->taskPointer != NULL)
        {
            if (currentTask->status == sch_running)
            {
                /*Execute Task*/
                currentTask->taskPointer(&(currentTask->step));
                #if SCHED_REENTRY != 0
                noneExecuted = 0;
                #endif
            }
        }
    }
    while ((++sched.currentTCB < SCHED_TASK_MAX) && noneExecuted);
}

char schedSignalSem( eventType event )
{
    intProtectSem();
    if (sched.event[event - 1] <= EVENT_COUNTER_MAX)
    {
        (sched.event[event - 1])++;
        intUnProtectSem();
        return 0;
    }
    intUnProtectSem();
    return 1;
}

char schedWait( delayType delay, eventType event )
{
taskType *currentTask;
    
    currentTask = &sched.task[sched.currentTCB];
    sched.task[sched.currentTCB].delay = delay;
    sched.task[sched.currentTCB].eventPointer = event;
    
    intProtectSem();
    if (sched.event[(currentTask->eventPointer) - 1] == 0)
    {
        sched.task[sched.currentTCB].status = sch_waiting;
        intUnProtectSem();
        return 0;
    }
    (sched.event[(currentTask->eventPointer) - 1])--;
    intUnProtectSem();
    sched.task[sched.currentTCB].status = sch_running;    
    return 1;
}

char schedTry( eventType event )
{
    intProtectSem();
    if (sched.event[event - 1] > 0)
    {
        (sched.event[event - 1])--;
        intUnProtectSem();
        return 1;
    }
    intUnProtectSem();
    return 0;
}

char schedTaskCreate( taskPointerType task, taskCountType TaskNo )
{
    if (sched.task[TaskNo - 1].status == sch_destroyed)
    {
        schedTaskReCreate(task, TaskNo);
        return 1;
    }
    else {return 0;}
}

void schedTaskReCreate( taskPointerType task, taskCountType TaskNo )
{
    sched.task[TaskNo - 1].status = sch_running;
    sched.task[TaskNo - 1].taskPointer = task;
    sched.task[TaskNo - 1].eventPointer = NULL;
    sched.task[TaskNo - 1].delay = 0;
    sched.task[TaskNo - 1].step = 1;    
}
