/* GPIO configuration file for the VHF20-A remote controller

Autor: Cristian Alberoni*/

#include "msp430g2553.h"

#ifndef CONTROLREMVHF_CFG_H__
#define CONTROLREMVHF_CFG_H__

/* Buttons Definitions */
#define BUTTON1         BIT4
#define BUTTON2         BIT5
#define BUTTON3         BIT0
#define BUTTON4         BIT1
#define BUT1_PIN        P1IN
#define BUT2_PIN        P1IN
#define BUT3_PIN        P2IN
#define BUT4_PIN        P2IN
#define BUT1_PDIR       P1DIR
#define BUT2_PDIR       P1DIR
#define BUT3_PDIR       P2DIR
#define BUT4_PDIR       P2DIR
#define BUT1_PREN       P1REN
#define BUT2_PREN       P1REN
#define BUT3_PREN       P2REN
#define BUT4_PREN       P2REN
#define BUT1_PSEL       P1SEL
#define BUT2_PSEL       P1SEL 
#define BUT3_PSEL       P2SEL
#define BUT4_PSEL       P2SEL 
#define BUT1_PSEL2      P1SEL2
#define BUT2_PSEL2      P1SEL2
#define BUT3_PSEL2      P2SEL2
#define BUT4_PSEL2      P2SEL2 

#define BUT1_PIES       P1IES
#define BUT2_PIES       P1IES
#define BUT3_PIES       P2IES
#define BUT4_PIES       P2IES

#define BUT1_PIFG       P1IFG
#define BUT2_PIFG       P1IFG
#define BUT3_PIFG       P2IFG
#define BUT4_PIFG       P2IFG

#define BUT1_PIE        P1IE
#define BUT2_PIE        P1IE
#define BUT3_PIE        P2IE
#define BUT4_PIE        P2IE

/* LCD Display Definitions */ 
#define PORT_EN         P2OUT
#define PORT_RS         P2OUT
#define EN              BIT3
#define RS              BIT2
#define D4              BIT4
#define D5              BIT5
#define D6              BIT3
#define D7              BIT2
#define LCD_POUT1       P1OUT
#define LCD_POUT2       P2OUT
#define LCD_PDIR1       P1DIR
#define LCD_PDIR2       P2DIR

/* LED Definitions */
#define LED_POUT        P1OUT
#define LED_PDIR        P1DIR
#define LED_PIN         BIT7

/* Shift Register Definitions */
#define SER             BIT6    //DATA
#define RCLK            BIT0    //LATCH
#define SRCLK           BIT1    //CLOCK
#define SFTR_SER_PDIR   P2DIR
#define SFTR_RCLK_PDIR  P1DIR
#define SFTR_SRCLK_PDIR P1DIR
#define SFTR_SER_POUT   P2OUT
#define SFTR_RCLK_POUT  P1OUT
#define SFTR_SRCLK_POUT P1OUT
#define SFTR_SER_PSEL   P2SEL
#define SFTR_RCLK_PSEL  P1SEL
#define SFTR_SRCLK_PSEL P1SEL
#define SFTR_SER_PSEL2  P2SEL2
#define SFTR_RCLK_PSEL2 P1SEL2

/* Buttons Configurations*/

/* LCD Display Configurations*/

/* LED Configurations*/

/* Shift Register Configurations*/

#endif // controlremvhf_cfg_h__