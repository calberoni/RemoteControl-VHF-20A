/* Shift Register Library for the Remote controller
Shift Register chip: 74595
Autor: Cristian Alberoni
*/

#include "shiftReg.h"
#include "ControlRemVHF_cfg.h"
#include "..\sched.h"

void shiftRegPutFrecu(int FrecMHz, int FrecKHz)
{
  /*- Desarma FrecMHz y FrecKHz en unidad de MHz, Decena de MHz, centena de KHz y decena de KHz
    - Da salida segun codificacion de 2 de 5.
  */
  char decMHz=0, uniMHz=0, centKHz=0, decKHz=0, auxDecKHz=0;
  int valorSalida=0;
  
  /*Arreglos que contienen valores de salida según el 2 de 5 para equipo de VHF*/
  char decMegas[]={0x0C,0x0A,0x09,0x05,0x03,0xFF,0x00,0x0E};  
  char uniMegas[]={0x0D,0x1C,0x1A,0x19,0x15,0x13,0x0B,0x07,0x16,0x0E,0xFF,0x00,0xFE};  //Se agrega 0xFF --> prende todo, Se agrega 0x00 --> prende todo
  char cientosKilos[]={0x0D,0x1C,0x1A,0x19,0x15,0x13,0x0B,0x07,0x16,0x0E,0xFF,0x00,0xFE};  //Se agrega 0xFF --> prende todo, Se agrega 0x00 --> prende todo
  char decenKilos[]={0x03,0x02,0x00,0x01};
   
  
  decMHz = (FrecMHz/10)%10;
  uniMHz = FrecMHz%10;
  centKHz = (FrecKHz/100)%10;
  decKHz = (FrecKHz/10)%10;
  
  /* Armado de auxDecKHz segun los valores de decKHz --> 00,25,50,75*/
  
  switch(decKHz)
  {
  case CERO:
    auxDecKHz = decenKilos[0];
    break;
    
  case DOS:
    auxDecKHz = decenKilos[1];  
    break;
    
  case CINCO:
    auxDecKHz = decenKilos[2];
    break;
    
  case SIETE:
    auxDecKHz = decenKilos[3];
    break;
  }
  
/* Armo el valorSalida --> los 2 bytes*/  
 
 valorSalida = ((int)(decMegas[decMHz] << 12)) + ((int)(uniMegas[uniMHz] << 7)) + ((int)(cientosKilos[centKHz] << 2)) + ((int)auxDecKHz);
 
 shiftRegWrite2Byte(valorSalida);
}

void shiftRegInit(void)
{
  SFTR_SER_PDIR |= SER;
  SFTR_RCLK_PDIR |= RCLK;
  SFTR_SRCLK_PDIR |= SRCLK;
  
  SFTR_SER_PSEL &= ~SER;
  SFTR_SER_PSEL2 &= ~SER;
  
  shiftRegClear(); 
}

void shiftRegClear(void)
{
  shiftRegWriteByte(0);
}

void shiftRegWriteByte(char valor )
{
  char i=0;
  char bitSalida[8];
  
  SFTR_RCLK_POUT &= ~RCLK;      //LATCH LOW
  schedDelay(1);
  
  for (i=0; i<=7;i++)
  {
    bitSalida[i] = valor & 0x01;
    valor = valor >>1;
    SFTR_SER_POUT |= bitSalida[i] << SER;
    SFTR_SRCLK_POUT |= SRCLK;   //CLOCK HIGH
  //  schedDelay(1);
    
    SFTR_SRCLK_POUT &= ~SRCLK;  //CLOCK LOW
 //   schedDelay(1);
    SFTR_RCLK_POUT |= RCLK;    //LATCH HIGH
  }
}

void shiftRegWrite2Byte(int integer)
{
  char aux=0;
  
  aux = (char)((integer >> 7) & 0xff);
  shiftRegWriteByte(aux);
  aux = (char)(integer & 0xff);
  shiftRegWriteByte(aux);
}

void shiftRegTest()
{
  shiftRegWrite2Byte(0xffff);
  schedDelay(1000);
  shiftRegWrite2Byte(0x0000);
  schedDelay(1000);
}