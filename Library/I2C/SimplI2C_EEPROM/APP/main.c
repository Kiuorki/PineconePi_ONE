#include "SWM320.h"

#define SLV_ADDR  0x6C

char mst_txbuff[6] = {0x00, 0x00, 0x37, 0x55, 0xAA, 0x78};
char mst_rxbuff[4] = {0};

void SerialInit(void);
void I2C_Mst_Init(void);

int main(void)
{
	uint32_t i;
	SystemInit();
	
	SerialInit();
	
	I2C_Mst_Init();
	
	while(1==1)
	{
		/* I2C Write EEPROM*/		
		I2C0->MSTDAT = (SLV_ADDR << 1) | 0;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
					   (1 << I2C_MSTCMD_WR_Pos);			//������ʼλ�ʹӻ���ַ
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 6; i++)
		{
			I2C0->MSTDAT = mst_txbuff[i];
			I2C0->MSTCMD = (1 << I2C_MSTCMD_WR_Pos);
			while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//�ȴ��������
			if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
			{
				printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
		}
		
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		
		printf("I2C Write %X %X %X %X to EEPROM @ %X %X \r\n", mst_txbuff[2], mst_txbuff[3], mst_txbuff[4], mst_txbuff[5], mst_txbuff[0], mst_txbuff[1]);
		
		
		/* I2C Read EEPROM */
		I2C0->MSTDAT = (SLV_ADDR << 1) | 0;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
					   (1 << I2C_MSTCMD_WR_Pos);			//������ʼλ�ʹӻ���ַ
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 2; i++)
		{
			I2C0->MSTDAT = mst_txbuff[i];
			I2C0->MSTCMD = (1 << I2C_MSTCMD_WR_Pos);
			while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//�ȴ��������
			if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
			{
				printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
		}
		
		I2C0->MSTDAT = (SLV_ADDR << 1) | 1;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |
					   (1 << I2C_MSTCMD_WR_Pos);			//����Restart
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 3; i++)
		{
			I2C0->MSTCMD = (1 << I2C_MSTCMD_RD_Pos) |
						   (0 << I2C_MSTCMD_ACK_Pos);
			while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//�ȴ��������
			mst_rxbuff[i] = I2C0->MSTDAT;
		}
		
		I2C0->MSTCMD = (1 << I2C_MSTCMD_RD_Pos)  |
					   (1 << I2C_MSTCMD_ACK_Pos) |
					   (1 << I2C_MSTCMD_STO_Pos);		//��ȡ���ݡ�����NACK��Ӧ����ɺ���STOP
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//�ȴ��������
		mst_rxbuff[i] = I2C0->MSTDAT;
		
		printf("I2C Read %X %X %X %X from EEPROM @ %X %X \r\n", mst_rxbuff[0], mst_rxbuff[1], mst_rxbuff[2], mst_rxbuff[3], mst_txbuff[0], mst_txbuff[1]);
		
		if((mst_txbuff[2] == mst_rxbuff[0]) && (mst_txbuff[3] == mst_rxbuff[1]) && (mst_txbuff[4] == mst_rxbuff[2]) && (mst_txbuff[5] == mst_rxbuff[3]))
			printf("Success\r\n");
		else
			printf("Fail\r\n");

nextloop:
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		for(i = 0; i < 2000000; i++) ;
	}
}

void I2C_Mst_Init(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTA, PIN4, FUNMUX0_I2C0_SCL, 1);	//GPIOA.4����ΪI2C0 SCL����
	PORT->PORTA_PULLU |= (1 << PIN4);				//����ʹ������������ģ�⿪©
	PORT_Init(PORTA, PIN5, FUNMUX1_I2C0_SDA, 1);	//GPIOA.5����ΪI2C0 SDA����
	PORT->PORTA_PULLU |= (1 << PIN5);				//����ʹ������������ģ�⿪©
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.Addr7b = 1;
	I2C_initStruct.MstClk = 10000;
	I2C_initStruct.MstIEn = 0;
	I2C_Init(I2C0, &I2C_initStruct);
	
	I2C_Open(I2C0);
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
