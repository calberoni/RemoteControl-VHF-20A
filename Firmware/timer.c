/*Timer Module*/
/*Interrupt driven*/

/*Required Header Files*/
#include "msp430g2553.h"

/*Timer header file*/
#include "timer.h"

/*Global Variable definition*/
struct tTimerData timer;

/*Methods*/
/*Timer Module Initialization*/
void vTimerIntInit( unsigned int value )
{
    /*Set Timer Data*/
    timer.ticks = 0;
    timer.status.overflow = FALSE;

__disable_interrupt();
  /*Interrupt Timer 0 initialitation */
  TACCR0 = value; //Number of cycles in the timer
  TACTL |= TASSEL_2; //Use SMCLK as source for timer
  TACTL |= MC_1; //Use UP mode timer
  TACTL |= ID_3; //Divisor por 8
 // TA0IV |= TA0IV_TACCR1; //Highest priority
  
  /*Interrupt Timer 1 initialitation*/
    TA1CTL |= TASSEL_2; //Use SMCLK as source for timer
    TA1CTL |= MC_1; //Use UP mode timer
    TA1CTL |= ID_3; //Divisor por 8
    TA1CCR0 = 125;
    TA1CCTL0 |= CCIE;
  
    /*Enable CCIE Interrupts*/
    TACCTL0 |= CCIE;
  __enable_interrupt();
}
