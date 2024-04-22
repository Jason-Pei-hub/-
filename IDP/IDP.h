#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <nmmintrin.h>
 
uint8_t* readGrayScaleBMP(const char* filename, int* width, int* height);//��ȡ8λ�Ҷ�ͼƬ
void saveGrayScaleBMP(const char* filename, const uint8_t* imageData, int width, int height);// ��8λ�Ҷ�ͼ�����ݱ���ΪBMP�ļ�
uint8_t* readColorBMP(const char* filename, int* width, int* height);//��ȡ24λ��ɫͼ���BMP�ļ�
void saveColorBMP(const char* filename, const uint8_t* imageData, int width, int height);//��24λ��ɫͼ�����ݱ���ΪBMP�ļ�
void convertToGray(uint8_t* rgbImage, uint8_t* grayImage, int width, int height);//24λ��ɫͼ��ת8λ�Ҷ�ֵ
void LinearStretchDemo(uint8_t* pGryImg, int width, int height, double k, double b);//�Ҷ���������
void GetHistogram(uint8_t* pImg, int width, int height, int* histogram);//ͳ��ͼ��Ҷ�ֵ
void GetBrightContrast(int* histogram, double* bright, double* contrast);//���ȺͶԱȶ�
void RmwHistogramEqualize(uint8_t* pGryImg, int width, int height);//ֱ��ͼ���⻯
void RmwLogTransform(uint8_t* pGryImg, int width, int height);//�����任
void RmwAvrFilterBySumCol(uint8_t* pGryImg, int width, int height, int M, int N, uint8_t* pResImg);//�����л��ֵĿ��پ�ֵ�˲�
void RmwDoSumGryImg(uint8_t* pGryImg, int width, int height, int* pSumImg);//�����л��ֵĻ���ͼʵ��
void RmwDoSumGryImg_SSE(uint8_t* pGryImg, int width, int height, int* pSumImg);//����SSE�Ļ���ͼʵ��
void RmwAvrFilterBySumImg(int* pSumImg, int width, int height, int M, int N, uint8_t* pResImg);//���ڻ���ͼ�Ŀ��پ�ֵ�˲�  
void GetMedianGry(int* histogram, int N, int* medGry);//��Ҷ�ֵ��ֵ
double RmwMedianFilter(uint8_t* pGryImg, int width, int height, int M, int N, uint8_t* pResImg);//��ֵ�˲�
void RmwBinImgFilter(uint8_t* pBinImg, int width, int height, int M, int N, double threshold, uint8_t* pResImg);//��ֵ�˲�
void RmwGradientGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pGrdImg);//�ݶ�����
void RmwGradientGryImgPlus(uint8_t* pGryImg, int width, int height, uint8_t* pGrdImg, int threshold);//�ݶ����Ӽ���ֵ
void invertImage(uint8_t* image, int width, int height);//����
void RmwRobertsGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pRbtImg);//�޲�������
void RmwSobelGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pSbImg);//����������
void RmwPrewittGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pPRTImg); //Prewitt����
void RmwShenJunGryImg(uint8_t* pGryImg, uint8_t* pTmpImg, int width, int height, double a0, uint8_t* pSJImg);//������
void RmwExtractRiceEdge(uint8_t* pGryImg, uint8_t* pTmpImg, int width, int height, double a0, int grdThre, uint8_t* pEdgeImg);//��������������