#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void F1(uint8_t* pImgRGB, int width, int height)
{
    uint8_t* pCur;
    uint8_t* pEnd;

    pEnd = pImgRGB + width * height * 3;  // ÿ�����ص�ռ�� 3 ���ֽڣ�BGR��
    for (pCur = pImgRGB; pCur < pEnd; pCur += 3) {
        uint8_t blue = pCur[0];
        uint8_t green = pCur[1];
        uint8_t red = pCur[2];
        printf("R: %d, G: %d, B: %d ", red, green, blue);
    }
}

int main()
{
    const char* filename = "E:/code/IDP-Learning-Journey/images/ME.bmp";

    FILE* file = fopen(filename, "rb");

    uint8_t bmpHeader[54];
    fread(bmpHeader, 1, 54, file);

    int width = *(int*)&bmpHeader[18];   // �����Ϣλ��ƫ����Ϊ 18 ��λ��
    int height = *(int*)&bmpHeader[22];  // �߶���Ϣλ��ƫ����Ϊ 22 ��λ��

    uint8_t* imageData = (uint8_t*)malloc(width * height * 3);  // ÿ�����ص�ռ�� 3 ���ֽڣ�BGR��

    fseek(file, 54, SEEK_SET);  // ���� BMP �ļ�ͷ

    fread(imageData, 3, width * height, file);  // ÿ�����ص�ռ�� 3 ���ֽڣ�BGR��

    F1(imageData, width, height);

    fclose(file);
    free(imageData);

    return 0;
}
