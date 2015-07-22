/*Scheduler Configurations*/

#ifndef sched_cfg_h__
#define sched_cfg_h__

/*Amount of Tasks*/
#define SCHED_TASK_MAX  4

/*Amount of events*/
#define SCHED_EVENT_MAX 6

/*Size of the counting events*/
#define SCHED_EVENT_SIZE 1

/*Size of Delays*/
#define SCHED_DELAY_SIZE 2

/*Size of Steps*/
#define SCHED_STEP_SIZE 1

/*Force scheduler reentry in each task ctx*/
#define SCHED_REENTRY 1

/*Interrupt protection Level*/
#define SCHED_PROTECT_LEVEL LOW

/*Priority Task levels*/
#define SCHED_PRIORITY_LEVELS 0

#endif // sched_cfg_h__
