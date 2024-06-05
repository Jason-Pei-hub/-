#include "pwm.h"

void Pwm_Tim1_Init(u16 arr,u16 psc){ 
    TIM_TimeBaseInitTypeDef	 	TIM_TimeBaseStructure; //����ṹ��
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_OCInitTypeDef 			TIM_OCInitStructure;
	
	TIM_OCStructInit(&TIM_OCInitStructure);//�ȸ���Ĭ��ֵ��Ȼ���ֶ��������ֵ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ;//�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	//ѡ��ʱ����Ԫ��ʱ��
	TIM_InternalClockConfig(TIM1);//����TIM1��ʱ����Ԫ�����ڲ�ʱ����������
	
	//����ʱ����Ԫ
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//ָ��ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up ;//������ģʽ ���ϼ���
	TIM_TimeBaseStructure.TIM_Period=arr-1;//���� ����ARR�Զ���װ����ֵ    ��ʱƵ��=72M/(PSC+1)/(ARR+1)
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1;//PSCԤ��Ƶ����ֵ
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;//���ظ���������ֵ(�߼���ʱ���Ĺ��ܣ����ﲻ��Ҫ)
    TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//��������Ƚ�ģʽ
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//��������Ƚϵļ���(��Ч��ƽ�Ǹߵ�ƽ,REF��Чʱ������ߵ�ƽ,)
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable   ;//�������ʹ��
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);//ͨ��1  PA8
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//��������Ƚ�ģʽ
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//��������Ƚϵļ���(��Ч��ƽ�Ǹߵ�ƽ,REF��Чʱ������ߵ�ƽ,)
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable   ;//�������ʹ��
	TIM_OC2Init(TIM1,&TIM_OCInitStructure);//ͨ��2 PA9
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);  //Ԥ��װֵ���������PWM
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable); 	//Ԥ��װֵ���������PWM
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//�����Զ���װ
    TIM_Cmd(TIM1,ENABLE);//������ʱ��
}



//2������ռ�ձȵ���
void Pwm_Out(int16_t motor1,int16_t motor2){   
	if(motor1>=0){	
		GPIO_SetBits(GPIOE,GPIO_Pin_2);
		GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	}
	else{
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
		GPIO_ResetBits(GPIOE,GPIO_Pin_2);
	}
	
	if(motor2>=0){
		GPIO_SetBits(GPIOE,GPIO_Pin_6);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	}
	else{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);
		GPIO_ResetBits(GPIOE,GPIO_Pin_6);
	}
	
	motor1=GFP_abs(motor1);//ȡ����ֵ
	motor2=GFP_abs(motor2);//ȡ����ֵ
	
	Limit(&motor1,&motor2,7000);//�޷�
	
	TIM1->CCR1 = motor1; //���
	TIM1->CCR2 = motor2; //���
}

