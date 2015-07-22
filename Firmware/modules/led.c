/* Led control Library 

Autor: Cristian Alberoni*/

/*Required Header Files*/
#include "msp430g2553.h"

/*Led Header File*/
#include "led.h"
#include "ControlRemVHF_cfg.h"
 
/*Methods */

/*Led Module Initialization */
void vLedInit(void){
  LED_PDIR |= LED_PIN;
  LED_POUT &= ~LED_PIN;
  
}

void vLedToggle(void){
  LED_POUT ^= LED_PIN;  
}

