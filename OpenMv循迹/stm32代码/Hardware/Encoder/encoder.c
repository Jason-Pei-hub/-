#include "encoder.h" 

void Encoder1_Init(void)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_ICInitTypeDef 			TIM_ICInitInitStructure;
	
	/*ʱ��*/
	/*������������������*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//��ʱ��2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/*ʱ����Ԫ����*/
	/*������������������*/
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;//ARR		Ƶ��:F=72M/(PSC+1)/(ARR+1)=1000Hz
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;//PSCԤ��Ƶ		ռ�ձ�:D=CCR/(ARR+1)=50%
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//		�ֱ���:R=1/(ARR+1)=0.01
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	/*���벶��Ԫ����*/
	/*������������������*/
	TIM_ICStructInit(&TIM_ICInitInitStructure);							//����ʼֵ
	
	TIM_ICInitInitStructure.TIM_Channel=TIM_Channel_1;					//ѡ��ͨ��
	TIM_ICInitInitStructure.TIM_ICFilter=10;							//���벶���˲���
	TIM_ICInit(TIM2,&TIM_ICInitInitStructure);
	
	TIM_ICInitInitStructure.TIM_Channel=TIM_Channel_2;					//ѡ��ͨ��
	TIM_ICInitInitStructure.TIM_ICFilter=10;							//���벶���˲���
	TIM_ICInit(TIM2,&TIM_ICInitInitStructure);
	
	/*���ñ������ӿ�*/
	/*������������������*/
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}

void Encoder2_Init(void)
{
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_ICInitTypeDef 			TIM_ICInitInitStructure;
	
	/*ʱ��*/
	/*������������������*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//��ʱ��3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;//PA6-A��   PA7-B�� 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*ʱ����Ԫ����*/
	/*������������������*/
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;//ARR		Ƶ��:F=72M/(PSC+1)/(ARR+1)=1000Hz
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;//PSCԤ��Ƶ		ռ�ձ�:D=CCR/(ARR+1)=50%
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//		�ֱ���:R=1/(ARR+1)=0.01
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	/*���벶��Ԫ����*/
	/*������������������*/
	TIM_ICStructInit(&TIM_ICInitInitStructure);							//����ʼֵ
	
	TIM_ICInitInitStructure.TIM_Channel=TIM_Channel_1;					//ѡ��ͨ��
	TIM_ICInitInitStructure.TIM_ICFilter=10;							//���벶���˲���
	TIM_ICInit(TIM3,&TIM_ICInitInitStructure);
	
	TIM_ICInitInitStructure.TIM_Channel=TIM_Channel_2;					//ѡ��ͨ��
	TIM_ICInitInitStructure.TIM_ICFilter=10;							//���벶���˲���
	TIM_ICInit(TIM3,&TIM_ICInitInitStructure);
	
	/*���ñ������ӿ�*/
	/*������������������*/
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM3,0);
	
	TIM_Cmd(TIM3,ENABLE);//������ʱ��
}

int32_t Read_Encoder(u8 TIMX)
{
    int32_t Encoder_TIM;    
	switch(TIMX)
	 {
		 case 1:  
				Encoder_TIM= -(short)TIM_GetCounter(TIM2);//TIM3��cntֵ
				TIM_SetCounter(TIM2,0);//��cnt����
				break;	
		 case 2:  
				Encoder_TIM= (short)TIM_GetCounter(TIM3); //TIM2��cntֵ
				TIM_SetCounter(TIM3,0);
				break;	
		 default:  
			 Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
