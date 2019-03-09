#include "SWM320.h"


int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SYS->CLKEN |= (1 << SYS_CLKEN_RTCBKP_Pos);		//���ѱ��뿪����ʱ��
	
	SYS->LRCCR &= ~(1 << SYS_LRCCR_OFF_Pos);		//����32K��Ƶ����
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0);				//����� ��LED
	
	GPIO_Init(GPIOA, PIN4, 0, 1, 0);				//���룬����ʹ�ܣ���KEY
	SYS->PAWKEN |= (1 << PIN4);						//����PA4���ŵ͵�ƽ����
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//����LED
		for(i = 0; i < 10000000; i++);
		GPIO_ClrBit(GPIOA, PIN5);					//Ϩ��LED
		
		switchCLK_32KHz();
		
		SYS->HRCCR = (1 << SYS_HRCCR_OFF_Pos);		//�ر�HRC�����͹���
		
		SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);	//����˯��ģʽ
	
		while((SYS->PAWKSR & (1 << PIN4)) == 0);	//�ȴ���������
		SYS->PAWKSR |= (1 << PIN4);					//�������״̬
		
		switchCLK_20MHz();
	}
}
