
#include "SWM320.h"
#include "bsp_systick.h"
#include "bsp_uart.h"
#include "bsp_SPIFlash.h"

int main(void)
{	
	uint32_t DeviceID = 0;
	uint32_t FlashID = 0;
	
	SystemInit();
	SerialInit(115200);							// ��ʼ������
	SysTick_Init();								// ��ʼ���δ�ʱ��
	SPI_Flash_Init();
	
	printf("SPI Flash�����ϵ�OK ...... \r\n");
	
	DeviceID = SPI_FLASH_ReadDeviceID();
	SWM_Delay(200);
	/* ��ȡ SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();	
	printf("\r\n FlashID is 0x%X,\
	Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	while(1)
	{
	}
}

