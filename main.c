/*
 * Prelab_6.c
 *
 * Created: 21/04/2024 23:11:40
 * Author : pablo
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setup (void);
void initUART9600 (void);
void writeUART (char Caracter);
void writeTextUART (char * Texto);
void initADC(void);
void UARTsc();	
volatile uint8_t bufferTX;

void setup (void){
	DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3) | (1<<DDB4) | (1<<DDB5);
	initADC();
	initUART9600();
	sei();
}

int main(void)
{
	setup();
	UARTsc();
	//writeUART('H');
	//writeUART('o');
	//writeUART('l');
	//writeUART('a');
	//writeUART(' ');
	//writeUART('M');
	//writeUART('u');
	//writeUART('n');
	//writeUART('d');
	//writeUART('o');
	//writeUART('\n');
    while (1) 
    {
    }
}

void writeTextUART (char * Texto){
	for (uint8_t i = 0; Texto[i] != '\0'; i++){
		writeUART(Texto[i]);
	}
}

void initUART9600 (void) {
	//Paso 1:
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	//Paso 2:
	UCSR0A = 0;
	
	//Configurar USCR0B > 
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	//Paso 4: Configurar UCSR0C > asíncrono, paridad: none, 1 bit de stop, data bits 8/bits
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	
	//Paso 5: configurar velocidad de Baudrate: 9600
	UBRR0 = 103;
}

void writeUART (char Caracter){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
	
}

void UARTsc(){
	writeTextUART("Caracter que quiere mostrar en LEDS:\r\n");
}

ISR(USART_RX_vect){
	
	bufferTX = UDR0;
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = bufferTX;
	PORTB = bufferTX;
	writeUART(10);
	writeUART(13);
	UARTsc();
	
	
}

void initADC() {
	ADMUX = 0; // INICIO EN 0
	// REFERENCIA AVCC = 5V
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << REFS1);
	// SELECCIONO EL ADC[6] = 0110
	ADMUX |= (1 << MUX2) | (1 << MUX0);
	
	// JUSTIFICACION A LA IZQUIERDA
	ADMUX |= (1 << ADLAR);
	
	ADCSRA = 0;
	
	
	// HABILITAR LA INTERRUPCION DEL ADC
	ADCSRA |= (1 << ADIE);
	
	// HABILITAMOS EL PRESCALER 128  FADC = 125 KHz
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// HABILITAMOS EL ADC
	ADCSRA |= (1 << ADEN);

}