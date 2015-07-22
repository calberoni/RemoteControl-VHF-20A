/*Timer header file*/

#ifndef timer_h__
#define timer_h__

/*Required header files*/

/*Configurations*/

/*Tick counter size*/
#define TICK_COUNTER_SIZE  2 

/*Constants definition*/
#ifndef Mhz
#define Mhz 000000
#endif

/*Status definition*/
#define IN              1u
//#define OUT             0u
#define ON              1u
#define OFF             0u
#define HIGH            1u
#define LOW             0u
#define EXTERNAL        1u
#define INTERNAL        0u
#define TRUE            1u
#define FALSE           0u

/*Divider definition*/
#define DIVIDER_ID1     1
#define DIVIDER_ID2     2
#define DIVIDER_ID4     4
#define DIVIDER_ID8     8

/*Functions enabling*/

/*typedefs*/
#ifndef TICK_COUNTER_SIZE
#define TICK_COUNTER_SIZE 1
#endif

#if     TICK_COUNTER_SIZE == 1
typedef unsigned char tickType;
#define TICK_COUNTER_MAX    0xFE
#elif   TICK_COUNTER_SIZE == 2
typedef unsigned int tickType;
#define TICK_COUNTER_MAX    0xFFFE
#else
#error "Only 8 bits and 16 bits tick counters allowed"
#endif

typedef struct
{
    unsigned char overflow:1,:7;
} tStatus;

struct tTimerData
{
    volatile tickType ticks;
    tStatus status;
};

extern struct tTimerData timer;

/*Methods Declaration*/
void vTimerIntInit( unsigned int value );

/*Macro definitions*/
#define vTimerInit( clockHZCPU, clockTickRate)  vTimerIntInit( (clockHZCPU / DIVIDER_ID8) / clockTickRate)
#define vTimerDirection(x, y) x = y
#define vTimerLevel(x, y) x = y

/*Timer Interrupt Hook*/
#define vTimerISR(){ do { TACCR0 &= ~CCIFG; if (timer.ticks++ >= TICK_COUNTER_MAX){timer.ticks = TICK_COUNTER_MAX; timer.status.overflow = TRUE;}} while (0);}

#endif // timer_h__
