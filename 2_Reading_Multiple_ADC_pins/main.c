/*
 * Project_Name    :   Reading_Multiple_ADC_pins
 * File_Name       :   main.c
 * Author          :   Mohamed Akram JABALLAH
 * Target          :   Atmega324p
*/


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define EN  PORTB1
#define RS  PORTB2


void LCD_Init (void);
void Write_LCD_Command (unsigned char cmd);
void Write_LCD_Data (unsigned char data);
void LCD_Write_String (char *str);
void LCD_Gotoxy (unsigned char x, unsigned char y);
void ADC_Init (void);
unsigned int ADC_Read (unsigned char ADCpin);



int main (void) {

	unsigned int a1, a2, a3,a4;
	char Temp [16];

	DDRB  = 0xFF;
	DDRA  = 0x00;
	PORTA = 0xFF;

	ADC_Init();
	LCD_Init();

	for (;;) {
		
		a1 = ADC_Read (0);
		a2 = ADC_Read (2);
		LCD_Gotoxy (0,0);
		sprintf (Temp,"V1=%d  V2=%d",a1,a2);
		LCD_Write_String (Temp);
		
		
		a3 = ADC_Read (4);
		a4 = ADC_Read (6);
		LCD_Gotoxy (0,1);
		sprintf (Temp,"V3=%d  V4=%d",a3,a4);
		LCD_Write_String (Temp);
		_delay_ms (1000);
	}

	return 0;
}

unsigned int ADC_Read (unsigned char ADCpin) {

	ADCpin &= 0x07;
	ADMUX   = (ADMUX & 0xF8) | ADCpin;
	ADCSRA |= (1 << ADSC);
	while((ADCSRA & (1  << ADIF)) == 0);
	ADCSRA |= (1  << ADIF);
	return ADC;
}


void ADC_Init (void) {

	ADMUX  |= (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}


void LCD_Init (void) {

	Write_LCD_Command (0x02);
	Write_LCD_Command (0x28);
	Write_LCD_Command (0x0C);
	Write_LCD_Command (0x06);
	Write_LCD_Command (0x01);
}

void Write_LCD_Command (unsigned char cmd) {

	PORTB   = (PORTB & 0x0F) | (cmd & 0xF0);
	PORTB  &= ~(1 << RS);
	PORTB  |=  (1 << EN);
	_delay_us (2);
	PORTB  &= ~(1 << EN);

	PORTB   = (PORTB & 0x0F) | (cmd << 4);
	PORTB  |=  (1 << EN);
	_delay_us (2);
	PORTB  &= ~(1 << EN);
	_delay_us (2);
}


void Write_LCD_Data (unsigned char data) {

	PORTB   = (PORTB & 0x0F) | (data & 0xF0);
	PORTB  |=  (1 << RS);
	PORTB  |=  (1 << EN);
	_delay_us (2);
	PORTB  &= ~(1 << EN);

	PORTB   = (PORTB & 0x0F) | (data << 4);
	PORTB  |=  (1 << EN);
	_delay_us (2);
	PORTB  &= ~(1 << EN);
	_delay_us (2);
}


void LCD_Write_String (char *str) {

	int i;
	for (i = 0; str[i] != 0; i++) {
		Write_LCD_Data (str[i]);
	}
}


void LCD_Gotoxy (unsigned char x, unsigned char y) {

	if (y == 0)
		Write_LCD_Command (0x80 + x);
	if (y == 1)
		Write_LCD_Command (0xC0 + x);
}






