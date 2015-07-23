/* Shift register Header file

Autor: Cristian Alberoni
*/
#ifndef SHIFTREG_H_
#define SHIFTREG_H_

#define CERO            0
#define UNO             1
#define DOS             2
#define TRES            3
#define CUATRO          4
#define CINCO           5
#define SEIS            6
#define SIETE           7
#define OCHO            8
#define NUEVE           9
#define VIENTI5         25
#define SETENTAY5       75
#define CINCUENTA       50

void shiftRegInit(void);
void shiftRegPutFrecu(int FrecMHz, int FrecKHz); 
void shiftRegClear(void);
void shiftRegWriteByte(char);
void shiftRegWrite2Byte(int integer);
void shiftRegTest(void);

#endif /* SHIFTREG_H_ */