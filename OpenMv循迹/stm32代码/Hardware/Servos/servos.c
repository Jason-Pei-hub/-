#include "servos.h"

void Servos_Init(void){
	/*����ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			//����TIM4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);			//����GPIOA��ʱ��
	
	/*GPIO��ӳ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//����AFIO��ʱ�ӣ���ӳ������ȿ���AFIO��ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);			
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);							//��PB8���ų�ʼ��Ϊ�����������	
																	//��������Ƶ����ţ�����Ҫ����Ϊ����ģʽ
	/*����ʱ��Դ*/
	TIM_InternalClockConfig(TIM4);		//ѡ��TIM4Ϊ�ڲ�ʱ�ӣ��������ô˺�����TIMĬ��ҲΪ�ڲ�ʱ��
	
	/*ʱ����Ԫ��ʼ��*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//����ṹ�����
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //ʱ�ӷ�Ƶ��ѡ�񲻷�Ƶ���˲������������˲���ʱ�ӣ���Ӱ��ʱ����Ԫ����
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ģʽ��ѡ�����ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				//�������ڣ���ARR��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//Ԥ��Ƶ������PSC��ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //�ظ����������߼���ʱ���Ż��õ�
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             //���ṹ���������TIM_TimeBaseInit������TIM4��ʱ����Ԫ
	
	/*����Ƚϳ�ʼ��*/ 
	TIM_OCInitTypeDef TIM_OCInitStructure;							//����ṹ�����
	TIM_OCStructInit(&TIM_OCInitStructure);                         //�ṹ���ʼ�������ṹ��û��������ֵ
	                                                                //�����ִ�д˺��������ṹ�����г�Ա����һ��Ĭ��ֵ
	                                                                //����ṹ���ֵ��ȷ��������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //����Ƚ�ģʽ��ѡ��PWMģʽ1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //������ԣ�ѡ��Ϊ�ߣ���ѡ����Ϊ�ͣ�������ߵ͵�ƽȡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;								//��ʼ��CCRֵ
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);                        //���ṹ���������TIM_OC2Init������TIM4������Ƚ�ͨ��2
	
	/*TIMʹ��*/
	TIM_Cmd(TIM4, ENABLE);			
}

void Servos_SetAngle(float Angle){
	TIM4->CCR1 = (Angle / 180 * 2000 + 500);										
}

void Servos_Pwm(int16_t pwm){   
	
	if(pwm>=1833) pwm=1833;
	if(pwm<=0) pwm=0;
	TIM4->CCR1 = pwm;
}
