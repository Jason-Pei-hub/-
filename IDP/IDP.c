#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

//��ȡ8λ�Ҷ�ͼƬ
//filename���ַ������ָ�룬����ָ��Ҫ�����ͼ���ļ������ƻ�·����
//imageData���޷��� 8 λ�������ݵ�ָ�룬����Ҫ�����ͼ�����ݡ�
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
uint8_t* readGrayScaleBMP(const char* filename, int* width, int* height) 
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    // ��ȡBMP�ļ�ͷ����Ϣ
    uint8_t bmpHeader[54];
    fread(bmpHeader, 1, 54, file);

    // ���ļ�ͷ����ȡͼ���Ⱥ͸߶���Ϣ
    *width = *(int*)&bmpHeader[18];
    *height = *(int*)&bmpHeader[22];

    // ����洢ͼ�����ݵ��ڴ�
    uint8_t* imageData = (uint8_t*)malloc(*width * *height);
    if (!imageData) {
        fprintf(stderr, "�ڴ����ʧ��\n");
        fclose(file);
        return NULL;
    }

    // �����ɫ��Ĵ�С
    int paletteSize = *(int*)&bmpHeader[46];
    if (paletteSize == 0)
        paletteSize = 256;

    // ��ȡ��ɫ������
    uint8_t palette[1024];
    fread(palette, 1, paletteSize * 4, file);

    // ��ȡͼ������
    fseek(file, *(int*)&bmpHeader[10], SEEK_SET);
    fread(imageData, 1, *width * *height, file);

    fclose(file);

    return imageData;
}

// ��8λ�Ҷ�ͼ�����ݱ���ΪBMP�ļ�
//filename���ַ������ָ�룬����ָ��Ҫ�����ͼ���ļ������ƻ�·����
//imageData���޷��� 8 λ�������ݵ�ָ�룬����Ҫ�����ͼ�����ݡ�
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
void saveGrayScaleBMP(const char* filename, const uint8_t* imageData, int width, int height) 
{
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error creating file %s\n", filename);
        return;
    }

    // BMP�ļ�ͷ����Ϣ
    uint8_t bmpHeader[54] = {
        0x42, 0x4D,             // �ļ����ͱ�ʶ "BM"
        0x36, 0x00, 0x0C, 0x00, // �ļ���С�����ֽ�Ϊ��λ���˴�����ͼ�����ݴ�С������4GB��
        0x00, 0x00,             // �����ֶ�
        0x00, 0x00,             // �����ֶ�
        0x36, 0x00, 0x00, 0x00, // λͼ����ƫ�ƣ����ֽ�Ϊ��λ��
        0x28, 0x00, 0x00, 0x00, // λͼ��Ϣͷ��С��40�ֽڣ�
        0x00, 0x00, 0x00, 0x00, // ͼ����
        0x00, 0x00, 0x00, 0x00, // ͼ��߶�
        0x01, 0x00,             // Ŀ���豸�ļ��𣨴˴�Ϊ1����ѹ����
        0x08, 0x00,             // ÿ�����ص�λ����8λ��
        0x00, 0x00, 0x00, 0x00, // ѹ�����ͣ��˴�Ϊ��ѹ����
        0x00, 0x00, 0x00, 0x00, // ͼ�����ݴ�С�����ֽ�Ϊ��λ���˴�Ϊ0����ʾ��ѹ����
        0x00, 0x00, 0x00, 0x00, // ˮƽ�ֱ��ʣ�����/�ף��˴�Ϊ0����ʾδ֪��
        0x00, 0x00, 0x00, 0x00, // ��ֱ�ֱ��ʣ�����/�ף��˴�Ϊ0����ʾδ֪��
        0x00, 0x00, 0x00, 0x00, // ʹ�õ���ɫ��������0��ʾʹ�����е�ɫ���
        0x00, 0x00, 0x00, 0x00  // ��Ҫ����ɫ��������0��ʾ������ɫ����Ҫ��
    };

    // ����BMP�ļ�ͷ����Ϣ�еĿ�Ⱥ͸߶�
    *(int*)&bmpHeader[18] = width;
    *(int*)&bmpHeader[22] = height;

    // д��BMP�ļ�ͷ����Ϣ
    fwrite(bmpHeader, 1, 54, file);

    // д���ɫ������
    for (int i = 0; i < 256; i++) {
        fputc(i, file);  // ��ɫ����
        fputc(i, file);  // ��ɫ����
        fputc(i, file);  // ��ɫ����
        fputc(0, file);  // �����ֽ�
    }

    // д��ͼ������
    fwrite(imageData, 1, width * height, file);

    fclose(file);
}

// ��ȡ24λ��ɫͼ���BMP�ļ�
//filename���ַ������ָ�룬����ָ��Ҫ��ȡ�� BMP ��ʽͼ���ļ������ƻ�·����
//width�����ͱ�����ָ�룬���ڴ洢��ȡ��ͼ��Ŀ�ȡ�
//height�����ͱ�����ָ�룬���ڴ洢��ȡ��ͼ��ĸ߶ȡ�
uint8_t* readColorBMP(const char* filename, int* width, int* height) 
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    // ��ȡBMP�ļ�ͷ����Ϣ
    uint8_t bmpHeader[54];
    fread(bmpHeader, 1, 54, file);

    // ���ļ�ͷ����ȡͼ���Ⱥ͸߶���Ϣ
    *width = *(int*)&bmpHeader[18];
    *height = *(int*)&bmpHeader[22];

    // ����洢ͼ�����ݵ��ڴ�
    uint8_t* imageData = (uint8_t*)malloc(*width * *height * 3);
    if (!imageData) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // ��ȡͼ������
    fseek(file, *(int*)&bmpHeader[10], SEEK_SET);
    fread(imageData, 1, *width * *height * 3, file);

    fclose(file);

    return imageData;
}

//��24λ��ɫͼ�����ݱ���ΪBMP�ļ�
//filename���ַ������ָ�룬����ָ��Ҫ�����ͼ���ļ������ƻ�·����
//imageData���޷��� 8 λ�������ݵ�ָ�룬����Ҫ�����ͼ�����ݡ�
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
void saveColorBMP(const char* filename, const uint8_t* imageData, int width, int height) 
{
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error creating file %s\n", filename);
        return;
    }

    // BMP�ļ�ͷ����Ϣ
    uint8_t bmpHeader[54] = {
        0x42, 0x4D,             // �ļ����ͱ�ʶ "BM"
        0x00, 0x00, 0x00, 0x00, // �ļ���С��ռλ���Ժ���㣩
        0x00, 0x00,             // �����ֶ�
        0x00, 0x00,             // �����ֶ�
        0x36, 0x00, 0x00, 0x00, // λͼ����ƫ�ƣ����ֽ�Ϊ��λ��
        0x28, 0x00, 0x00, 0x00, // λͼ��Ϣͷ��С��40�ֽڣ�
        0x00, 0x00, 0x00, 0x00, // ͼ����
        0x00, 0x00, 0x00, 0x00, // ͼ��߶�
        0x01, 0x00,             // Ŀ���豸�ļ��𣨴˴�Ϊ1����ѹ����
        0x18, 0x00,             // ÿ�����ص�λ����24λ��
        0x00, 0x00, 0x00, 0x00, // ѹ�����ͣ��˴�Ϊ��ѹ����
        0x00, 0x00, 0x00, 0x00, // ͼ�����ݴ�С��ռλ���Ժ���㣩
        0x00, 0x00, 0x00, 0x00, // ˮƽ�ֱ��ʣ�����/�ף��˴�Ϊ0����ʾδ֪��
        0x00, 0x00, 0x00, 0x00, // ��ֱ�ֱ��ʣ�����/�ף��˴�Ϊ0����ʾδ֪��
        0x00, 0x00, 0x00, 0x00, // ʹ�õ���ɫ��������0��ʾʹ�����е�ɫ���
        0x00, 0x00, 0x00, 0x00  // ��Ҫ����ɫ��������0��ʾ������ɫ����Ҫ��
    };

    // ����BMP�ļ�ͷ����Ϣ�еĿ�Ⱥ͸߶�
    *(int*)&bmpHeader[18] = width;
    *(int*)&bmpHeader[22] = height;

    // ����ͼ�����ݴ�С
    uint32_t imageDataSize = width * height * 3 + 54; // �����ļ�ͷ����С
    bmpHeader[2] = (uint8_t)(imageDataSize & 0xFF);
    bmpHeader[3] = (uint8_t)((imageDataSize >> 8) & 0xFF);
    bmpHeader[4] = (uint8_t)((imageDataSize >> 16) & 0xFF);
    bmpHeader[5] = (uint8_t)((imageDataSize >> 24) & 0xFF);

    // д��BMP�ļ�ͷ����Ϣ
    fwrite(bmpHeader, 1, 54, file);

    // д��ͼ������
    fwrite(imageData, width * height * 3, 1, file);

    fclose(file);
}

//�Ҷ���������
//pGryImg���Ҷ�ͼ�����ݵ�ָ�롣
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
//k�����������б�ʡ�����������������ʻ�̶ȡ���(k) ���� 1 ʱ��ͼ��ĶԱȶ����ӣ���(k) С�� 1 ʱ���ԱȶȽ��͡�
//b�����������ƫ�ơ��������������Ҷ�ֵ����ʼλ�á���(b) ���� 0 ʱ��ͼ��������������ӣ���(b) С�� 0 ʱ���������ȼ�С��
void LinearStretchDemo(uint8_t* pGryImg, int width, int height, double k, double b)
{
    uint8_t* pCur, * pEnd;
    int LUT[256];    //��Ϊֻ��[0,255]��256���Ҷ�ֵ

    //step1. ���ɲ��ұ�
    for (int g = 0; g < 256; g++)
    {
        LUT[g] = max(0, min(255, k * g + b));
    }

    //step2. ���б任
    for (pCur = pGryImg, pEnd = pGryImg + width * height; pCur < pEnd; pCur++)
    {
        *pCur = LUT[*pCur];
    }
    //step3. ����
    return;
}

//ͳ��ͼ��Ҷ�ֵ
//pImg���Ҷ�ͼ�����ݵ�ָ�롣
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
//* histogram��������Ԫ�ص�ַ����Ҫһ���ܴ���256����������������
void GetHistogram(uint8_t* pImg, int width, int height, int* histogram)
{
    uint8_t* pCur;
    uint8_t* pEnd = pImg + width * height;

    // ��ʼ��ֱ��ͼ����
    memset(histogram, 0, sizeof(int) * 256);

    // ֱ��ͼͳ��
    for (pCur = pImg; pCur < pEnd;)
    {
        histogram[*pCur]++;
        pCur++;
    }

    // ��������
    return;
}

//���ȺͶԱȶ�
//����histogram�Ҷ�ֱ��ͼ��ָ��
//�������ȵı�����ַ
//���նԱȶȵı�����ַ
void GetBrightContrast(int* histogram, double* bright, double* contrast)
{
    int g;
    double sum, num; //����˵ͼ�����ʱ��int�п��ܻ����������������ֱ����double
    double fsum;

    //step.1 ������
    for (sum = num = 0, g = 0; g < 256; g++)
    {
        sum += histogram[g] * g;
        num += histogram[g];
    }
    *bright = sum * 1.0 / num;

    //step.2 ��Աȶ�
    for (fsum = 0.0, g = 0; g < 256; g++)
    {
        fsum += histogram[g] * (g - *bright) * (g - *bright);
    }
    *contrast = sqrt(fsum / (num - 1)); //��Std Dev

    //step.3 ����
    return;
}

//pGryImg���Ҷ�ͼ�����ݵ�ָ�롣
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
void RmwHistogramEqualize(uint8_t* pGryImg, int width, int height)
{
    uint8_t* pCur, * pEnd = pGryImg + width * height; // ָ�������ָ��ǰ���غ�ͼ��ĩβ
    int histogram[256], LUT[256], A, g; // ֱ��ͼ���顢���ұ����顢�ۻ�ֱ��ͼ���Ҷȼ�

    // step.1-------------��ֱ��ͼ--------------------------//
    memset(histogram, 0, sizeof(int) * 256); // ��ʼ��ֱ��ͼ����Ϊ0
    for (pCur = pGryImg; pCur < pEnd;)
        histogram[*(pCur++)]++; // ͳ��ÿ���Ҷȼ����ֵ�Ƶ��

    // step.2-------------��LUT[g]-------------------------//
    A = histogram[0]; // ��ʼ���ۻ�ֱ��ͼ��ֵΪ��һ���Ҷȼ���Ƶ��
    LUT[0] = 255 * A / (width * height); // �����һ���Ҷȼ���Ӧ�ľ��⻯��ĻҶ�ֵ
    for (g = 1; g < 256; g++) {
        A += histogram[g]; // �����ۻ�ֱ��ͼ��ֵ
        LUT[g] = 255 * A / (width * height); // ���㵱ǰ�Ҷȼ���Ӧ�ľ��⻯��ĻҶ�ֵ
    }

    // step.3-------------���------------------------------//
    for (pCur = pGryImg; pCur < pEnd;)
        *(pCur++) = LUT[*pCur]; // ʹ�ò��ұ��ÿ�����ؽ��лҶ�ӳ��

    // step.4-------------����------------------------------//
    return;
}

//�����任
//pGryImg���Ҷ�ͼ�����ݵ�ָ�롣
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
void RmwLogTransform(uint8_t* pGryImg, int width, int height)
{
    uint8_t* pCur, * pEnd = pGryImg + width * height; // ָ��Ҷ�ͼ�����ݵĵ�ǰָ��ͽ���ָ��
    int histogram[256], LUT[256], gmax, g; // ����ֱ��ͼ���顢���ұ����顢���Ҷ�ֵ����ǰ�Ҷ�ֵ
    double c; // ��������c

    // step.1-------------��ֱ��ͼ--------------------------//
    memset(histogram, 0, sizeof(int) * 256); // ��ʼ��ֱ��ͼ����Ϊ0
    for (pCur = pGryImg; pCur < pEnd;)
        histogram[*(pCur++)]++; // ����ͼ�����ݣ�ͳ��ÿ���Ҷȼ�����������

    // step.2-------------���ֵ---------------------------//
    for (gmax = 255; gmax >= 0; gmax++)
        if (histogram[gmax]) break; // �����Ҷȼ���ʼ��ͻҶȼ��������ҵ���һ������Ҷȼ��������Ҷ�ֵ

    // step.3-------------��LUT[g]-------------------------//
    c = 255.0 / log(1 + gmax); // ���㳣��c
    for (g = 0; g < 256; g++)
    {
        LUT[g] = (int)(c * log(1 + g)); // ���ݶ����任��ʽ������ұ���ÿ���Ҷȼ���ӳ��ֵ
    }

    // step.4-------------���------------------------------//
    for (pCur = pGryImg; pCur < pEnd;)
        *(pCur++) = LUT[*pCur]; // ʹ�ò��ұ�ͼ�����ݽ��ж����任

    // step.5-------------����------------------------------//
    return; // ��������
}
