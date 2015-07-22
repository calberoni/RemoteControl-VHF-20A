/* LCD Display 16x1 and 16x2 Library

Autor: Cristian Alberoni
*/

#include "lcd3.h"
#include "ControlRemVHF_cfg.h"
#include "..\sched.h"
#include <string.h>

   
void lcdcmd(unsigned char Data)
{

lcdRSOff(); //because sending command
lcdENOff();

lcdPortDataOut((Data >> 4) & 0x0F);
lcdENOn();
schedDelay(1);
//waitlcd(2);
lcdENOff();

lcdPortDataOut(Data & 0x0F);
lcdENOn();
schedDelay(1);
//waitlcd(2);
lcdENOff();


}
void lcdData(unsigned char d)
{
lcdRSOn();  //because sending data
lcdENOff();
lcdPortDataOut((d >> 4) & 0x0F);

lcdENOn();
//waitlcd(2);
schedDelay(1);
lcdENOff();
lcdPortDataOut(d & 0x0F);
lcdENOn();
schedDelay(1);
//waitlcd(2);
lcdENOff();



}

void lcdInit(void)
{
  //Port config
  lcd_port_dir_conf();
   
waitlcd(500);
lcdRSOff();
lcdENOff();
lcdPortDataOut(0x03);
waitlcd(40);
lcdENOn();
lcdENOff();
waitlcd(5);
lcdENOn();
lcdENOff();
waitlcd(5);
lcdENOn();
lcdENOff();
waitlcd(2);

lcdPortDataOut(0x02);
lcdENOn();
lcdENOff();
lcdcmd(0x28);   //set data length 4 bit 2 line
waitlcd(250);
lcdcmd(0x0E);   // set display on cursor on blink on
waitlcd(250);
lcdcmd(0x01); // clear lcd
waitlcd(250);
lcdcmd(0x06);  // cursor shift direction
waitlcd(250);
lcdcmd(0x80);  //set ram address
waitlcd(250);
}

void waitlcd(volatile unsigned int x)
{
volatile unsigned int i;
for (x ;x>1;x--)
{
for (i=0;i<=110;i++);
}
}

int prints(char *s)
  {
int largo = strlen(s);
    while (*s)
      {
	 lcdData(*s);
	 s++;
      }
    return largo;
  }

void gotoXy(unsigned char  x,unsigned char y)
{
 if(x<40)
 {
  if(y) x |= 0x40;
  x |=0x80;
  lcdcmd(x);
  }

}
void integerToAscii(unsigned int integer, char * character )
{
  character[0]=(integer / 100) + 0x30;
  character[1]=(integer%100)/10 + 0x30;
  character[2]=integer%10 + 0x30;      
}

void printFrecu(unsigned int megas, unsigned int kilos)
{
    //IMPORTANTE!!! PENDIENTE ->Tener en cuenta de imprimir 0xx en ascii cuando FrecKHz sea 0, 025,050 y 075
  char sMegas[3];
  char sKilos[3];
  
  integerToAscii(megas,sMegas);
  integerToAscii(kilos,sKilos);
    
 /*Armado de Visualizacion en LCD
  Fr. 1xx,xxx MHz --> prints("Fr. 116.525 MHz ");
  */ 
  gotoXy(0,0);      
  prints("Fr. ");
  gotoXy(4,0);
  prints(sMegas);
  gotoXy(7,0);
  prints(".");
  gotoXy(0,1);
  prints(sKilos);
  gotoXy(3,1);
  prints(" MHz ");
}