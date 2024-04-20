#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

int main() 
{
    const char* grayScaleFilename = "H0606Gry.bmp";

    int grayScaleWidth, grayScaleHeight;

   

    int his[256] = { 0 };
    int hisout[256] = { 0 };

    double liangdu1 = 0;
    double duibidu1 = 0;
    double liangdu2 = 0;
    double duibidu2 = 0;

    // ��ȡ8λ�Ҷ�ͼ��
    uint8_t* grayScaleImageData = readGrayScaleBMP(grayScaleFilename, &grayScaleWidth, &grayScaleHeight);
    if (!grayScaleImageData) {
        fprintf(stderr, "Failed to read gray scale BMP image\n");
        return 1;
    }

    //int* pSumImg = (int*)malloc(grayScaleWidth * grayScaleHeight * sizeof(int));//�������ͼ���ڴ�

    //RmwDoSumGryImg(grayScaleImageData, grayScaleWidth, grayScaleHeight,pSumImg);//�����л��ֵĻ���ͼʵ��

    GetHistogram(grayScaleImageData, grayScaleWidth, grayScaleHeight,his);

    GetBrightContrast(his, &liangdu1, &duibidu1);
    // ִ�жԻҶ�ͼ��Ĳ���
    //LinearStretchDemo(grayScaleImageData, grayScaleWidth, grayScaleHeight,1.8,0);
    //RmwHistogramEqualize(grayScaleImageData, grayScaleWidth, grayScaleHeight);

    uint8_t* pResImg = (uint8_t*)malloc(grayScaleWidth * grayScaleHeight);

    //RmwMedianFilter(grayScaleImageData, grayScaleWidth, grayScaleHeight,5,5,pResImg);//��ֵ�˲�
    RmwBinImgFilter(grayScaleImageData, grayScaleWidth, grayScaleHeight,5,5,150,pResImg);//��ֵ�˲�

    //RmwAvrFilterBySumImg(pSumImg, grayScaleWidth, grayScaleHeight,5,5, pResImg);//���ڻ���ͼ�Ŀ��پ�ֵ�˲�  

    GetHistogram(pResImg, grayScaleWidth, grayScaleHeight, hisout);

    GetBrightContrast(hisout, &liangdu2, &duibidu2);

    // ����8λ�Ҷ�ͼ������ΪBMP�ļ�
    saveGrayScaleBMP("out.bmp", pResImg, grayScaleWidth, grayScaleHeight);


    printf("ԭʼͼ�������Ϊ��%.0f,�Աȶ�Ϊ%.0f\n", liangdu1, duibidu1);
    printf("���ͼ�������Ϊ��%.0f,�Աȶ�Ϊ%.0f\n", liangdu2, duibidu2);

    free(grayScaleImageData);
    free(grayScaleImageData);
    //free(pSumImg);
    free(pResImg);


    return 0;
}
