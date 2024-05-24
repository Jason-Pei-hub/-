#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

int main() 
{
    const char* Filename = "input/test.bmp";

    int width, height;

   

    int his[256] = { 0 };
    int hisout[256] = { 0 };

    //double liangdu1 = 0;
    //double duibidu1 = 0;
    //double liangdu2 = 0;
    //double duibidu2 = 0;

    uint8_t* ImageData = readColorBMP(Filename,&width,&height);//��ȡ24λ��ɫͼ���BMP�ļ�

    //uint8_t* grayScaleImageData = readGrayScaleBMP(Filename, &width, &height);//��ȡ8λ�Ҷ�ͼƬ

    //uint8_t* pTmpImg = (uint8_t*)malloc(width * height);

    //uint8_t* pResImgout = (uint8_t*)malloc(width * height);

    uint8_t* grayScaleImageData = (uint8_t*)malloc(width * height);//���ٿռ��ŻҶ�ֵͼƬ
    
    convertToGray(ImageData, grayScaleImageData,width,height);//24λ��ɫͼ��ת8λ�Ҷ�ֵ

    


    //int* pSumImg = (int*)malloc(grayScaleWidth * grayScaleHeight * sizeof(int));//�������ͼ���ڴ�

    //RmwDoSumGryImg(grayScaleImageData, grayScaleWidth, grayScaleHeight,pSumImg);//�����л��ֵĻ���ͼʵ��

    //GetHistogram(grayScaleImageData, width, height,his);

    //GetBrightContrast(his, &liangdu1, &duibidu1);
    // ִ�жԻҶ�ͼ��Ĳ���
    //RmwHistogramEqualize(grayScaleImageData,width, height);//ֱ��ͼ���⻯
    //LinearStretchDemo(grayScaleImageData, width, height,3,-60);
    //uint8_t* pResImg = (uint8_t*)malloc(width * height);


    //RmwGradientGryImgPlus(grayScaleImageData,width,height, pResImg,15);//�ݶ�����
    //RmwRobertsGryImg(grayScaleImageData, width, height, pResImg);//�޲�������
    //RmwSobelGryImg(grayScaleImageData, width, height, pResImg);//����������
    //RmwPrewittGryImg(grayScaleImageData, width, height, pResImg); //Prewitt����
    //RmwShenJunGryImg(pResImg,pTmpImg, width, height,0.01, pResImgout);//������
    
    //RmwExtractRiceEdge(grayScaleImageData, pTmpImg, width, height,0.01,5, pResImg);//��������������
    //invertImage(pResImg,width,height);//����
    //LinearStretchDemo(grayScaleImageData, grayScaleWidth, grayScaleHeight,1.8,0);
    //RmwHistogramEqualize(grayScaleImageData, grayScaleWidth, grayScaleHeight);

    

    //RmwMedianFilter(grayScaleImageData, grayScaleWidth, grayScaleHeight,5,5,pResImg);//��ֵ�˲�
    //RmwBinImgFilter(grayScaleImageData, grayScaleWidth, grayScaleHeight,5,5,150,pResImg);//��ֵ�˲�

    //RmwAvrFilterBySumImg(pSumImg, grayScaleWidth, grayScaleHeight,5,5, pResImg);//���ڻ���ͼ�Ŀ��پ�ֵ�˲�  

    //GetHistogram(pResImg, width, height, hisout);

    //GetBrightContrast(hisout, &liangdu2, &duibidu2);

    // ����8λ�Ҷ�ͼ������ΪBMP�ļ�
    saveGrayScaleBMP("output/testout111.bmp", grayScaleImageData, width, height);
    //saveColorBMP("testout222.bmp", ImageData ,width,height);
    //saveGrayScaleBMP("endout111.bmp", pResImg, width, height);
    


    //printf("ԭʼͼ�������Ϊ��%.0f,�Աȶ�Ϊ%.0f\n", liangdu1, duibidu1);
    //printf("���ͼ�������Ϊ��%.0f,�Աȶ�Ϊ%.0f\n", liangdu2, duibidu2);

    free(grayScaleImageData);
    //free(pSumImg);
    //free(ImageData);
    //free(pTmpImg);


    return 0;
}
