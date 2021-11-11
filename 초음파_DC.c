#define F_CPU 7372800
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

volatile unsigned int count=0, flag=0, distance=0;
volatile unsigned char duty;
unsigned char pattern_num[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f};				//0~9출력 배열

void Serialinit(unsigned long baud)			//분주비 계산 함수
{
	unsigned short ubrr;
	ubrr = (unsigned short) (F_CPU/(16*baud)-1);
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)(ubrr & 0xff);
	UCSR0B = 0x18; 							// RX TX 허용
	UCSR0C = 0x06; 							// 비동기 모드 , 정지 비트 : 1bit , 문자 크기: 8bit , 패리티 불능
}
void trans_x (unsigned char data)
{
		while((UCSR0A & 0x20) == 0x00);			// 버퍼(UDR0)에 데이터가 있어서 전송준비가 될 때 까지 대기
		UDR0 = data;							// 시리얼 포트를 통해 데이터 전송														
}

void fnd_num(unsigned char data)
{
	for(int i=0;i<20;i++)						//FND 작동
	{
		PORTD=~0x20;							//PD4
		PORTC=~pattern_num[(distance%100)/10];	//십의 자리
		_delay_ms(1);
		
		PORTD=~0x10;							//PD5
		PORTC=~pattern_num[distance%10];		//일의 자리
		_delay_ms(1);
	}
}
void Ultrasonic_DC_setting()
{
	// DC 모터
	TCCR1A = 0x09;	//Fast PWM(8bit)
	TCCR1B = 0x0b;	//분주비 64
	
	// 초음파 센서
	TCCR3B=0x0a;	// CTC모드, 분주비8
	ETIMSK=0x10;	// 타이머/카운터3 비교일치 인터럽트 허용
	EICRB=0x03;		// 상승 모서리에서 인터럽트 요구
	EIMSK=0x10;		// INT4 인터럽트 허용
	OCR3A = 0;		// 비교일치 레지스터 초기화
}

ISR(TIMER3_COMPA_vect)	//타이머카운터3 비교일치모드
{
	if(count<10)
	{
		PORTE=0x08; 	//PORTE3 초음파 트리거
		OCR3A=1;		//1us
	}
	else				// 10us가 되었을 때
	{
		PORTE=0x00;
		OCR3A=55295;	//60ms  (측정 주기를 60ms 이상으로 할 것 datasheets 참조)
		count=0;
	}
	count++;
}

ISR(INT4_vect)			//에코  PORTE4   (INT4)
{
	if(flag==0)
	{
		TCNT3=0; 		// TCNT값 초기화
		EICRB=0x02; 	// 하강모서리가 인터럽트 요구
		flag=1;
	}
	else
	{
		distance = TCNT3/58;	// 1uS/58 =1cm (데이터 시트 참조)
		EICRB=0x03;				// 상승 모서리가 인터럽트 요구
		flag=0;
	}
}

void main(void)
{
	DDRA=0Xff;
	DDRB=0x80;					// PORTB7 출력
	DDRC=0XFF;
	DDRD=0xC0;					// PORTD 6,7 모터출력
	DDRE=0x08;					// PORTE3 (트리거) 입력, PORTE4 (에코) 출력
	PORTA=~0xff;
	PORTB=0x80;					// 모터 드라이버 출력 허용

	
	Ultrasonic_DC_setting();
	sei();						// 전역 인터럽트 허용
	Serialinit(9600);			// 보레이트 9600 설정
	
	int y=0, LED;
	
	while(1)
	{
		OCR1C = duty;				// 듀티값을 OCR값으로 설정		
		while((~PIND&0X0F)==0x01)
		{
			if(5>distance) 
			{	
				LED=0x01;
				PORTA=~0X01;
			}
			else if(10>distance)
			{
				LED=0x03;
				PORTA=~0X03;
			}
			else if(15>distance)
			{
				LED=0x07;
				PORTA=~LED;
			}
			else if(20>distance)
			{
				LED=0x0F;
				PORTA=~LED;
			}
			else if(25>distance)
			{
				LED=0x1F;
				PORTA=~LED;
			}
			else if(30>distance)
			{
				LED=0x3F;
				PORTA=~LED;
			}
			else if(35>distance)
			{
				LED=0x7F;
				PORTA=~LED;
			}
			else if(40>distance)
			{
				LED=0xFF;
				PORTA=~LED;
			}
		}
		if((~PIND&0X0F)==0x02)
		{
			fnd_num(distance);
		}

		else if((~PIND&0X0F)==0X04)
		{
			trans_x((distance%100)/10 +'0');
			trans_x((distance%10) +'0');
			trans_x(0x0D);              // 새 줄로 문자표시 하는 아스키코드
		}
		else if((~PIND&0X0F)==0x08)
		{
			if(distance<10)
			{
				OCR1C=255;
				PORTD=0x80;
			}
			else if(10<=distance)
			{
				OCR1C=191;
				PORTD=0x80;
			}
			else if(15<=distance)
			{
				OCR1C=127;
				PORTD=0x80;
			}
			else if(distance==20)
			{
				OCR1C=0;
				PORTD=0x00;
			}
			else if(25<=distance)
			{
				OCR1C=127;
				PORTD=0x40;
			}
			else if(30<=distance)
			{
				OCR1C=191;
				PORTD=0x40;
			}
			else if(30<distance)
			{
				OCR1C=255;
				PORTD=0x40;
			}
		}
	}
}


