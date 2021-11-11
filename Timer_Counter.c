#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

void TimerCounter_Setup()
{	
	TCCR1A = (1<<COM1A1)| (0<<COM1A0) |(1<<WGM11);		//Fast PWM비교출력
	TCCR1B = (1<<WGM13) | (1<<WGM12)  | (1<<CS11) ;		//Fast PWM비트	
	ICR1 = 18432;										//프리스케일러 8
	OCR1A = 1382;										//서보모터 중간으로 세팅
	TCCR0 = (1<<COM01)|(1<<CS01)|(1<<WGM00)|(1<<WGM01);		//Fast PWM Mode, 비교출력, 프리스케일링 8
	OCR0 = 150;												//비교일치
}

int main (void)
{
	DDRB = 0XFF;
	TimerCounter_Setup();
	int i=1382,j=150; 
	while (1)
	{
		if(((~PIND&0x0F)==0X01))						//스위치1
		{
			OCR1A=i--;
			if(i<=645) i = 645;								//왼쪽
			_delay_ms(1);									
			
		}
		else if(((~PIND&0x0F)==0x02))						//스위치2
		{
			OCR1A=i++;
			if(i>=2119)	i=2119;								//오른쪽
			_delay_ms(1);								
		
		}
		else if((~PIND&0x0F)==0X04)						//스위치3
		{
			OCR0=j--;
			if(j<=30)j=30;								//led밝기감소
			_delay_ms(1);
		}
		else if((~PIND&0x0F)==0X08) 					//스위치4
		{
			OCR0=j++;
			if(j>=250)j=250;							//led밝기증가
			_delay_ms(1);
			
		}		
		else continue;
		}
}