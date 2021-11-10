#define F_CPU 7372800
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	int i,led1,led2,n,t,led3,delay=20;
	DDRD = 0x00;                                					  //입력포트 설정
	DDRA = 0xFF;                                					 //출력포트 설정
	PORTA = 0xFF;                                   					//꺼짐

	while(1)
	{								           //누르면 LED0>7이동
		if((~PIND&0x0F)==0x01)						//1번 스위치 누른다면
		{
			
			led1 = 0x01;						//초기값 대입
			for(i=0;i<9;i++)
			{
				PORTA = ~led1;					// 비트 반전하여 A 포트에 출력(0xFE)
				led1 = (led1<<1);					// led1 변수에서 1비트만큼 왼쪽으로 이동
				_delay_ms(200);					// 시간 지연 함수
			}
			
		}
		
		else if
  		while(((~PIND&0x0f)==0x02))						//2번 스위치에서 손을 땔때까지 대기
		{
			led2 = 0x80;
			_delay_ms(1000);
			PORTA=0xFF;						//1번~8번 스위치 점등
			_delay_ms(1000);
			PORTA=0xFD;						//채터링 방지(채터링:스위치나 릴레이 등의 접점이 개폐될 때 기계에서 발생하는 진동)하기위해서 딜레이를 준다.
			_delay_ms(1000);
			PORTA=0xFB;
			_delay_ms(1000);
			PORTA=0xF7;
			_delay_ms(1000);
			PORTA=0xEF;
			_delay_ms(1000);
			PORTA=0xDF;
			_delay_ms(1000);
			PORTA=0xBF;
			_delay_ms(1000);
			PORTA=0x7F;
		}
	
		else if((~PIND&0x0f)==0x04)			       //3번 스위치 누른다면
		{
			
			int i;
				i++;
				if(i%2==1)			//홀수 일때
				{
					PORTA=0xAA;		// 홀수번 출력
					_delay_ms(1000);		 // 위 상태를 유지
				}
				
				else
				{
					PORTA=0x55;		// 짝수번 출력
					_delay_ms(1000);		 // 위 상태를 유지
				}
		}
		
		else if((~PIND&0x0F)==0x08)		              //4번 스위치 누른다면
		{
			led3=~0x80;
			t=4000;
			for(i=0;i<8;i++)
			{
				PORTA=~led3;
				led3=(led3>>1);
				_delay_ms(100);
				t=t/2;
			}
			i=0;
			_delay_ms(2000);
		}
	}
}
