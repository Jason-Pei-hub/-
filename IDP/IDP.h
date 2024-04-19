#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
 
uint8_t* readGrayScaleBMP(const char* filename, int* width, int* height);//��ȡ8λ�Ҷ�ͼƬ
void saveGrayScaleBMP(const char* filename, const uint8_t* imageData, int width, int height);// ��8λ�Ҷ�ͼ�����ݱ���ΪBMP�ļ�
uint8_t* readColorBMP(const char* filename, int* width, int* height);//��ȡ24λ��ɫͼ���BMP�ļ�
void saveColorBMP(const char* filename, const uint8_t* imageData, int width, int height);//��24λ��ɫͼ�����ݱ���ΪBMP�ļ�
void LinearStretchDemo(uint8_t* pGryImg, int width, int height, double k, double b);//�Ҷ���������
void GetHistogram(uint8_t* pImg, int width, int height, int* histogram);//ͳ��ͼ��Ҷ�ֵ
void GetBrightContrast(int* histogram, double* bright, double* contrast);//���ȺͶԱȶ�
void RmwHistogramEqualize(uint8_t* pGryImg, int width, int height);//ֱ��ͼ���⻯
void RmwLogTransform(uint8_t* pGryImg, int width, int height);//�����任