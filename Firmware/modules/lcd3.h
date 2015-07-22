/* LCD Display 16x1 and 16x2 Header file

Autor: Cristian Alberoni
*/

#ifndef LCD3_H_
#define LCD3_H_

void waitlcd(unsigned int x);

void lcdInit(void);
void integerToAscii(unsigned int integer, char * character );
void lcdData(unsigned char l);
int prints(char *s);
void gotoXy(unsigned char  x,unsigned char y);
void printFrecu (unsigned int, unsigned int);

/*Macros*/
#define lcdPortDataOut(x) LCD_POUT2 &=~(D4+D5); LCD_POUT1 &=~(D6+D7); LCD_POUT2 |= ((x & 0x01)<< 4) + ((x & 0x02)<< 4); LCD_POUT1 |= ((x & 0x04)<<1) + ((x & 0x08)>> 1); 
#define lcdENOn() PORT_EN |= EN
#define lcdENOff() PORT_EN &= ~EN
#define lcdRSOn() PORT_RS |= RS
#define lcdRSOff() PORT_RS &= ~RS
#define lcd_port_dir_conf() LCD_PDIR1 |=(D7 + D6); LCD_PDIR2 |=(D4 + D5 + RS + EN)

#endif /* LCD3_H_ */
