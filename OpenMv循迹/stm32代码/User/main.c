#include "sys.h"

int main(void){	 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//�жϷ���
	
	/*1.���������� 2.��������(�ַ���) 3.��ջ��С 4.���񴫵ݲ���	5.�������ȼ�	 6.������	*/
	xTaskCreate(Create_Task,"Create_Task",512,NULL,1,NULL);
	
	vTaskStartScheduler();//����������
	
	while(1){  
		
	}
}
