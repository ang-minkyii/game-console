/*************************************************************************
Title:    game_console Template
Initial Author:   David Jahshan
Extended by : (PUT YOUR NAME HERE) (PUT YOUR STUDENT NUMBER HERE)
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Main and functions for Game Console basic sample code

*************************************************************************/


#include "game_console.h" 

char checkPage(char);
char checkColumn(char);
void clearScreen();
void SPI_transmit(char);
void SPI_Init(void);
void RAM_Init(void);
void LCD_command_tx(byte);
void LCD_data_tx(byte);
void LCD_Init(void);
void select_page (byte);
void select_column (byte); 
void movement(unsigned char, unsigned char, char);
char checkRow(char);
void int0_BUTTON_Init(void);
void ADC_Init(void);
void backlightAdjust(unsigned char);
void timer1_PWM_Init(void);

//allocate enough memory for array
char frame_buffer[MAX_COL+1][MAX_PAGE+1] = {{0}};
const byte LIGHT[]={0xFF,0xC8,0x96,0x64,0x32};
char PA = 0;
char CA = 0;
char row = 0;
char pixel = 0;
byte brightness = INIT_BRIGHT;

int main(void)
{
	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.
	LCDBACKLIGHT_DIR(OUT);
	LCDBACKLIGHT(ON);
	LCD_RST_DIR(OUT);
	LCD_CD_DIR(OUT);
	LCD_CS_DIR(OUT);

	UP_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	DOWN_BUTTON_DIR(IN);
	LEFT_BUTTON_DIR(IN);
	RIGHT_BUTTON_DIR(IN);
	ACTION1_BUTTON_DIR(IN);
	ACTION2_BUTTON_DIR(IN);
	ACTION3_BUTTON_DIR(IN);

	UP_BUTTON_PULL(OFF);
	DOWN_BUTTON_PULL(OFF);
	LEFT_BUTTON_PULL(OFF);
	RIGHT_BUTTON_PULL(OFF);
	ACTION1_BUTTON_PULL(OFF);
	ACTION2_BUTTON_PULL(OFF);
	ACTION3_BUTTON_PULL(OFF);

	SPI_Init();
	LCD_Init();
	RAM_Init();
	ADC_Init();
	timer1_PWM_Init();
	int0_BUTTON_Init();
	backlightAdjust(brightness);
	
	
	while (TRUE)//Master loop always true so always loop
	{
		CA = checkColumn(CA);
		movement(PA,CA,pixel);

		//Turn on the LED if UP_BUTTON is pressed
 		if (~UP_BUTTON) 
		{
			row--;
			row = checkRow(row);
			PA = row/8;
			pixel = row%8;
		}
		if(~DOWN_BUTTON)
		{
			row++;
			row = checkRow(row);
			PA = row/8;
			pixel = row%8;
		}
		if(~LEFT_BUTTON)
		{
			CA--;
		}
		if(~RIGHT_BUTTON)
		{
			CA++;
		}

		if(battThres<ADC_Read())
		{
			BAT_LOW_LED(ON);
		}
	}
}

char checkRow(char row)
{
	if(row > MAX_ROW)
	{
		row = MIN_ROW;
	}
	if(row < MIN_ROW)
	{
		row = MAX_ROW;
	}
	return (row);
}

void SPI_transmit(char cData)
{
	
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)))
	;
}

void LCD_command_tx(byte cData)
{
	LCD_CS_PULL(LOW);
	LCD_CD_PULL(LOW);
	SPI_transmit(cData);
	LCD_CS_PULL(HIGH);
	LCD_CD_PULL(HIGH);
}

void LCD_data_tx(byte cData)
{
	LCD_CS_PULL(LOW);
	LCD_CD_PULL(HIGH);
	SPI_transmit(cData);
	LCD_CS_PULL(HIGH);
}

void SPI_Init()
{
	MOSI_DIR(OUT);
	MISO_DIR(IN);
	SCK_DIR(OUT);
	SS_DIR(OUT);

	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); //SPI mode 3
}


void select_page (byte page)
{
	byte page_cmd_address; 
	page_cmd_address =(CMD_PAGE | page); 
	LCD_command_tx(page_cmd_address ); 
}

void select_column (byte column) 
{
	byte page_cmd_address_MSB;
	byte page_cmd_address_LSB;
	page_cmd_address_LSB =(CMD_COL_LSB | (column&0x0F)); 
	page_cmd_address_MSB =(CMD_COL_MSB | (column >> 4)); 
	LCD_command_tx(page_cmd_address_LSB); 
	LCD_command_tx(page_cmd_address_MSB);
}

void LCD_Init()
{

	LCD_RST_STAT(HIGH);

	//low command data to allow initialisation
	LCD_command_tx(startLine);
	LCD_command_tx(SEGdirect);
	LCD_command_tx(COMdirect);
	LCD_command_tx(pixelON);
	LCD_command_tx(inverseDisp);
	LCD_command_tx(biasRatio);
	LCD_command_tx(powerCont);
	LCD_command_tx(resisRatio);
	LCD_command_tx(setContrast);
	LCD_command_tx(setContrast2);
	LCD_command_tx(advProg);
	LCD_command_tx(advProg2);
	LCD_command_tx(dispON);
	clearScreen();

}

void int0_BUTTON_Init(void)
{
	INTERRUPT_DIR(IN);
	MCUCR=0x03;
	GICR=0x40;
	SREG=(1<<7);
}

ISR(INT0_vect)
{
	if(~ACTION1_BUTTON)
	{
		if(brightness == 5)
		{
			brightness = 0;	
		}
		backlightAdjust(brightness);
		brightness ++;
	}
}

void ADC_Init(void)
{
	ADMUX=0xE4;// internal 2.56V reference,Left adjust,ADC4
	ADCSRA=0xC4;// enable adc, clk/64
	SFIOR=0x00;
}

int ADC_Read(void)
{
	//Start single conversion
	ADCSRA |= (1<<ADSC);
	//Runs until ADSC becomes 0 again
	while(ADCSRA & (_BV(ADSC)));
	//Return ADCH & ADCL
	return(ADC);
}

void RAM_Init()
{
	RAM_CS_DIR(OUT);
	RAM_HOLD_DIR(OUT);
	RAM_WP_DIR(OUT);
	RAM_CS_STAT(HIGH);
	RAM_HOLD_STAT(HIGH);
	RAM_WP_STAT(HIGH);
}

void backlightAdjust(unsigned char brightness)
{
	OCR1BL= LIGHT[brightness];
}

void timer1_PWM_Init(void)
{
	TCNT1H=0x00;
	TCNT1L=0x00;//Setup counter
	OCR1BH=0x00;
	OCR1BL=0xFF;//Set comparison value
	TCCR1A=0x31;
	TCCR1B=0x0C;//8-bit fast PWM, clk/256
}

void movement(unsigned char page, unsigned char column, char pixel)
{	
		select_page(page);
		select_column(column);
		pixel = (_BV(pixel)|(frame_buffer[column][page]));
		frame_buffer[column][page] = pixel;
		LCD_data_tx(pixel);
		_delay_ms(50);
}

char checkColumn(char column)
{
	if(column > MAX_COL)
	{
		column = MIN_COL;
	}
	if (column < MIN_COL)
	{
		column = MAX_COL;
	}
	return (column);
}

void clearScreen()
{	
	byte PA;
	byte CA;

	for(PA=0;PA<(MAX_PAGE+1);PA++)
	{
		select_page(PA);
		for(CA=0;CA<(MAX_COL+1);CA++)
		{
			select_column(CA);
			LCD_data_tx(0x00);
		}
	}
}

void ramWrite(char wdata)
{
	RAM_WP_STAT(HIGH);
	RAM_HOLD_STAT(LOW);
	RAM_CS_STAT(LOW);
	SPI_transmit(wdata);
	RAM_CS_STAT(HIGH);
}
