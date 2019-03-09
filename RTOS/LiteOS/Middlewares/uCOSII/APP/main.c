#include "SWM320.h"

#include "ucos_ii.h"

static  OS_STK TaskLEDStk[TASK_LED_STK_SIZE];
static  OS_STK TaskCOMStk[TASK_COM_STK_SIZE];

void TaskLED(void *arg);
void TaskCOM(void *arg);

int main(void)
{ 	
 	SystemInit();
	
	NVIC_SetPriority(SysTick_IRQn, OS_CPU_CFG_SYSTICK_PRIO);
	SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);
	
	OSInit();
	
	OSTaskCreate((void (*)(void *))TaskLED, (void *)0, &TaskLEDStk[TASK_LED_STK_SIZE - 1], TASK_LED_PRIO);
	OSTaskCreate((void (*)(void *))TaskCOM, (void *)0, &TaskCOMStk[TASK_COM_STK_SIZE - 1], TASK_COM_PRIO);
	
	OSStart();
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
		
		OSTimeDly(OS_TICKS_PER_SEC);
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
		
		OSTimeDly(OS_TICKS_PER_SEC);
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
