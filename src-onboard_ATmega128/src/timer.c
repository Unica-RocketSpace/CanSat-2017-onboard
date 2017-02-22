#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "wheel_control.h"

uint8_t cntr;

void timer0_init(void)
{
   //Инициируем регистр управления таймером
   TCCR0 =	(0 << FOC0 ) |	//Включает принудительное внешнее сравнение
		   (1 << WGM01) |	//Определяет режим таймера (00 - режим CTC)
		   (0 << WGM00) |
		   (0 << COM01) |	//подключает к таймеру пин OC0 (00 - OC0 отключен)
		   (0 << COM00) |
		   (1 << CS02 ) |	//Выставляет предделитель (111 - предделитель = 1024)
		   (1 << CS01 ) |
		   (1 << CS00 );

   TCNT0 = 0x00;		//Сбрасываем таймер на 0
   OCR0  = 255;			//Номер такта, при котором будет срабатывать прерывание
   //TIMSK |= (1 << OCIE0);		//Разрешает прерывания по сравнению
}

void timer1_init(void)
{
	TCCR1A =	(1 << COM1A1) |
				(0 << COM1A0) |
				(1 << COM1B1) |
				(0 << COM1B0) |	//подключает к таймеру пин OC0 (00 - пин отключен)
				(1 << COM1C1) |
				(0 << COM1C0) |
				(1 << WGM11) |	//WGM13:0 Определяет режим таймера (1110 - режим Быстрая ШИМ)
				(0 << WGM10);

	TCCR1B =	(1 << WGM13) |	//WGM13:0 Определяет режим таймера (1110 - режим Быстрый ШИМ)
				(1 << WGM12) |
				(0 << CS12) |	// устанавливаем предделитель (8)
				(1 << CS11) |
				(0 << CS10);
	OCR1A = 0;
	OCR1B = 0;
	OCR1C = 0;
	ICR1L = 0x20;
	ICR1H = 0x4E;
	//TIMSK |= (1 << TICIE1) | (1 << OCIE1A) | (1 << OCIE1B);
	//ETIMSK |= (1 << OCIE1C);
}


ISR(TIMER2_COMP_vect)
{

}


