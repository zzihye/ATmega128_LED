#define _DELAY_BACKWARD_COMPATIBLE_																//_delay_ms()의 괄호 안 변수를 입력
#define F_CPU 73728200UL																		//atmega 128의 클럭수 입력
#include <avr/io.h>
#include <avr/interrupt.h>																		//인터럽트 허용 또는 금지
# include <util/delay.h>																		//딜레이 헤더파일

unsigned char pattern_num[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f};				//0~9출력 배열

volatile int i=0, flag=527;

void display_num(void)
{
	for(int j=0;j<20;j++)																		//FND 작동
	{
		PORTD=~0x20;																			//PD4
		PORTC=~pattern_num[(i%100)/10];															//십의 자리
		_delay_ms(1);
		
		PORTD=~0x10;																			//PD5
		PORTC=~pattern_num[i%10];																//일의 자리
		_delay_ms(1);
	}
}

ISR(INT0_vect)																					//FND 상승
{
	flag=0;
}

ISR(INT1_vect)																					//FND 일시정지
{
	flag=1;
}

ISR(INT2_vect)																					//FND 3씩 감소
{
	flag=2;

ISR(INT3_vect)																					//홀수번 누르면 +2 짝수번 누르면 -2
{
	flag=3;
}

int main(void)
{
	
	DDRA=0xff;																					//A 출력포트 사용
	DDRC=0xff;																					//C 출력포트 사용
	DDRD=0XF0;																					//D 포트 0,1,2,3입력  4,5출력
	
	EICRA=0xaa;																					//인터럽트 제어 레지스터, 하강모서리
	EIMSK=0x0f;																					//외부 인터럽트 0,1,2,3사용
	sei();																						//인터럽트 허용
	
	PORTA=~0x00;																				//A 포트 초기화
	PORTC=~0x00;																				//C 포트 초기화
	
	while(1)
	
	{
		display_num();
		if(flag==0)
		{
			i++;
		}
		
		else if(flag==1)
		{
			if((~PIND&0x0f)==0x02)
			{
				i=i;
			}
			else
			{
				flag=0;
			}
		}
		else if(flag==2)
		{
			i=i-3;
		}
		 else if(flag==3)
		 {
			 
		 }
	}
}
