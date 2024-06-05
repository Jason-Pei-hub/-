#include "delay.h"
#include "stm32f10x.h"

void delay_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM7ʱ��
	TIM_TimeBaseInitStructure.TIM_Period    = 50000-1; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 60-1;     //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
}

//΢�뼶��ʱ
void delay_us(u16 xus)
{
	TIM_Cmd(TIM7,ENABLE); //������ʱ��
	while(TIM7->CNT < xus);
	TIM7->CNT = 0;
	TIM_Cmd(TIM7,DISABLE); //�رն�ʱ��
}

//���뼶��ʱ
void delay_ms(u16 xms)
{
	int i;
	for(i=0;i<xms;i++)
	{
		delay_us(1000);
	}
}

//�뼶��ʱ
void delay_s(u16 xs)
{
	int i;
	for(i=0;i<xs;i++)
	{
		delay_ms(1000);
	}
}
