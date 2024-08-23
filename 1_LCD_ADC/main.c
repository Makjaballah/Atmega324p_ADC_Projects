/*
 * Project_Name    :   LCD_ADC
 * File_Name       :   main.c
 * Author          :   Mohamed Akram JABALLAH
 * Target          :   Atmega324p
*/


#include <avr/io.h>
#include <avr/delay.h>



#define LCD_Data       PORTC
#define RS             4
#define RW             5
#define EN             6


void LCD_Init (void);
void Write_LCD_Command (unsigned char cmd);
void Write_LCD_Data (unsigned char data);
void LCD_Write_String (char *str);
void LCD_Gotoxy (unsigned char x_pos, unsigned char y_pos);
void ADC_Init (void);
unsigned int ADC_Read (unsigned char ADCpin);


int main (void) {

	DDRA = 0x00;
	DDRB = 0xFF;
	DDRC = 0xFF;

	LCD_Init();
	_delay_ms (100);
	ADC_Init();
	_delay_ms (100);
	Write_LCD_Command (0x0C);
	_delay_ms (100);

	Write_LCD_Command (0x01);

	LCD_Gotoxy (0,0);
	LCD_Write_String ("D_output = ");
	_delay_ms (1);
	LCD_Gotoxy (1,0);
	LCD_Write_String ("Vin =  ");
	_delay_ms (1);

	uint16_t D_output;
	float V_in;
	unsigned char ch1[4] = {' '};
	unsigned char ch2[10] = {' '};

	while (1) {

		D_output =  ADC_Read (0);
		V_in = (D_output * 5.0)/ 1023;

		for (uint8_t i = 0; i < 4; i++) {
			ch1[i] = ' ';
		}

		LCD_Gotoxy (0,11);
		itoa (D_output, ch1, 10);

		for (uint8_t i = 0; i < 4; i++) {

			if ((ch1[i] < '0' || ch1[i] > '9'))
				Write_LCD_Data (' ');
			else
				Write_LCD_Data (ch1[i]);
		}

		for (uint8_t i = 0; i < 10; i++) {
			ch2[i] = ' ';
		}

		LCD_Gotoxy (1,7);
		dtostrf (V_in, 4,4, ch2);

		for (uint8_t i = 0; i < 10; i++) {
			if ((ch2[i] != '.') && (ch2[i] < '0' || ch2[i] > '9'))
				Write_LCD_Data (' ');
			else
				Write_LCD_Data (ch2[i]);
		}

		_delay_ms (250);
	}

	return 0;
}


void LCD_Init (void) {

	Write_LCD_Command (0x38);
	_delay_ms (1);
	Write_LCD_Command (0x01);
	_delay_ms (1);
	Write_LCD_Command (0x02);
	_delay_ms (1);
	Write_LCD_Command (0x06);
	_delay_ms (1);
	Write_LCD_Command (0x80);
	_delay_ms (1);
}


void Write_LCD_Command (unsigned char cmd) {

	LCD_Data =  cmd;
	PORTB &= ~ (1 << RS);
	PORTB &= ~ (1 << RW);
	PORTB |=   (1 << EN);
	_delay_ms (2);
	PORTB &= ~ (1 << EN);
}

void Write_LCD_Data (unsigned char data) {

	LCD_Data =  data;
	PORTB |=   (1 << RS);
	PORTB &= ~ (1 << RW);
	PORTB |=   (1 << EN);
	_delay_ms (2);
	PORTB &= ~ (1 << EN);
}


void LCD_Write_String (char *str) {

	unsigned char i = 0;
	while (str[i] != 0) {
		Write_LCD_Data (str[i]);
		i++;
	}
}


void LCD_Gotoxy (unsigned char x_pos, unsigned char y_pos) {

	uint8_t The_Address = 0;
	if (x_pos == 0)
		The_Address = 0x80;
	else if (x_pos == 1)
		The_Address = 0xC0;
	if (y_pos < 16)
		The_Address += y_pos;
	Write_LCD_Command (The_Address);
}


unsigned int ADC_Read (unsigned char ADCpin) {

	ADCpin &= 0x07;
	ADMUX   = (ADMUX & 0xF8) | ADCpin ;
	ADCSRA |= (1 << ADSC);
	while((ADCSRA & (1  << ADIF)) == 0) ;
	ADCSRA |= (1  << ADIF);
	return ADC;
}

void ADC_Init (void) {

	ADMUX  |= (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}












