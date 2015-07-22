/*Scheduler header file
  Scheduler module
  Developer: Marcelo Lorenzati
  License: GPL v3
 */
#ifndef sched_h__
#define sched_h__

/*Scheduler configuration header files*/
#include "sched_cfg.h"

/*Constants definition*/

/*Status definition*/
#define NULL 0u
#ifndef ONOFF
#define ONOFF
#define ON  1u
#define OFF 0u
#endif
#define HIGH    1u
#define MID     2u
#define LOW     0u

/*By default values*/

#ifndef SCHED_EVENT_SIZE
#define SCHED_EVENT_SIZE 1
#endif

#ifndef SCHED_DELAY_SIZE
#define SCHED_DELAY_SIZE 1
#endif

#ifndef SCHED_REENTRY
#define SCHED_REENTRY 0
#endif

#ifndef SCHED_STEP_SIZE
#define SCHED_STEP_SIZE 1
#endif

#ifndef SCHED_PROTECT_LEVEL
#define SCHED_PROTECT_LEVEL HIGH
#endif

#ifndef SCHED_PRIORITY_LEVELS
#define SCHED_PRIORITY_LEVELS 0
#endif

/*typedefs*/

/*Event Type definition*/
#if     SCHED_EVENT_SIZE == 1
typedef unsigned char eventType;
#define EVENT_COUNTER_MAX    0xFE
#elif   SCHED_EVENT_SIZE == 2
typedef unsigned int eventType;
#define EVENT_COUNTER_MAX    0xFFFE
#else
#error "Only 8 bits and 16 bits event counters allowed"
#endif

/*Delay Type definition*/
#if     SCHED_DELAY_SIZE == 1
typedef unsigned char delayType;
#define DELAY_COUNTER_MAX    0xFE
#elif   SCHED_DELAY_SIZE == 2
typedef unsigned int delayType;
#define DELAY_COUNTER_MAX    0xFFFE
#else
#error "Only 8 bits and 16 bits delay counters allowed"
#endif

/*Step Type Definition*/
#if SCHED_STEP_SIZE == 1
typedef unsigned char stepType;
#elif   SCHED_STEP_SIZE == 2
typedef unsigned int stepType;
#else
#error "Only 8 bits and 16 bits step counters allowed"
#endif


/*Other Error Reporting*/
#if     SCHED_TASK_MAX < 255
typedef unsigned char taskCountType;
#else
typedef unsigned int taskCountType;
#endif

#if     SCHED_EVENT_MAX < 255
typedef unsigned char eventCountType;
#else
typedef unsigned int eventCountType;
#endif

#if SCHED_PRIORITY_LEVELS > 0
#error "Functionality not programmed yet."
#endif

#if    SCHED_PRIORITY_LEVELS < 255
typedef unsigned char priorityType;
#else
typedef unsigned int priorityType;
#endif

#if     SCHED_REENTRY > 1
#error "Only 1(ON) or 0(OFF) allowed for SCHED_REENTRY"
#endif

/*Typedefs*/
typedef void (*taskPointerType)( stepType *step );

typedef enum
{
    sch_destroyed,
    sch_stopped,
    sch_delayed,
    sch_waiting,
    sch_running
}
statusType;

typedef struct 
{
    taskPointerType taskPointer;
    eventCountType eventPointer;
    delayType delay;
    statusType status;
    stepType step;
    #if SCHED_PRIORITY_LEVELS > 1
        priorityType priority;
    #endif
} taskType;

struct tSchedData
{
    volatile eventType event[SCHED_EVENT_MAX];
    taskType task[SCHED_TASK_MAX];
    volatile taskCountType currentTCB;
} ;

extern struct tSchedData sched;

/*Methods Declaration*/
void schedule( void );
void schedInit( void );
char schedSignalSem( eventType event );
char schedWait( delayType delay, eventType event );
char schedTry( eventType event );
void schedTaskReCreate( taskPointerType task, taskCountType TaskNo );
char schedTaskCreate( taskPointerType task, taskCountType TaskNo );

/*Macro definitions*/
#define intEnablePriority(x) //IPEN = x //VER ADAPTAR TODOS ESTOOs!!!!!!!
#define intLowEnable(x) //PEIE = x
#define intHighEnable(x) //GIE = x

/*Interrupt protection Level*/
#if SCHED_PROTECT_LEVEL == HIGH
    #define intProtectTick()        intHighEnable(OFF);intLowEnable(OFF)
    #define intProtectSem()         intLowEnable(OFF);intHighEnable(OFF)
    #define intUnProtectTick()      intHighEnable(ON);intLowEnable(ON)
    #define intUnProtectSem()       intLowEnable(ON);intHighEnable(ON)
    #define intProtectLowInt()      intHighEnable(OFF)
    #define intUnProtectLowInt()    intHighEnable(ON)
#elif SCHED_PROTECT_LEVEL == MID
    #define intProtectTick()        intHighEnable(OFF);intLowEnable(OFF)
    #define intProtectSem()         intLowEnable(OFF);intHighEnable(OFF)
    #define intUnProtectTick()      intHighEnable(ON);intLowEnable(ON)
    #define intUnProtectSem()       intLowEnable(ON);intHighEnable(ON)
    #define intProtectLowInt()  
    #define intUnProtectLowInt()
#elif SCHED_PROTECT_LEVEL == LOW
    #define intProtectTick()        intHighEnable(OFF)
    #define intProtectSem()         intLowEnable(OFF)
    #define intUnProtectTick()      intHighEnable(ON)
    #define intUnProtectSem()       intLowEnable(ON)
    #define intProtectLowInt()
    #define intUnProtectLowInt()
#else
    #error "Only HIGH and LOW level of interrupt protection"
#endif

  // Stop watchdog timer to prevent time out reset
  

//#define clrwdt()    asm("clrwdt")
#define clrwdt() WDTCTL = WDTPW + WDTCNTCL;
#define enableWatchdog(x)   if(x==OFF) { WDTCTL = WDTPW + WDTHOLD; } //Esto está mal configurado
#define schedStep(ctx)  do { *step = ctx; } while (0)
#define schedDelay(time)\
do { sched.task[sched.currentTCB].delay = time; sched.task[sched.currentTCB].status = sch_delayed; } while (0)
#define schedSignalTimeout()    (sched.task[sched.currentTCB].delay == 0)
#define schedGetDelay(TaskNo)  sched.task[TaskNo - 1].delay
#define schedGetCurrDelay()    sched.task[sched.currentTCB].delay
#define schedEventInit(eventNo, state)   sched.event[eventNo - 1] = state
#define schedTaskStop(TaskNo) sched.task[TaskNo - 1].status = sch_stopped
#define schedTaskCurrStop() sched.task[sched.currentTCB].status = sch_stopped
#define schedTaskStart(TaskNo)    sched.task[TaskNo - 1].status = sch_running
#define schedTaskCurrDestroy() sched.task[sched.currentTCB].status = sch_destroyed; sched.task[sched.currentTCB].taskPointer = NULL
#define schedTaskDestroy(TaskNo) sched.task[TaskNo - 1].status = sch_destroyed; sched.task[TaskNo - 1].taskPointer = NULL
#define schedTaskState(TaskNo) sched.task[TaskNo - 1].status
#define schedSignalSemInt(eventNo)\
do {\
    intProtectLowInt();\
    if (sched.event[eventNo - 1] <= EVENT_COUNTER_MAX)\
    {\
         (sched.event[eventNo - 1])++;\
    }\
    intUnProtectLowInt();\
} while (0)

#endif // sched_h__


