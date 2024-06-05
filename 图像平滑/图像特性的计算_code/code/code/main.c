#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

int main()
{
    int width, height;//ͼ��Ŀ�͸�

    // ��ȡͼ��
    const char* img = "input/test.bmp";

    unsigned char* imageData = readColorBMP(img,&width,&height);//��ȡ24λ��ɫͼ���BMP�ļ�

    //[�������Ƕ���Ҷ�ֵͼƬ��������Ҫ����ɫͼ��ת��Ϊ�Ҷ�ͼ��]
    unsigned char* grayImage = (unsigned char*)malloc(width*height*1);//Ϊ�Ҷ�ֵͼƬ�����ڴ�
    convertToGray(imageData,grayImage,width,height);//24λ��ɫͼ��ת8λ�Ҷ�ֵ
    
    //[����ͼƬ��ͳ������]
    int histogram[256] = {0};//�����Ҷ�ֱֵ��ͼ
    GetHistogram(grayImage,width,height,histogram);//ͳ��ͼ��Ҷ�ֵ

    double bright, contrast;//���ȺͶԱȶ�
    GetBrightContrast(histogram,&bright,&contrast);//���ȺͶԱȶ�
    double mean = Median( histogram, width,height);//���м�ֵ

    int size = sizeof(histogram)/sizeof(histogram[0]);//ֱ��ͼ�Ĵ�С
    displayHistogram(histogram,size,bright,contrast,mean);//��ʾֱ��ͼ

    printf("Brightness: %.2f\n", bright);
    printf("Contrast: %.2f\n", contrast);
    printf("Median: %.2f\n", mean);

    printf("��������Ԥ������ͼƬ...\n");

    //[����Ԥ������ͼƬ]
    printf("���ڽ���ͼ����ǿ...\n");
    printf("###########################################\n");

    //[ͼ����ǿ]
    unsigned char* grayImage1 = (unsigned char*)malloc(width * height * 1);//Ϊ�Ҷ�ֵ����ͼƬ�����ڴ�
    memcpy(grayImage1, grayImage, width * height * 1);//����ԭͼ
    RmwHistogramEqualize(grayImage1, width, height);//ֱ��ͼ���⻯
    saveGrayScaleBMP("output/ֱ��ͼ���⻯.bmp", grayImage1,width,height);
    printf("�����ͼ����ǿ��ֱ��ͼ���⻯\n");

    unsigned char* grayImage2 = (unsigned char*)malloc(width * height * 1);//Ϊ�Ҷ�ֵ����ͼƬ�����ڴ�
    memcpy(grayImage2, grayImage, width * height * 1);//����ԭͼ
    LinearStretchDemo(grayImage2,width,height,3,-60);
    saveGrayScaleBMP("output/��������.bmp", grayImage2,width,height);
    printf("�����ͼ����ǿ����������\n");

    unsigned char* grayImage3 = (unsigned char*)malloc(width * height * 1);//Ϊ�Ҷ�ֵ����ͼƬ�����ڴ�
    memcpy(grayImage3, grayImage, width * height * 1);//����ԭͼ
    RmwLogTransform(grayImage3,width,height);//�����任
    saveGrayScaleBMP("output/�����任.bmp", grayImage3,width,height);
    printf("�����ͼ����ǿ�Ķ����任\n");

    //[ͼ���˲�]
    printf("���ڽ���ͼ���˲�...\n");
    printf("###########################################\n");

    unsigned char* grayImage4 = (unsigned char*)malloc(width * height * 1);//Ϊ�Ҷ�ֵ����ͼƬ�����ڴ�
    memcpy(grayImage4, grayImage, width * height * 1);//����ԭͼ
    int* pSumImg = (int*)malloc(width * height * sizeof(int));//�������ͼ���ڴ�
    RmwDoSumGryImg(grayImage4,width,height,pSumImg);//�����л��ֵĻ���ͼʵ��
    RmwAvrFilterBySumCol(grayImage,width,height,3,3, grayImage4);//�����л��ֵĿ��پ�ֵ�˲�
    saveGrayScaleBMP("output/�����л��ֵĿ����˲�.bmp", grayImage4,width,height);
    printf("�����ͼ���˲��Ļ����л��ֵĿ����˲�\n");

    unsigned char* grayImage5 = (unsigned char*)malloc(width * height * 1);//Ϊ�Ҷ�ֵ����ͼƬ�����ڴ�
    RmwMeanFilter(grayImage,width,height,3,3, grayImage5);//��ֵ�˲�
    saveGrayScaleBMP("output/��ֵ�˲�.bmp", grayImage5,width,height);
    printf("�����ͼ���˲��ľ�ֵ�˲�\n");

    unsigned char* grayImage6 = (unsigned char*)malloc(width * height * 1);//Ϊ�Ҷ�ֵ����ͼƬ�����ڴ�
    RmwMedianFilter(grayImage,width,height,3,3, grayImage6);//��ֵ�˲�
    saveGrayScaleBMP("output/��ֵ�˲�.bmp", grayImage6,width,height);
    printf("�����ͼ���˲�����ֵ�˲�\n");

    unsigned char* grayImage8 = (unsigned char*)malloc(width * height * 1);//Ϊ�Ҷ�ֵ����ͼƬ�����ڴ�
    RmwBinImgFilter(grayImage, width, height, 3, 3, 100,grayImage8);//��ֵ�˲�
    saveGrayScaleBMP("output/��ֵ�˲�.bmp", grayImage8,width,height);
    printf("�����ͼ���˲��Ķ�ֵ�˲�.bmp\n");


    unsigned char* grayImage7 = (unsigned char*)malloc(width * height * 1);//Ϊ�Ҷ�ֵ����ͼƬ�����ڴ�

    //[�ͷ��ڴ�]
    free(imageData);
    free(grayImage1);
    free(grayImage);//�ͷ��ڴ�
    fclose(img);

    return 0;
}


