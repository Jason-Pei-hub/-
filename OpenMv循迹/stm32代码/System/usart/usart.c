#include "usart.h"
int OpenMv_x=0;
int OpenMv_Data[7]={0};

void USART3_Init(u32 BoTeLv){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TXD  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RXD  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=BoTeLv;//������
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//Ӳ�������� none��ʹ��
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;//У��λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//�ֳ�
	USART_Init(USART3,&USART_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;//����2�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3,ENABLE);
}
void UART4_Init(u32 BoTeLv){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TXD  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RXD  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=BoTeLv;//������
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//Ӳ�������� none��ʹ��
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;//У��λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//�ֳ�
	USART_Init(UART4,&USART_InitStructure);
	
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;//����5�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART4,ENABLE);
}
//OpenMv����
void UART5_Init(u32 BoTeLv){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//TXD  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//RXD  
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=BoTeLv;//������
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//Ӳ�������� none��ʹ��
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;//У��λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//�ֳ�
	USART_Init(UART5,&USART_InitStructure);
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;//����5�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART5,ENABLE);
}

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{
	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//��������
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//�ȴ��������
	}
}


void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//��ʽ��
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}





//�������ݰ���ʽ
u8 OpenMv_Data_Receive (int data[])
{
	if(data[0]!=0xaf) return 0;
	if(data[1]!=0xbf) return 0;
	if(data[5]!=0xcf) return 0;
			
	return 1;
}

void UART5_IRQHandler(void)
{
	static u8 i=0;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) //�����ж�
	{
		
		OpenMv_Data[i++]=USART_ReceiveData(UART5);
		if (OpenMv_Data[0]!=0xaf) i=0;
		if ( (i==2) && (OpenMv_Data[1]!=0xbf) ) i=0;
		if(i==6)
		{
			i=0;
			if(OpenMv_Data_Receive(OpenMv_Data))
			{
				OpenMv_x=OpenMv_Data[2]*100+OpenMv_Data[3]*10+OpenMv_Data[4];
			}				
		}
		
		USART_ClearFlag(UART5, USART_FLAG_RXNE);
	}
}




