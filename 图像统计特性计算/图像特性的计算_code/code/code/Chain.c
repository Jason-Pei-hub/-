#define _CRT_SECURE_NO_WARNINGS 1


#include "IDP.h"


int main() 
{
    int width, height;
    uint8_t* imageData = read1BitBMP("input/�������ʵ��ͼ��.bmp", &width, &height);

    printf("ͼ���ȣ�%d\n", width);
    printf("ͼ��߶ȣ�%d\n", height);


    ChainCode(imageData, width, height);

   
    printf("�������\n");
    free(imageData);

    return 0;
}

