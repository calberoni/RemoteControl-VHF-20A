/*Main Program Module*/

/*Required Header Files*/
#include "msp430g2553.h"
#include "main.h"

/*Modules Header files*/
#include "timer.h"
#include "sched.h"
#include "modules\lcd3.h"
#include "modules\led.h"
#include "modules\buttons.h"
#include "modules\ControlRemVHF_cfg.h"
#include "modules\shiftReg.h"

/*Methods declaration (This can be placed in another module)*/
void vTaskLed( stepType *step );
void vTaskProcesarSalida( stepType *step );
void vTaskProcesarTestMode( stepType *step );

/*RTOS Task pointers*/
#define	TCBP_TASK_LED  4
#define TCBP_TASK_PROCESAR_SALIDA       2
#define TCBP_TASK_PROCESAR_TESTM        3

#define DEBOUNCE        10

/*Global Vars*/
  int contInt1ms=0;
  int contInt4ms=0;
  int contInt10ms=0;
  int contInt20ms=0;
  char button1Fg=0;
  char button2Fg=0;
  char button3Fg=0;
  char button4Fg=0;
  
  int FrecMHz=116;
  int FrecKHz=525;
  
  int delayLedTicks = 2000;
  
  float timeCount=0;
  float lastTime = 0;
  float currentTime = 0;
  
/*Main Code*/
void main( void )
{
//   BCSCTL1 |= CAL_BC1_1MHZ;// Use 1Mhz cal data for DCO
//   DCOCTL |= CAL_DCO_1MHZ;// Use 1Mhz cal data for DCO 
  /*Scheduler Init*/
    schedInit();

    /*Modules Initialization*/
    vTimerInit(XT_INTERNAL, 1000);
    vLedInit();
    buttonInit();
    shiftRegInit();
    lcdInit();

    /*Services Initialization*/
    schedTaskCreate(vTaskLed, TCBP_TASK_LED);
    schedTaskCreate(vTaskProcesarSalida, TCBP_TASK_PROCESAR_SALIDA);
 
    /*Init semaphores*/
    schedEventInit(6,0); //Semaphore number, semaphore state (0 = unsignaled)

    /*Interrups Initialization*/
    intEnablePriority(ON);
    intLowEnable(ON);
    intHighEnable(ON);
   
    /*Scheduler*/
    for ( ;; )
    {
        schedule();
    }
}


/* Interrupt Hook*/
#pragma vector= TIMER0_A0_VECTOR
__interrupt void hiIntHook(void){
    vTimerISR();
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void lowTimerIntHook(void)
{
    timeCount++;
}

#pragma vector = PORT1_VECTOR
__interrupt void port1IntHook (void)
{
  if (BUT1_PIFG & BUTTON1)
  {
    if (schedTaskState(TCBP_TASK_PROCESAR_TESTM) == sch_destroyed) schedTaskStart(TCBP_TASK_PROCESAR_SALIDA);        //comienzo tarea
    if ((!button1Fg) && (schedTaskState(TCBP_TASK_PROCESAR_TESTM) == sch_destroyed))                                     //Primera vez que se presiona
    {
      if ((timeCount - lastTime) >= DEBOUNCE) lastTime = timeCount;
          button1Fg=HIGH;     
    }
    
    //else {      button1Fg = LOW;}                      //Resetear variables buttonXFg
    BUT1_PIFG &=~BUTTON1;
  }
  if (BUT2_PIFG & BUTTON2)
  {
    if (schedTaskState(TCBP_TASK_PROCESAR_TESTM) == sch_destroyed) schedTaskStart(TCBP_TASK_PROCESAR_SALIDA);
    if ((!button2Fg) && (schedTaskState(TCBP_TASK_PROCESAR_TESTM) == sch_destroyed))
    {
      if ((timeCount - lastTime) >= DEBOUNCE) lastTime = timeCount;
      button2Fg=HIGH;
    }
    BUT1_PIFG &=~BUTTON2;
  }
}

#pragma vector = PORT2_VECTOR
__interrupt void port2IntHook (void)
{
    if (BUT3_PIFG & BUTTON3)
  {
    if (schedTaskState(TCBP_TASK_PROCESAR_TESTM) == sch_destroyed) schedTaskStart(TCBP_TASK_PROCESAR_SALIDA);
    if ((!button3Fg) && (schedTaskState(TCBP_TASK_PROCESAR_TESTM) == sch_destroyed))
    {
      button3Fg=HIGH;
      if ((timeCount - lastTime) >= DEBOUNCE) lastTime = timeCount;
    }
    BUT3_PIFG &=~BUTTON3;
  }
  if (BUT4_PIFG & BUTTON4)
  {
    if (schedTaskState(TCBP_TASK_PROCESAR_TESTM) == sch_running) {schedTaskDestroy(TCBP_TASK_PROCESAR_TESTM);}
    else { schedTaskStart(TCBP_TASK_PROCESAR_SALIDA);}
    if (!button4Fg)
    {
      button4Fg=HIGH;
      if ((timeCount - lastTime) >= DEBOUNCE) lastTime = timeCount;
    }
    BUT4_PIFG &=~BUTTON4;
  } 
}

void vTaskLed( stepType *step )
{
    vLedToggle();
    schedDelay(delayLedTicks);
}

/**************************************
Tarea que incrementa o decrementa frecuencia, imprime
en LCD los valores y carga en el shift register para dar
salida de los valores actuales
***************************************/
void vTaskProcesarSalida( stepType *step )
{
  delayLedTicks = 2000;

      if (button1Fg && button3Fg && ((timeCount - lastTime) >= DEBOUNCE) ) 
    {
      button1Fg = LOW;
      button3Fg = LOW;
      schedTaskCreate(vTaskProcesarTestMode,TCBP_TASK_PROCESAR_TESTM);
      schedTaskCurrStop();
    }
    if (button1Fg && ((timeCount - lastTime) >= DEBOUNCE))
    {
      button1Fg = LOW;
      FrecMHz++;
      if (FrecMHz >= 150) FrecMHz=150;
    }
    if (button2Fg && ((timeCount - lastTime) >= DEBOUNCE))
    {

      button2Fg = LOW;
      FrecMHz--;
      if (FrecMHz <= 116) FrecMHz=116;
    }
    if (button3Fg && ((timeCount - lastTime) >= DEBOUNCE))
    {

      button3Fg = LOW;
      FrecKHz = FrecKHz + 25;   /* Ingrementamos frecuencia con un paso de 25*/
      if (FrecKHz > 975) FrecKHz = 0; 
    }
    if (button4Fg && ((timeCount - lastTime) >= DEBOUNCE))
    {
      button4Fg = LOW;
      FrecKHz = FrecKHz - 25;   /* Decrementamos frecuencia con un paso de 25 */
      if (FrecKHz < 0) FrecKHz = 975;
    }

    shiftRegPutFrecu(FrecMHz,FrecKHz);  //Dar salida de valores
    printFrecu(FrecMHz,FrecKHz);        //Imprimo en pantalla valores
    schedTaskCurrStop();
}

/*

*/

void vTaskProcesarTestMode( stepType *step )
{
  button1Fg = LOW;                      //Resetear variables buttonXFg
  button2Fg = LOW;
  button3Fg = LOW;
  button4Fg = LOW;
  if (button4Fg && ((timeCount - lastTime) >= DEBOUNCE)) schedTaskCurrDestroy();
  
  delayLedTicks = 250; // Modifico tiempo de blink para indicar que estoy en modo test
  shiftRegWrite2Byte(0x0000);
  schedDelay(1000);
  shiftRegWrite2Byte(0xffff);
  schedDelay(1000);
  
  gotoXy(0,0);
  prints("** Test ");
  gotoXy(0,1);
  prints("Mode ** ");
  shiftRegTest();


}
