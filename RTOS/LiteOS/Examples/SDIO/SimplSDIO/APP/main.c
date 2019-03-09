#include "SWM320.h"

#define BLK_ADDR	0x80000	//0x80000*512 = 256M

uint32_t Buffer[512/4] = {
	0x00000000, 0x01010101, 0x02020202, 0x03030303, 0x04040404, 0x05050505, 0x06060606, 0x07070707, 0x08080808, 0x09090909, 0x0A0A0A0A, 0x0B0B0B0B, 0x0C0C0C0C, 0x0D0D0D0D, 0x0E0E0E0E, 0x0F0F0F0F, 0x10101010, 0x11111111, 0x12121212, 0x13131313,
	0x14141414, 0x15151515, 0x16161616, 0x17171717, 0x18181818, 0x19191919, 0x1A1A1A1A, 0x1B1B1B1B, 0x1C1C1C1C, 0x1D1D1D1D, 0x1E1E1E1E, 0x1F1F1F1F, 0x20202020, 0x21212121, 0x22222222, 0x23232323, 0x24242424, 0x25252525, 0x26262626, 0x27272727,
	0x28282828, 0x29292929, 0x2A2A2A2A, 0x2B2B2B2B, 0x2C2C2C2C, 0x2D2D2D2D, 0x2E2E2E2E, 0x2F2F2F2F, 0x30303030, 0x31313131, 0x32323232, 0x33333333, 0x34343434, 0x35353535, 0x36363636, 0x37373737, 0x38383838, 0x39393939, 0x3A3A3A3A, 0x3B3B3B3B,
	0x3C3C3C3C, 0x3D3D3D3D, 0x3E3E3E3E, 0x3F3F3F3F, 0x40404040, 0x41414141, 0x42424242, 0x43434343, 0x44444444, 0x45454545, 0x46464646, 0x47474747, 0x48484848, 0x49494949, 0x4A4A4A4A, 0x4B4B4B4B, 0x4C4C4C4C, 0x4D4D4D4D, 0x4E4E4E4E, 0x4F4F4F4F,
	0x50505050, 0x51515151, 0x52525252, 0x53535353, 0x54545454, 0x55555555, 0x56565656, 0x57575757, 0x58585858, 0x59595959, 0x5A5A5A5A, 0x5B5B5B5B, 0x5C5C5C5C, 0x5D5D5D5D, 0x5E5E5E5E, 0x5F5F5F5F, 0x60606060, 0x61616161, 0x62626262, 0x63636363,
	0x64646464, 0x65656565, 0x66666666, 0x67676767, 0x68686868, 0x69696969, 0x6A6A6A6A, 0x6B6B6B6B, 0x6C6C6C6C, 0x6D6D6D6D, 0x6E6E6E6E, 0x6F6F6F6F, 0x70707070, 0x71717171, 0x72727272, 0x73737373, 0x74747474, 0x75757575, 0x76767676, 0x77777777,
	0x78787878, 0x79797979, 0x7A7A7A7A, 0x7B7B7B7B, 0x7C7C7C7C, 0x7D7D7D7D, 0x7E7E7E7E, 0x7F7F7F7F
};

void SerialInit(void);

int main(void)
{
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN1, PORTB_PIN1_SD_CLK, 0);
	PORT_Init(PORTB, PIN2, PORTB_PIN2_SD_CMD, 1);
	PORT_Init(PORTB, PIN3, PORTB_PIN3_SD_D0,  1);
	PORT_Init(PORTB, PIN4, PORTB_PIN4_SD_D1,  1);
	PORT_Init(PORTB, PIN5, PORTB_PIN5_SD_D2,  1);
	PORT_Init(PORTB, PIN6, PORTB_PIN6_SD_D3,  1);
	
    SDIO_Init();
	
   	SDIO_BlockWrite(BLK_ADDR, Buffer);
	
	for(i = 0; i < 512/4; i++) Buffer[i] = 0;
	
	SDIO_BlockRead(BLK_ADDR, Buffer);
	
	printf("Read from SD Card:\r\n");
	for(i = 0; i < 512/4; i++) printf("0x%08X,", Buffer[i]);
	
	while(1==1)
	{
		
	}
}

void SerialInit(void)
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
