#include "SWM320.h"

void SerialInit(void);
void PWMSignal(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	PWMSignal();	// ����PWM�����ź�
	
	PORT_Init(PORTA, PIN5, FUNMUX1_PULSE_IN, 1);
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, 0xFFFFFFFF, 0);	// �������ģ����Ҫʹ��TIMR0������
	
	TIMRG->PCTRL = (0 << TIMRG_PCTRL_CLKSRC_Pos) |		// ϵͳʱ����Ϊʱ��Դ
				   (1 << TIMRG_PCTRL_HIGH_Pos) |		// �����ߵ�ƽ���
				   (1 << TIMRG_PCTRL_EN_Pos);			// ��ʼ����
	
	TIMRG->IE |= (1 << TIMRG_IE_PULSE_Pos);
	NVIC_EnableIRQ(PULSE_IRQn);
	
	printf("Hi from Synwit\r\n");
	
	while(1==1)
	{
	}
}

void PULSE_Handler(void)
{
	if(TIMRG->IF & TIMRG_IF_PULSE_Msk)
	{
		TIMRG->IF = TIMRG_IF_PULSE_Msk;		// ����жϱ�־
		
		printf("%d\r\n", TIMRG->PCVAL);
		
		TIMRG->PCTRL |= (1 << TIMRG_PCTRL_EN_Pos);			// ������ɺ�ʹ���Զ����㣬��Ҫ��������
	}
}


void PWMSignal(void)
{
	PWM_InitStructure  PWM_initStruct;
	
	PWM_initStruct.clk_div = PWM_CLKDIV_8;		//F_PWM = 40M/8 = 5M
	
	PWM_initStruct.mode = PWM_MODE_INDEP;		//A·��B·�������					
	PWM_initStruct.cycleA = 50000;				//   5M/50000 = 100Hz			
	PWM_initStruct.hdutyA = 25000;				//25000/50000 = 50%
	PWM_initStruct.initLevelA = 1;
	PWM_initStruct.cycleB = 50000;
	PWM_initStruct.hdutyB = 20000;
	PWM_initStruct.initLevelB = 1;
	PWM_initStruct.HEndAIEn = 0;
	PWM_initStruct.NCycleAIEn = 0;
	PWM_initStruct.HEndBIEn = 0;
	PWM_initStruct.NCycleBIEn = 0;
	
	PWM_Init(PWM0, &PWM_initStruct);
	
	PORT_Init(PORTA, PIN4, FUNMUX0_PWM0A_OUT, 0);
	PORT_Init(PORTA, PIN6, FUNMUX0_PWM0B_OUT, 0);
	
	PWM_Start(PWM0, 1, 1);
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
