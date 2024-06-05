#include "mytimer.h"       


int16_t Encoder_Count=0;
int16_t	motor = 0;
int16_t	Expect_Speed = 15;
uint8_t stop_flag = 0;

int16_t servos_pwm=1166;


void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ѡ��APB1�����µĶ�ʱ��Timer2
	
	TIM_InternalClockConfig(TIM5);		//ʹ���ڲ�ʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//����ģʽ���˴�Ϊ���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;		//ARR 
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;		//�߼���ʱ�����У��ظ�����
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);		//ʹ���ж�
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;		//�ж�ͨ��ѡ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//���ȼ���ͬ��
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM5, ENABLE);		//�򿪶�ʱ��
}

void TIM5_IRQHandler(void)		
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
	{
		
		Encoder_Count = (Read_Encoder(1)+Read_Encoder(2))/2;//��ȡ������
		
		if(stop_flag==1){									//ͣ��
			TIM1->CCR1 = 0;
			TIM1->CCR2 = 0;
			err_sum=0;//��ջ���ֵ
			Servos_err_sum=0;
		}
		else{
			
			motor=Speed_PID(Expect_Speed,Encoder_Count);		//PID����pwmֵ
			Pwm_Out(motor,motor);							//���pwm
			
			servos_pwm=Axis_PID(160,OpenMv_x);
			servos_pwm=1166-servos_pwm;
			Servos_Pwm(servos_pwm);
		}
		
		
		
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);		//��ձ�־λ
	}
}
