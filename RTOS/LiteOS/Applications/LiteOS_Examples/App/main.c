#include "SWM320.h"

#include "los_base.h"

UINT32 Example01_Entry(VOID);
UINT32 Example02_Entry(VOID);
UINT32 Example03_Entry(VOID);
UINT32 Example04_Entry(VOID);
UINT32 Example05_Entry(VOID);
UINT32 Example06_Entry(VOID);
UINT32 Example07_Entry(VOID);
UINT32 Example08_Entry(VOID);
UINT32 Example09_Entry(VOID);
UINT32 Example10_Entry(VOID);
UINT32 Example11_Entry(VOID);
UINT32 Example12_Entry(VOID);
UINT32 Example13_Entry(VOID);
UINT32 Example14_Entry(VOID);
UINT32 Example15_Entry(VOID);

static void SerialInit(void);

int main(void)
{
 	SystemInit();
	
	SerialInit();
	
	/* USER CODE BEGIN 2 */
  if (LOS_OK != LOS_KernelInit()) {
      return LOS_NOK;
  }
  
  Example15_Entry();

  LOS_Start();
	
 	while(1) {
 	}
}

static void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTA, PIN2, FUNMUX0_UART0_RXD, 1);	//GPIOA.2����ΪUART0��������
	PORT_Init(PORTA, PIN3, FUNMUX1_UART0_TXD, 0);	//GPIOA.3����ΪUART0�������
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
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
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
