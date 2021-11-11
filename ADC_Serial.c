#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define CPU_CLOCK_HZ 7372800UL

void SerialInit(unsigned long baud)
{
	unsigned short ubrr;
	ubrr=(unsigned short)(CPU_CLOCK_HZ/(16*baud)-1);
	UBRR0H=(unsigned char) (ubrr>>8);				//보레이트 레지스터 상위 4비트
	UBRR0L=(unsigned char) (ubrr&0xff);				//보레이트 레지스터 하위 8비트
	
	UCSR0B = 0x98;									//RX, TX 허용
	UCSR0C = 0x06;									//비동기모드, 정지비트 1-비트, 문자크기 8-비트, 패리티비트 사용안함
}
void tx_data(unsigned char data)
{
	while((UCSR0A & 0x20) == 0x00);					//
	UDR0 = data;									//송신과 수신값을 저장하는 레지스터 UDR
}
unsigned char fnd_num[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f};
void bright_num(unsigned int data)
{
	for(int i=0;i<100;i++)						//FND 작동
	{
		PORTD=~0x20;							//PD4
		PORTC=~fnd_num[(data%100)/10];					//십의 자리
		_delay_ms(1);
		
		PORTD=~0x10;							//PD5
		PORTC=~fnd_num[data%10];							//일의 자리
		_delay_ms(1);
	}
}
int main (void)
{
	
	unsigned int data=0;
	
	DDRA=0xff;										//A 출력포트 사용
	DDRC=0xff;										//C 출력포트 사용
	DDRD=0X00;										//D 입력포트 사용
	
	ADMUX = 0xc0;									//내부기준전압사용, 오른쪽 정렬, 싱글엔드 입력
	ADCSRA = 0xe7;									//ADC가능, ADC변환시작, ADC 프리러닝 사용,(인터럽트AND인터럽트 플래그 사용안함,인트럽트 허용안함), 프리스케일러 128
	SerialInit(9600);								//속도 9600
	
	PORTA=~0x00;									//A 포트 초기화
	PORTC=~0x00;									//C 포트 초기화
	
	
	while(1)
	{
		data=ADC;									//CDS값을 data에 옮김
		bright_num(data);
		tx_data(data%100/10+'0');					//십의자리
		tx_data(data%10+'0');						//일의자리
		tx_data(0x0D);								//새 줄로 문자표시 하는 아스키코드 0x0D
		_delay_ms(1);
			}
}