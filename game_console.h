/*************************************************************************
Title:    Game Console Template Header File
Inital Author:   David Jahshan
Extended by : (PUT YOUR NAME HERE) (PUT YOUR STUDENT NUMBER HERE) 
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console

*************************************************************************/

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define byte unsigned char 

/*ON OFF*/
#define ON 0xFF
#define OFF 0x00
#define HIGH 0xFF
#define LOW 0x00
#define IN 0x00
#define OUT 0xFF
#define ALL 0xFF
#define TRUE 1
#define FALSE 0
#define FORWARD 0x00
#define BACK 0xFF
#define CMD_PAGE 0xB0 
#define CMD_COL_LSB 0x00
#define CMD_COL_MSB 0x10
#define MAX_PAGE 7
#define MIN_PAGE 0
#define MAX_COL 101
#define MIN_COL 0
#define MAX_ROW 63
#define MIN_ROW 0
#define INIT_BRIGHT 2
#define battThres 360
/*LCD constant*/
#define startLine 0x40
#define SEGdirect 0xA1
#define COMdirect 0xC0
#define pixelON 0xA4
#define inverseDisp 0xA6
#define biasRatio 0xA2
#define powerCont 0x2F
#define resisRatio 0x27
#define setContrast 0x81
#define setContrast2 0x0C
#define advProg 0xFA
#define advProg2 0x90
#define dispON 0xAF

/*SET and GET MACRO*/
#define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
#define GET(PORT,MASK) PORT & MASK

//Devices Direction MACROs
//#define MULTI_LINES_DIR(DIR) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)
#define BAT_LOW_LED_DIR(DIR) SET(DDRD,_BV(PD3),DIR)
#define UP_BUTTON_DIR(DIR) SET(DDRA,_BV(PA4),DIR)
#define DOWN_BUTTON_DIR(DIR) SET(DDRA,_BV(PA5),DIR)
#define LEFT_BUTTON_DIR(DIR) SET(DDRA,_BV(PA6),DIR)
#define RIGHT_BUTTON_DIR(DIR) SET(DDRA,_BV(PA7),DIR)
#define ACTION1_BUTTON_DIR(DIR) SET(DDRA,_BV(PA3),DIR)
#define ACTION2_BUTTON_DIR(DIR) SET(DDRA,_BV(PA2),DIR)
#define ACTION3_BUTTON_DIR(DIR) SET(DDRA,_BV(PA1),DIR)
#define LCDBACKLIGHT_DIR(DIR) SET(DDRD,_BV(PD4),DIR)
#define INTERRUPT_DIR(DIR) SET(DDRD,_BV(PD2),DIR)
//LCD
#define LCD_CD_DIR(DIR) SET(DDRC,_BV(PC1),DIR)
#define LCD_CS_DIR(DIR) SET(DDRC,_BV(PC0),DIR)
#define LCD_RST_DIR(DIR) SET(DDRC,_BV(PC7),DIR)

//uController
#define MOSI_DIR(DIR) SET(DDRB,_BV(PB5),DIR)
#define MISO_DIR(DIR) SET(DDRB,_BV(PB6),DIR)
#define SCK_DIR(DIR) SET(DDRB,_BV(PB7),DIR)
#define SS_DIR(DIR) SET(DDRB,_BV(PB4),DIR)

//RAM
#define RAM_CS_DIR(DIR) SET(DDRB,_BV(PB1),DIR)
#define RAM_HOLD_DIR(DIR) SET(DDRB,_BV(PB2),DIR)
#define RAM_WP_DIR(DIR) SET(DDRB,_BV(PB3),DIR)

//Devices Outputs
//#define MULTI_LINES(STATE) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)
#define UP_BUTTON_PULL(STATE) SET(PORTA,_BV(PA4),~STATE)
#define DOWN_BUTTON_PULL(STATE) SET(PORTA,_BV(PA5),~STATE)
#define LEFT_BUTTON_PULL(STATE) SET(PORTA,_BV(PA6),~STATE)
#define RIGHT_BUTTON_PULL(STATE) SET(PORTA,_BV(PA7),~STATE)
#define ACTION1_BUTTON_PULL(STATE) SET(PORTA,_BV(PA3),~STATE)
#define ACTION2_BUTTON_PULL(STATE) SET(PORTA,_BV(PA2),~STATE)
#define ACTION3_BUTTON_PULL(STATE) SET(PORTA,_BV(PA1),~STATE)
#define LCD_CD_PULL(STATE) SET(PORTC,_BV(PC1),STATE)
#define LCD_CS_PULL(STATE) SET(PORTC,_BV(PC0),STATE)
#define LCD_RST_STAT(STATE) SET(PORTC,_BV(PC7),STATE)
#define LCDBACKLIGHT(STATE) SET(PORTD,_BV(PD4),STATE)
#define BAT_LOW_LED(STATE) SET(PORTD,_BV(PD3),~STATE)
// RAM
#define RAM_CS_STAT(STATE) SET(PORTB,_BV(PB1),STATE)
#define RAM_HOLD_STAT(STATE) SET(PORTB,_BV(PB2),STATE)
#define RAM_WP_STAT(STATE) SET(PORTB,_BV(PB3),STATE)

//Devices Inputs
#define UP_BUTTON GET(PINA,_BV(PA4))
#define DOWN_BUTTON GET(PINA,_BV(PA5))
#define LEFT_BUTTON GET(PINA,_BV(PA6))
#define RIGHT_BUTTON GET(PINA,_BV(PA7))
#define ACTION1_BUTTON GET(PINA,_BV(PA3))
#define ACTION2_BUTTON GET(PINA,_BV(PA2))
#define ACTION3_BUTTON GET(PINA,_BV(PA1))


//OP-code for RAM
#define WREN 00000110b
#define READ 0000A011b
#define WRITE 0000A010b
