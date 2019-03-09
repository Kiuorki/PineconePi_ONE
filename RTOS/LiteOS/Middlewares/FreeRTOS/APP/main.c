#include "SWM320.h"

#include "FreeRTOS.h"
#include "task.h"

void TaskLED(void *arg);
void TaskCOM(void *arg);

int main(void)
{ 	
 	SystemInit();
	
	xTaskCreate(TaskLED, (const char *)"LED", 128, NULL, 2, NULL);
	xTaskCreate(TaskCOM, (const char *)"COM", 128, NULL, 3, NULL);
	
	vTaskStartScheduler();
}

/****************************************************************************************************************************************** 
* ��������:	TaskLED()
* ����˵��: LED����˸����
* ��    ��: void *arg
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void TaskLED(void *arg)
{
	GPIO_Init(GPIOA, PIN4, 1, 0, 0);	//GPIOA.4����Ϊ������ţ��������
	GPIO_Init(GPIOA, PIN5, 1, 0, 0);	//GPIOA.5����Ϊ������ţ��������
	GPIO_ClrBit(GPIOA, PIN4);
 	GPIO_SetBit(GPIOA, PIN5);
	
	while(1)
	{
		GPIO_InvBit(GPIOA, PIN4);
 		GPIO_InvBit(GPIOA, PIN5);
		
		vTaskDelay(500);
	}
}

void SerialInit(void);
/****************************************************************************************************************************************** 
* ��������:	TaskCOM()
* ����˵��: ���ڴ�ӡ����
* ��    ��: void *arg
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void TaskCOM(void *arg)
{
	SerialInit();
	
	while(1)
	{
		printf("Hi, World!\r\n");
		
		vTaskDelay(500);
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN2, FUNMUX0_UART0_RXD, 1);	//GPIOA.2����ΪUART0��������
	PORT_Init(PORTA, PIN3, FUNMUX1_UART0_TXD, 0);	//GPIOA.3����ΪUART0�������
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
 	while(UART_IsTXFIFOFull(UART0));
	
	UART_WriteByte(UART0, ch);
 	
	return ch;
}
