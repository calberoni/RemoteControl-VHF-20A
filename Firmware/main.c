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
//void vTaskLcdPrint( stepType *step );



/*RTOS Task pointers*/
#define	TCBP_TASK_LED  4
//#define	TCBP_TASK_LCD_CONFIG  2
#define TCBP_TASK_PROCESAR_SALIDA     3

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
 //   schedTaskCreate(vTaskLcdPrint,TCBP_TASK_LCD_PRINT);
    
    
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
__interrupt void lowTimerIntHook(void){
  
  if (contInt20ms >=20){
    contInt20ms = 0;
    //Habilitar semaforo EVENT_20MS
    //schedSignalSemInt(EVENT_20MS);
  }
  else{
    contInt20ms++;
    if(contInt10ms >=10){
      contInt10ms = 0;
      //Habilitar semaforo EVENT_10MS
      //schedSignalSemInt(EVENT_10MS);
    }
    else{
      contInt10ms++;
      if(contInt4ms >= 4){
        contInt4ms = 0;
        //Habilitar semafor EVENT_4MS
        //schedSignalSemInt(EVENT_4MS);
      }
      else{
        if (contInt1ms >= 1){
          contInt1ms =0;
          //Habilitar semaforo EVENT_1MS
         // schedSignalSemInt(EVENT_1MS);
        }
          else{
            contInt1ms++;
          }
      }
    }
  }
  
}

#pragma vector = PORT1_VECTOR
__interrupt void port1IntHook (void)
{
  if (BUT1_PIFG & BUTTON1)
  {
    if (!button1Fg)                                     //Primera vez que se presiona
    {
      button1Fg=HIGH;
      schedTaskStart(TCBP_TASK_PROCESAR_SALIDA);        //crear la tarea
      //schedSignalSemInt(); //Despierto tarea Procesar_salida
    }
    BUT1_PIFG &=~BUTTON1;
  }
  if (BUT2_PIFG & BUTTON2)
  {
    if (!button2Fg)
    {
      button2Fg=HIGH;
      schedTaskStart(TCBP_TASK_PROCESAR_SALIDA); 
      //schedSignalSemInt(); //Despierto tarea Procesar_salida
    }
    BUT1_PIFG &=~BUTTON2;
  }
}

#pragma vector = PORT2_VECTOR
__interrupt void port2IntHook (void)
{
    if (BUT3_PIFG & BUTTON3)
  {
    if (!button3Fg)
    {
      button3Fg=HIGH;
      schedTaskStart(TCBP_TASK_PROCESAR_SALIDA); 
      //schedSignalSemInt(); //Despierto tarea Procesar_salida
    }
    BUT3_PIFG &=~BUTTON3;
  }
  if (BUT4_PIFG & BUTTON4)
  {
    if (!button4Fg)
    {
      button4Fg=HIGH;
      schedTaskStart(TCBP_TASK_PROCESAR_SALIDA); 
      //schedSignalSemInt(); //Despierto tarea Procesar_salida
    }
    BUT4_PIFG &=~BUTTON4;
  } 
}

void vTaskLed( stepType *step )
{
    /*Example Task with a 100ms delay an a signaling of a semaphore*/
//    schedSignalSem(ECBP_TASK_DONE);
    vLedToggle();
    schedDelay(2000);
    
}


/*
void vTaskLcdConf( stepType *step )
{
  //This task configures the LCD display one time 
  //schedDelay(1000);
  lcd_init ();
//  lcdConfInit();
  schedSignalSem(ECBP_TASK_DONE);
  schedTaskCurrDestroy();  
}
void vTaskLcdPrint( stepType *step){
  schedWait(5000, ECBP_TASK_DONE);
  lcd_cmd(0x80);
  display_line("ABCDEFGHIJKMNLOP");
  
//  lcdPrint("ABCDEFGHIJKMNLOP");
//  schedDelay(2000);
//  lcdGotoXy(0,1);
//  lcdPrint(" 1");
//  schedDelay(2000);
//  lcdPrint(" 2");
  schedTaskCurrDestroy();
}
*/

/**************************************
Tarea que incrementa o decrementa frecuencia, imprime
en LCD los valores y carga en el shift register para dar
salida de los valores actuales
***************************************/
void vTaskProcesarSalida( stepType *step )
{
    if (button1Fg)
    {
    //Si activo - dormir tarea por xx tiempo de antirrebote o ver la forma medir tiempo e incrementar variables
      FrecMHz++;
      if (FrecMHz >= 150) FrecMHz=150;
    }
    if (button2Fg)
    {
    //Si activo - dormir tarea por xx tiempo de antirrebote o ver la forma medir tiempo e incrementar variables
      FrecMHz--;
      if (FrecMHz <= 116) FrecMHz=116;
    }
    if (button3Fg)
    {
    //Si activo - dormir tarea por xx tiempo de antirrebote o ver la forma medir tiempo e incrementar variables
      FrecKHz = FrecKHz + 25;   /* Ingrementamos frecuencia con un paso de 25*/
      if (FrecKHz > 975) FrecKHz = 0; 
    }
    if (button4Fg)
    {
    //Si activo - dormir tarea por xx tiempo de antirrebote o ver la forma medir tiempo e incrementar variables
      FrecKHz = FrecKHz - 25;   /* Decrementamos frecuencia con un paso de 25 */
      if (FrecKHz < 0) FrecKHz = 975;
    }
    printFrecu(FrecMHz,FrecKHz);        //Imprimo en pantalla valores

    shiftRegPutFrecu(FrecMHz,FrecKHz);  //Dar salida de valores
    
    button1Fg = 0;                      //Resetear variables buttonXFg
    button2Fg = 0;
    button3Fg = 0;
    button4Fg = 0;
    schedTaskCurrStop();
}