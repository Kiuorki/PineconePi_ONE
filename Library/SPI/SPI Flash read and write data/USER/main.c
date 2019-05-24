/* ****************************** ���ߣ�������� ***************************************
 * �ļ����� : main.c
 * �ļ����� : ��SPI Flashд���ַ�����֮��������Ա���д��Ͷ������Ƿ�һ�����̶�ʵ��SPI�Ķ�д����
 * Ӳ��ƽ̨ : ������ſ����壨��������SWM32LQ100FA��  
 * ������� : Keil uVision5.23 + SWM320�⺯��
 * 
 * �汾���� : V1.1 	   					
 * �޸����� : 2018-5-12
 * ��Ȩ���� : ��о΢�غ����߹�ͬ����
 * ����ѧϰʹ�ã�δ��������ɣ���������������ҵ��;������ؾ���
************************************************************************************** */
#include "SWM320.h"
#include "bsp_systick.h"
#include "bsp_uart.h"
#include "bsp_SPIFlash.h"

/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "PineconePi ONE!!!!";
uint8_t Rx_Buffer[BufferSize];

int main(void)
{	
	uint32_t DeviceID = 0;
	uint32_t FlashID = 0;
	
	SystemInit();
	SerialInit(115200);							// ��ʼ������
	SysTick_Init();								// ��ʼ���δ�ʱ��
	SPI_Flash_Init();
	
	printf("SPI Flash Test Project RunOK ...... \r\n");
	
	DeviceID = SPI_FLASH_ReadDeviceID();		// ��ȡ Flash Device ID
	SWM_Delay(200);
	
	FlashID = SPI_FLASH_ReadID();				// ��ȡ SPI Flash ID
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID) 
	{	
		printf("\r\nSPI FLASH is W25Q128 !\r\n");
		
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* �����ͻ�����������д��flash�� */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		printf("\r\nWrite Date:\r\n%s", Tx_Buffer);
		
		/* ���ո�д������ݶ������ŵ����ջ������� */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\nRead Date:\r\n%s", Rx_Buffer);

	}
	
	while(1)
	{
	}
}

