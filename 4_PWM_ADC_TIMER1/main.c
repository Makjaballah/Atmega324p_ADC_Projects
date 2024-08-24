/*
 * Project_Name    :   PWM_ADC_TIMER1
 * File_Name       :   main.c
 * Author          :   Mohamed Akram JABALLAH
 * Target          :   Atmega324p
*/


#include <avr/io.h>
#include <util/delay.h>



void PWM_Init (uint16_t Freq);
void ADC_Init (void);
uint16_t ADC_Read (uint8_t channel);


int main (void) {

	DDRA  = 0x00;
	PORTA = 0xFF;
	DDRD  = 0xFF;

	PWM_Init (1000);
	ADC_Init ();
	OCR1B = 0;

	for (;;) {

		OCR1B = ADC_Read (0);
	}

	return 0;
}


void PWM_Init (uint16_t Freq) {

	TCCR1A |=  (1 << COM1B1) | (1 << WGM11) | (1 << WGM10);
	TCCR1B |=   (1 << CS10)  | (1 << WGM13) | (1 << WGM12);
	OCR1A   = (F_CPU/(1*Freq))-1;
}


void ADC_Init (void) {

	ADMUX  |= (1 << REFS0);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}


uint16_t ADC_Read (uint8_t channel) {

	channel &= 0x07;
	ADMUX   &= 0xF8;
	ADMUX   |= channel;
	ADCSRA  |=  (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADCL | (ADCH << 8);
}
