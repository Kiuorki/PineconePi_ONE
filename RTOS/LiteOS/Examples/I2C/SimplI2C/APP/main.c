#include "SWM320.h"

#include <string.h>

#define SLV_ADDR  0x6C

char mst_txbuff[4] = {0x37, 0x55, 0xAA, 0x78};
char mst_rxbuff[4] = {0};
char slv_txbuff[4] = {0};
char slv_rxbuff[4] = {0};

volatile uint32_t slv_rxindx = 0;
volatile uint32_t slv_txindx = 0;

void SerialInit(void);
void I2C_Mst_Init(void);
void I2C_Slv_Init(void);

int main(void)
{
	uint32_t i;
	SystemInit();
	
	SerialInit();
	
	I2C_Mst_Init();
	I2C_Slv_Init();
	
	while(1==1)
	{
		/*************************** Master Write ************************************/
		slv_rxindx = 0;
		
		I2C0->MSTDAT = (SLV_ADDR << 1) | 0;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
					   (1 << I2C_MSTCMD_WR_Pos);			//������ʼλ�ʹӻ���ַ
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		else
		{
			printf("Slave send ACK for address\r\n");
		}
		
		for(i = 0; i < 4; i++)
		{
			I2C0->MSTDAT = mst_txbuff[i];
			I2C0->MSTCMD = (1 << I2C_MSTCMD_WR_Pos);
			while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;		//�ȴ��������
			if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
			{
				printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
			else
			{
				printf("Slave send ACK for data\r\n");
			}
		}
		
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STO_Pos);
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		
		printf("Master Send %X %X %X %X\r\n", mst_txbuff[0], mst_txbuff[1], mst_txbuff[2], mst_txbuff[3]);
		
		
		/********************************** Master Read *******************************/
		slv_txindx = 0;
		memcpy(slv_txbuff, slv_rxbuff, 4);
		
		I2C0->MSTDAT = (SLV_ADDR << 1) | 1;
		I2C0->MSTCMD = (1 << I2C_MSTCMD_STA_Pos) |		
					   (1 << I2C_MSTCMD_WR_Pos);			//������ʼλ�ʹӻ���ַ
		while(I2C0->MSTCMD & I2C_MSTCMD_TIP_Msk) ;			//�ȴ��������
		if(I2C0->MSTCMD & I2C_MSTCMD_RXACK_Msk)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		else
		{
			printf("Slave send ACK for address\r\n");
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
		
		printf("Master Read %X %X %X %X\r\n", mst_rxbuff[0], mst_rxbuff[1], mst_rxbuff[2], mst_rxbuff[3]);
		
		if((mst_txbuff[0] == mst_rxbuff[0]) && (mst_txbuff[1] == mst_rxbuff[1]) && (mst_txbuff[2] == mst_rxbuff[2]) && (mst_txbuff[3] == mst_rxbuff[3]))
			printf("Master Read == Master Send\r\n\r\n");
		else
			printf("Master Read != Master Send\r\n\r\n");

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

void I2C_Slv_Init(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTA, PIN6, FUNMUX0_I2C1_SCL, 1);		//GPIOA.6����ΪI2C1 SCL����
	PORT->PORTA_PULLU |= (1 << PIN6);					//����ʹ������������ģ�⿪©
	PORT_Init(PORTA, PIN7, FUNMUX1_I2C1_SDA, 1);		//GPIOC.1����ΪI2C1 SDA����
	PORT->PORTA_PULLU |= (1 << PIN7);					//����ʹ������������ģ�⿪©
	
	I2C_initStruct.Master = 0;
	I2C_initStruct.Addr7b = 1;
	I2C_initStruct.SlvAddr = SLV_ADDR;
	I2C_initStruct.SlvSTADetIEn = 1;
	I2C_initStruct.SlvRdReqIEn = 0;
	I2C_initStruct.SlvWrReqIEn = 1;
	I2C_initStruct.SlvTxEndIEn = 1;
	I2C_initStruct.SlvRxEndIEn = 1;
	I2C_initStruct.SlvSTODetIEn = 1;
	I2C_Init(I2C1, &I2C_initStruct);
	
	I2C_Open(I2C1);
}

void I2C1_Handler(void)
{
	if(I2C1->SLVIF & I2C_SLVIF_STADET_Msk)				//�յ���ʼλ
	{
		I2C1->SLVIF = (1 << I2C_SLVIF_STADET_Pos);
		
		I2C1->SLVTX = slv_txbuff[0];
		slv_txindx = 1;
	}
	else if(I2C1->SLVIF & I2C_SLVIF_STODET_Msk)			//�յ�ֹͣλ
	{
		I2C1->SLVIF = (1 << I2C_SLVIF_STODET_Pos);
	}
	else if(I2C1->SLVIF & I2C_SLVIF_WRREQ_Msk)			//�յ�д����
	{
		I2C1->SLVIF = (1 << I2C_SLVIF_WRREQ_Pos);
		
		slv_rxindx = 0;
		I2C1->SLVCR |= (1 << I2C_SLVCR_ACK_Pos);
	}
	else if(I2C1->SLVIF & I2C_SLVIF_RXEND_Msk)			//�������
	{
		I2C1->SLVIF = (1 << I2C_SLVIF_RXEND_Pos);
		
		slv_rxbuff[slv_rxindx] = I2C1->SLVRX;
		if(slv_rxindx < 4) slv_rxindx++;
	}
	else if(I2C1->SLVIF & I2C_SLVIF_RDREQ_Msk)			//�յ�������
	{		
		I2C1->SLVIF = (1 << I2C_SLVIF_RDREQ_Pos);
		
		//�յ���������������SLVTX�е�ֵ���ͳ�ȥ�������ǵȴ����д����ٷ��ͣ����Խ���һ������д��SLVTXֻ����ǰ�����յ���ʼλ��
	}
	else if(I2C1->SLVIF & I2C_SLVIF_TXEND_Msk)			//�������
	{
		I2C1->SLVIF = (1 << I2C_SLVIF_TXEND_Pos);
		
		I2C1->SLVTX = slv_txbuff[slv_txindx];
		if(slv_txindx < 3) slv_txindx++;
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
