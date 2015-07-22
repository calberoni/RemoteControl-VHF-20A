/* Buttons Library

Autor: Cristian Alberoni
*/

#include "ControlRemVHF_cfg.h"
#include "buttons.h"


void buttonInit(void)
{
  BUT1_PDIR &= ~BUTTON1;
  BUT2_PDIR &= ~BUTTON2;
  BUT3_PDIR &= ~BUTTON3;
  BUT4_PDIR &= ~BUTTON4;
  
  BUT1_PREN &= ~BUTTON1;
  BUT2_PREN &= ~BUTTON2;
  BUT3_PREN &= ~BUTTON3;
  BUT4_PREN &= ~BUTTON4;
  
  BUT1_PSEL &= ~BUTTON1;  
  BUT2_PSEL &= ~BUTTON2;
  BUT3_PSEL &= ~BUTTON3;
  BUT4_PSEL &= ~BUTTON4;
  
  BUT1_PSEL2 &= ~BUTTON1; 
  BUT2_PSEL2 &= ~BUTTON2;
  BUT3_PSEL2 &= ~BUTTON3;
  BUT4_PSEL2 &= ~BUTTON4;
 
  /*Flanco descendente*/ 
  BUT1_PIES |= BUTTON1;
  BUT2_PIES |= BUTTON2;  
  BUT3_PIES |= BUTTON3;
  BUT4_PIES |= BUTTON4;
  
  //Interrupt Buttons configuration
  BUT1_PIFG &= ~BUTTON1;    
  BUT2_PIFG &= ~BUTTON2;      
  BUT3_PIFG &= ~BUTTON3;      
  BUT4_PIFG &= ~BUTTON4;
  
  BUT1_PIE |= BUTTON1;
  BUT2_PIE |= BUTTON2;
  BUT3_PIE |= BUTTON3;
  BUT4_PIE |= BUTTON4;
}

