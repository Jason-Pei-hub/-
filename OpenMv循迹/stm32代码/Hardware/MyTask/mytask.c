#include "mytask.h"


TaskHandle_t Esp_Handle;//������


/*��ʼ����*/
void Create_Task(void *arg){
	
	delay_Init();				//��ʱ��ʼ��
	OLED_Init();				//OLED��ʼ��
	UART4_Init(115200);			//debug����
	USART3_Init(115200);		//ESP8266����
	ESP8266_Init();
	
	
	while(OneNet_DevLink())			//���������
		delay_ms(500);
	OLED_ShowString(1,1,"              ");
	
	
	Pwm_Tim1_Init(7200,1);		//���pwm��ʼ��
	Motor_Init();				//��������ʼ��
	Encoder1_Init();			//������1��ʼ��
	Encoder2_Init();			//������2��ʼ��
	Timer_Init();				//��ʱ����ʼ��
	
	UART5_Init(9600);			//OpenMvͨѶ
	Servos_Init();				//�����ʼ��
	
	xTaskCreate(Esp_Task,"Esp_Task",256,NULL,1,&Esp_Handle);//��������
	
	
	vTaskDelete(NULL);
}

void Esp_Task(void *arg){
	
	const char *topic_sub[] = {"/QT_Topic"};//���Ļ�����
	const char topic_pub[] = {"/MCU_Topic"};//����������
	char pub_buf[256];						//�ϴ�������BUF
	unsigned short timeCount = 0;			//���ͼ������
	unsigned char *dataPtr = NULL;
	
	OneNet_Subscribe(topic_sub, 1);
	
	for(;;){
		
		if(++timeCount >= 25){

			//sprintf(pub_buf,"{\"Pitch\":%d,\"Roll\":%d,\"Yaw\":%d,\"Room\":%d,\"Speed\":%d,\"Flag\":%d,\"Power\":%d}",-30,40,50,RoomNum,Expect_Speed,1,(int)Vbatt);
			
			OneNet_Publish(topic_pub,pub_buf);//��������
			timeCount = 0;
			ESP8266_Clear();
		}
		
		dataPtr = ESP8266_GetIPD(2);//�Ƿ���յ���Ϣ
		
		if(dataPtr != NULL)	OneNet_RevPro(dataPtr);	
	
		vTaskDelay(10);
		
	}
}



