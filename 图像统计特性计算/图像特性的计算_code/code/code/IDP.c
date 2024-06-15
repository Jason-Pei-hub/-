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
    assert(filename);
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    // ��ȡBMP�ļ�ͷ����Ϣ
    uint8_t bmpHeader[54];
    fread(bmpHeader, 1, 54, file);

    // ���ļ�ͷ����ȡͼ���Ⱥ͸߶���Ϣ���㴫��������
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

//24λ��ɫͼ��ת8λ�Ҷ�ֵ
//rgbImageԭʼͼ��
//grayImage����Ҷ�ͼ��
//width,heightͼƬ�Ŀ�͸�
void convertToGray(uint8_t* rgbImage, uint8_t* grayImage, int width, int height)
{
    assert(rgbImage && grayImage);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            // ��ȡ��ǰ���ص� RGB ����
            uint8_t r = rgbImage[3 * (y * width + x) + 0];
            uint8_t g = rgbImage[3 * (y * width + x) + 1];
            uint8_t b = rgbImage[3 * (y * width + x) + 2];

            // ����Ҷ�ֵ�����õļ�Ȩƽ������
            // ����ʹ�õļ�Ȩϵ���ǳ����ģ�R: 0.299, G: 0.587, B: 0.114
            uint8_t gray = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);

            // ���Ҷ�ֵд��Ҷ�ͼ������
            grayImage[y * width + x] = gray;
        }
    }
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

//�����л��ֵĿ��پ�ֵ�˲�
//ԭʼ�Ҷ�ͼ��
//ͼ��Ŀ�Ⱥ͸߶�
//�˲�����M��N��
//���ͼ��
void RmwAvrFilterBySumCol(uint8_t* pGryImg,int width, int height,int M, int N,uint8_t* pResImg) 
{
    uint8_t* pAdd, * pDel, * pRes;
    int halfx, halfy;
    int x, y;
    int sum, c;
    int sumCol[4096]; // Լ��ͼ���Ȳ�����4096

    // step.1------------��ʼ��--------------------------//
    M = M / 2 * 2 + 1; // ������
    N = N / 2 * 2 + 1; // ������
    halfx = M / 2; // �˲����İ뾶x
    halfy = N / 2; // �˲����İ뾶y
    c = (1 << 23) / (M * N); // �˷�����
    memset(sumCol, 0, sizeof(int) * width);
    for (y = 0, pAdd = pGryImg; y < N; y++) {
        for (x = 0; x < width; x++) sumCol[x] += *(pAdd++);
    }
    // step.2------------�˲�----------------------------//
    for (y = halfy, pRes = pResImg + y * width, pDel = pGryImg; y < height - halfy; y++) {
        // ��ֵ
        for (sum = 0, x = 0; x < M; x++) sum += sumCol[x];
        // �˲�
        pRes += halfx; // �������
        for (x = halfx; x < width - halfx; x++) {
            // ��ҶȾ�ֵ
            // *(pRes++)=sum/(N*M);
            *(pRes++) = (sum * c) >> 23; // �������˷�����λ�������
            // ����,���»ҶȺ�
            sum -= sumCol[x - halfx]; // �������
            sum += sumCol[x + halfx + 1]; // ���ұ���
        }
        pRes += halfx; // �����Ҳ�
        // ����,����sumCol
        for (x = 0; x < width; x++) {
            sumCol[x] -= *(pDel++); // ����һ��
            sumCol[x] += *(pAdd++); // ����һ��
        }
    }
    // step.3------------����----------------------------//
    return;
}

//�����л��ֵĻ���ͼʵ��
//pGryImg, // ԭʼ�Ҷ�ͼ��
//width,       // ͼ��Ŀ�� 
//height,      // ͼ��ĸ߶�
//pSumImg     // ����õ��Ļ���ͼ
void RmwDoSumGryImg(uint8_t* pGryImg,int width,int height, int* pSumImg)
{
    uint8_t* pGry;
    int* pRes;
    int x, y;
    int sumCol[4096]; // Լ��ͼ���Ȳ�����4096

    memset(sumCol, 0, sizeof(int) * width);
    for (y = 0, pGry = pGryImg, pRes = pSumImg; y < height; y++)
    {
        // ��������ص��ر���
        sumCol[0] += *(pGry++);
        *(pRes++) = sumCol[0];
        // ��������
        for (x = 1; x < width; x++)
        {
            sumCol[x] += *(pGry++);       // �����л���
            int temp = *(pRes - 1);
            *(pRes++) = temp + sumCol[x];
        }
    }
    return;
}

//����SSE�Ļ���ͼʵ��
//pGryImgԭʼ�Ҷ�ͼ��
//widthͼ��Ŀ�ȣ�������4�ı���
//heightͼ��ĸ߶�
//pSumImg����õ��Ļ���ͼ
void RmwDoSumGryImg_SSE(uint8_t* pGryImg, int width, int height, int* pSumImg)
{
    int sumCol[4096]; //Լ��ͼ���Ȳ�����4096
    __m128i* pSumSSE, A;
    uint8_t* pGry;
    int* pRes;
    int x, y;

    memset(sumCol, 0, sizeof(int) * width);
    for (y = 0, pGry = pGryImg, pRes = pSumImg; y < height; y++)
    {
        // 0:��Ҫ�ر���
        sumCol[0] += *(pGry++);
        *(pRes++) = sumCol[0];
        // 1
        sumCol[1] += *(pGry++);
        *(pRes++) = *(pRes - 1) + sumCol[1];
        // 2
        sumCol[2] += *(pGry++);
        *(pRes++) = *(pRes - 1) + sumCol[2];
        // 3
        sumCol[3] += *(pGry++);
        *(pRes++) = *(pRes - 1) + sumCol[3];
        // [4...width-1]
        for (x = 4, pSumSSE = (__m128i*)(sumCol + 4); x < width; x += 4, pGry += 4)
        {
            // �ѱ����ĵ�32λ(��4��8λ�������)ת����32λ������
            A = _mm_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)pGry));
            // 4��32λ���������
            *(pSumSSE++) = _mm_add_epi32(*pSumSSE, A);
            // ����
            *(pRes++) = *(pRes - 1) + sumCol[x + 0];
            *(pRes++) = *(pRes - 1) + sumCol[x + 1];
            *(pRes++) = *(pRes - 1) + sumCol[x + 2];
            *(pRes++) = *(pRes - 1) + sumCol[x + 3];
        }
    }
    return;
}

//���ڻ���ͼ�Ŀ��پ�ֵ�˲�
//pSumImg����õ��Ļ���ͼ
//width,height,ͼ��Ŀ�Ⱥ͸߶�
//M, N,�˲�����M��N��
//pResImg ���ͼ��
void RmwAvrFilterBySumImg(int* pSumImg,int width, int height,int M, int N,uint8_t* pResImg)
{
    // û�жԱ߽����������������ؽ��д������Բ��ñ䴰�ڵĲ���
    int* pY1, * pY2;
    uint8_t* pRes;
    int halfx, halfy;
    int y, x1, x2;
    int sum, c;

    // step.1------------��ʼ��--------------------------//
    M = M / 2 * 2 + 1; // ������
    N = N / 2 * 2 + 1; // ������
    halfx = M / 2;      // �˲����İ뾶x
    halfy = N / 2;      // �˲����İ뾶y
    c = (1 << 23) / (M * N); // �˷�����
    // step.2------------�˲�----------------------------//
    for (y = halfy + 1, pRes = pResImg + y * width, pY1 = pSumImg, pY2 = pSumImg + N * width;
        y < height - halfy;
        y++, pY1 += width, pY2 += width)
    {
        pRes += halfx + 1; // �������
        for (x1 = 0, x2 = M; x2 < width; x1++, x2++) // ���Լ���ˣ�����̫���׶�
        {
            sum = *(pY2 + x2) - *(pY2 + x1) - *(pY1 + x2) + *(pY1 + x1);
            *(pRes++) = (uint8_t)((sum * c) >> 23); // �������˷�����λ�������
        }
        pRes += halfx; // �����Ҳ�
    }
    // step.3------------����----------------------------//
    return;
}

void GetMedianGry(int* histogram, int N, int* medGry)
{
    int g;
    int num;

    // step.1-------------��Ҷ���ֵ------------------------//
    num = 0;
    for (g = 0; g < 256; g++)
    {
        num += histogram[g];
        if (2 * num > N) break;  //num>N/2
    }
    *medGry = g;
    // step.2-------------����------------------------------//
    return;
}

//��ֵ�˲�
//pGryImg��ָ�������Ҷ�ͼ�����ݵ�ָ�롣
//width��height����ʾͼ��Ŀ�Ⱥ͸߶ȡ�
//M��N���ֱ��ʾ��ֵ�˲�����ˮƽ�ʹ�ֱ�����С��������Ϊ��λ����
//pResImg��ָ��洢���ͼ�����ݵ�ָ�롣
double RmwMedianFilter(uint8_t* pGryImg, int width, int height, int M, int N, uint8_t* pResImg) 
{
    uint8_t* pCur, * pRes;
    int halfx, halfy, x, y, i, j, y1, y2;
    int histogram[256];
    int wSize, j1, j2;
    int num, med, v;
    int dbgCmpTimes = 0; // ������ֵ����Ƚϴ����ĵ���

    M = M / 2 * 2 + 1; // ������
    N = N / 2 * 2 + 1; // ������
    halfx = M / 2;      // x�뾶
    halfy = N / 2;      // y�뾶
    wSize = (halfx * 2 + 1) * (halfy * 2 + 1); // �����������ܸ���

    for (y = halfy, pRes = pResImg + y * width; y < height - halfy; y++) {
        // step.1----��ʼ��ֱ��ͼ
        y1 = y - halfy;
        y2 = y + halfy;
        memset(histogram, 0, sizeof(int) * 256);

        for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width) {
            for (j = 0; j < halfx * 2 + 1; j++) {
                histogram[*(pCur + j)]++;
            }
        }

        // step.2-----��ʼ����ֵ
        num = 0; // ��¼�ŻҶ�ֵ��0����ֵ�ĸ���
        for (i = 0; i < 256; i++) {
            num += histogram[i];
            if (num * 2 > wSize) {
                med = i;
                break;
            }
        }

        // �˲�
        pRes += halfx; // û�д���ͼ����߽�������
        for (x = halfx; x < width - halfx; x++) {
            // ��ֵ
            *(pRes++) = med;

            // step.3-----ֱ��ͼ����: ��ȥ��ǰ��������ߵ�һ��,��������Ҳ��һ������
            j1 = x - halfx;     // �������
            j2 = x + halfx + 1; // �ұߵ�����

            for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width) {
                // ��ȥ�������
                v = *(pCur + j1);
                histogram[v]--;  // ����ֱ��ͼ
                if (v <= med) num--; // ����num

                // ����ұߵ�����
                v = *(pCur + j2);
                histogram[v]++; // ����ֱ��ͼ
                if (v <= med) num++; // ����num
            }

            // step.4-----������ֵ
            if (num * 2 < wSize) { // ���ϴ���ֵmed�ĸ���������,��medҪ���
                for (med = med + 1; med < 256; med++) {
                    dbgCmpTimes += 2; // �ܵıȽϴ���,������
                    num += histogram[med];
                    if (num * 2 > wSize) break;
                }
                dbgCmpTimes += 1; // �ܵıȽϴ���,������
            }
            else { // ���ϴ���ֵmed�ĸ�������,��medҪ��С
                while ((num - histogram[med]) * 2 > wSize) { // ����ȥ��,�Ա�С
                    dbgCmpTimes++; // �ܵıȽϴ���,������
                    num -= histogram[med];
                    med--;
                }
                dbgCmpTimes += 2; // �ܵıȽϴ���,������
            }
        }
        pRes += halfx; // û�д���ͼ���ұ߽�������
    }
    // ����������ֵ��Ҫ��ƽ���Ƚϴ���
    return dbgCmpTimes * 1.0 / ((width - halfx * 2) * (height - halfy * 2));
}

// ��ֵ�˲�
// pGryImg��ָ�������Ҷ�ͼ�����ݵ�ָ�롣
// width��height����ʾͼ��Ŀ�Ⱥ͸߶ȡ�
// M��N���ֱ��ʾ��ֵ�˲�����ˮƽ�ʹ�ֱ�����С��������Ϊ��λ����
// pResImg��ָ��洢���ͼ�����ݵ�ָ�롣
void RmwMeanFilter(uint8_t* pGryImg, int width, int height, int M, int N, uint8_t* pResImg) {
    int borderX = M / 2;
    int borderY = N / 2;

    for (int y = borderY; y < height - borderY; ++y) {
        for (int x = borderX; x < width - borderX; ++x) {
            // �������������ص�ƽ��ֵ
            int sum = 0;
            for (int j = -borderY; j <= borderY; ++j) {
                for (int i = -borderX; i <= borderX; ++i) {
                    sum += pGryImg[(y + j) * width + (x + i)];
                }
            }
            int avg = sum / (M * N);

            // ��ƽ��ֵ�洢�ڽ��ͼ����
            pResImg[y * width + x] = avg;
        }
    }
}

//��ֵ�˲�
//pBinImg,  ԭʼ��ֵͼ��
// width, height,ͼ��Ŀ�Ⱥ͸߶�
// M, N, �˲�����M��N��
// threshold, �Ҷ���ֵ,���ڵ��ڸ�ֵʱ�����255
// pResImg ���ͼ��
void RmwBinImgFilter(uint8_t* pBinImg,int width, int height,int M, int N,double threshold,uint8_t* pResImg )
{
    // û�жԱ߽����������������ؽ��д������Բ��ñ䴰�ڵĲ���
    uint8_t* pAdd, * pDel, * pRes;
    int halfx, halfy;
    int x, y, sum, sumThreshold;
    int sumCol[4096]; //Լ��ͼ���Ȳ�����4096

    // step.1------------��ʼ��--------------------------//
    M = M / 2 * 2 + 1; //������
    N = N / 2 * 2 + 1; //������
    halfx = M / 2; //�˲�����x�뾶
    halfy = N / 2; //�˲�����y�뾶
    sumThreshold = max(1, (int)(threshold * M * N)); //ת���������ڻҶ�ֵ֮�͵���ֵ
    memset(sumCol, 0, sizeof(int) * width);
    for (y = 0, pAdd = pBinImg; y < N; y++)
    {
        for (x = 0; x < width; x++)
            sumCol[x] += *(pAdd++);
    }
    // step.2------------�˲�----------------------------//
    for (y = halfy, pRes = pResImg + y * width, pDel = pBinImg; y < height - halfy; y++)
    {
        //��ֵ
        for (sum = 0, x = 0; x < M; x++)
            sum += sumCol[x];
        //�˲�
        pRes += halfx; //�������
        for (x = halfx; x < width - halfx; x++)
        {
            //��ҶȾ�ֵ
            /*if (sum>=sumThreshold)
            {
                *(pRes++) = 255;
            }
            else  *(pRes++) = 0;*/
            *(pRes++) = (sum >= sumThreshold) * 255; //Ϊ��ʱ��255,����0
            //����,���»ҶȺ�
            sum -= sumCol[x - halfx];     //�������
            sum += sumCol[x + halfx + 1]; //���ұ���
        }
        pRes += halfx; //�����Ҳ�
        //����,����sumCol
        for (x = 0; x < width; x++)
        {
            sumCol[x] -= *(pDel++); //����һ��
            sumCol[x] += *(pAdd++); //����һ��
        }
    }
    // step.3------------����----------------------------//
    return;
}

//�ݶ����Ӽ���ֵ
//pGryImg������ĻҶ�ͼ������ָ�롣
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
//pGrdImg��������ݶ�ͼ������ָ��
void RmwGradientGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pGrdImg)
{
    uint8_t* pGry, * pGrd;
    int dx, dy;
    int x, y;

    for (y = 0, pGry = pGryImg, pGrd = pGrdImg; y < height - 1; y++)
    {
        for (x = 0; x < width - 1; x++, pGry++)
        {
            dx = *pGry - *(pGry + 1);
            dy = *pGry - *(pGry + width);
            int gradient = (int)(sqrt(dx * dx * 1.0 + dy * dy));
            *pGrd++ = (gradient > 255) ? 255 : gradient;
        }
        *pGrd++ = 0; //β�в���,��Եǿ�ȸ�0
        pGry++;
    }
    memset(pGrd, 0, width); //β�в���,��Եǿ�ȸ�0
}

//�ݶ����Ӽ���ֵ
//pGryImg������ĻҶ�ͼ������ָ�롣
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
//pGrdImg��������ݶ�ͼ������ָ��
void RmwGradientGryImgPlus(uint8_t* pGryImg, int width, int height, uint8_t* pGrdImg, int threshold)
{
    uint8_t* pGry, * pGrd;
    int dx, dy;
    int x, y;

    for (y = 0, pGry = pGryImg, pGrd = pGrdImg; y < height - 1; y++)
    {
        for (x = 0; x < width - 1; x++, pGry++)
        {
            dx = *pGry - *(pGry + 1);
            dy = *pGry - *(pGry + width);
            int gradient = (int)(sqrt(dx * dx * 1.0 + dy * dy));
            *(pGrd++) = (gradient > threshold) ? min(255, gradient) : 0;
        }
        *(pGrd++) = 0; //β�в���,��Եǿ�ȸ�0
        pGry++;
    }
    memset(pGrd, 0, width); //β�в���,��Եǿ�ȸ�0
}

//����
void invertImage(uint8_t* image, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        image[i] = 255 - image[i];
    }
}

//�޲�������
void RmwRobertsGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pRbtImg)
{
    uint8_t* pGry, * pRbt;
    int dx, dy;
    int x, y;

    for (y = 0, pGry = pGryImg, pRbt = pRbtImg; y < height - 1; y++)
    {
        for (x = 0; x < width - 1; x++, pGry++)
        {
            dx = *pGry - *(pGry + width + 1);
            dy = *(pGry + 1) - *(pGry + width);
            *pRbt++ = (uint8_t)(dx > dy ? dx : dy); // ʹ����Ŀ�����ѡ��ϴ��ֵ
        }
        *pRbt++ = 0; // β�в���, ��Եǿ�ȸ�0
        pGry++;
    }
    memset(pRbt, 0, width); // β�в���, ��Եǿ�ȸ�0
}

//����������
void RmwSobelGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pSbImg)
{
    uint8_t* pGry, * pSb;
    int dx, dy;
    int x, y;

    memset(pSbImg, 0, width); // ���в���, ��Եǿ�ȸ�0
    for (y = 1, pGry = pGryImg + width, pSb = pSbImg + width; y < height - 1; y++)
    {
        *pSb++ = 0; // ���в���, ��Եǿ�ȸ�0
        pGry++;
        for (x = 1; x < width - 1; x++, pGry++)
        {
            // ��dx
            dx = *(pGry - 1 - width) + (*(pGry - 1) * 2) + *(pGry - 1 + width);
            dx -= *(pGry + 1 - width) + (*(pGry + 1) * 2) + *(pGry + 1 + width);
            // ��dy
            dy = *(pGry - width - 1) + (*(pGry - width) * 2) + *(pGry - width + 1);
            dy -= *(pGry + width - 1) + (*(pGry + width) * 2) + *(pGry + width + 1);
            // ���
            *pSb++ = (uint8_t)min(255, abs(dx) + abs(dy));
        }
        *pSb++ = 0; // β�в���, ��Եǿ�ȸ�0
        pGry++;
    }
    memset(pSb, 0, width); // β�в���, ��Եǿ�ȸ�0
}

//Prewitt����
void RmwPrewittGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pPRTImg)
{
    uint8_t* pGry, * pPRT;
    int dx, dy, d45, d135, v1, v2;
    int x, y;

    memset(pPRTImg, 0, width); // ���в���, ��Եǿ�ȸ�0
    for (y = 1, pGry = pGryImg + width, pPRT = pPRTImg + width; y < height - 1; y++)
    {
        *pPRT++ = 0; // ���в���, ��Եǿ�ȸ�0
        pGry++;
        for (x = 1; x < width - 1; x++, pGry++)
        {
            // ��dx
            dx = *(pGry - 1 - width) + *(pGry - 1) + *(pGry - 1 + width);
            dx -= *(pGry + 1 - width) + *(pGry + 1) + *(pGry + 1 + width);
            // ��dy
            dy = *(pGry - width - 1) + *(pGry - width) + *(pGry - width + 1);
            dy -= *(pGry + width - 1) + *(pGry + width) + *(pGry + width + 1);
            // ��45��
            d45 = *(pGry - width - 1) + *(pGry - width) + *(pGry - 1);
            d45 -= *(pGry + width + 1) + *(pGry + width) + *(pGry + 1);
            // ��135��
            d135 = *(pGry - width) + *(pGry - width + 1) + *(pGry + 1);
            d135 -= *(pGry + width - 1) + *(pGry + width) + *(pGry - 1);
            // ���
            v1 = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
            v2 = abs(d45) > abs(d135) ? abs(d45) : abs(d135);
            *pPRT++ = (uint8_t)((v1 > v2) ? ((v1 > 255) ? 255 : v1) : ((v2 > 255) ? 255 : v2));
        }
        *pPRT++ = 0; // β�в���, ��Եǿ�ȸ�0
        pGry++;
    }
    memset(pPRT, 0, width); // β�в���, ��Եǿ�ȸ�0
}

//������
//pGryImg �� pTmpImg ��ָ�� uint8_t ���͵�ָ�룬���Ƿֱ�ָ��ԭʼ�Ҷ�ͼ�����ݺ͸���ͼ�����ݡ�
//width �� height �����Ͳ�������ʾͼ��Ŀ�Ⱥ͸߶ȡ�
//a0 ��˫���ȸ����Ͳ�������ʾ�˲�ϵ����
//pSJImg ��ָ�� uint8_t ���͵�ָ�룬��ָ���������ͼ�����ݡ�
void RmwShenJunGryImg(uint8_t* pGryImg,uint8_t* pTmpImg, int width, int height, double a0, uint8_t* pSJImg)
{
    uint8_t* pGry, * pCur, * pSJ, * pEnd;
    int LUT[512], * ALUT; // a0���ұ�
    int x, y, pre, dif;

    // Step 1: ��ʼ�����ұ�
    a0 = (a0 < 0.01) ? 0.01 : ((a0 > 0.99) ? 0.99 : a0); // ��ȫ�Լ��
    // a0���ұ�, ��������������
    ALUT = LUT + 256;
    for (ALUT[0] = 0, dif = 1; dif < 256; dif++)
    {
        ALUT[dif] = (int)(dif * a0 + 0.5);
        ALUT[-dif] = (int)(-dif * a0 - 0.5);
    }

    // Step 2: ����ʵ��ָ���˲�
    // �����˲�
    for (y = 0, pGry = pGryImg, pCur = pTmpImg; y < height; y++)
    {
        // ��������: p1(y,x) = p1(y,x-1) + a * [p(y,x) - p1(y,x-1)]
        *(pCur++) = pre = *(pGry++);
        for (x = 1; x < width; x++, pGry++)
            *(pCur++) = pre = pre + ALUT[*pGry - pre];
        pCur--; // �ص���β
        // ��������: p2(y,x) = p2(y,x+1) - a * [p1(y,x) - p2(y,x+1)]
        for (x = width - 2, pCur = pCur - 1; x >= 0; x--)
            *(pCur--) = pre = pre + ALUT[*pCur - pre];
        pCur += (width + 1); // �ص���һ�еĿ�ʼ
    }
    // �����˲�
    for (x = 0, pCur = pTmpImg; x < width; x++, pCur = pTmpImg + x)
    {
        // ��������: p3(y,x) = p3(y-1,x) + a * [p2(y,x) - p3(y-1,x)]
        pre = *pCur;
        for (y = 1, pCur += width; y < height; y++, pCur += width)
            *pCur = pre = pre + ALUT[*pCur - pre];
        pCur -= width; // �ص���β
        // ��������: p4(i,j) = p4(i+1,j) + a * [p3(i,j) - p4(i+1,j)]
        for (y = height - 2, pCur -= width; y >= 0; y--, pCur -= width)
            *pCur = pre = pre + ALUT[*pCur - pre];
    }

    // Step 3: ������=1��������Ϊ0��0����Ҳ��0
    pEnd = pTmpImg + width * height;
    for (pCur = pTmpImg, pGry = pGryImg; pCur < pEnd; pGry++)
    {
        *(pCur++) = (*pCur > *pGry);
    }

    // Step 4: �������
    memset(pSJImg, 0, width * height); // ��Եǿ�ȸ�0
    pSJ = pSJImg + width;
    pCur = pTmpImg + width; // ���в��� 
    for (y = 1; y < height - 1; y++)
    {
        pSJ++; pCur++;  // ���в���
        for (x = 1; x < width - 1; x++, pGry++, pCur++, pSJ++)
        {
            if (*pCur) // ������
            {
                // ����ʹ��4����, ��ԵΪ8��ͨ, ���ܱ�֤4��ͨ; ʹ��8������ܱ�֤��Ե4��ͨ
                if ((!*(pCur - 1)) || // ��, ����<=0, ����<0
                    (!*(pCur + 1)) || // ��, ����<=0, ����<0
                    (!*(pCur - width)) || // ��, ����<=0, ����<0
                    (!*(pCur + width)))   // ��, ����<=0, ����<0
                {
                    *pSJ = 255; // ��Χ�е���С�ڵ���0
                }
            }
        }
        pSJ++; pCur++;  // β�в���
    }
}

//�����Ӽ�����������
//pGryImg��ָ��ԭʼ�Ҷ�ͼ�����ݵ�ָ��
//pTmpImg��ָ����ͼ�����ݵ�ָ��
//width��ͼ��Ŀ��
//height��ͼ��ĸ߶�
//a0�����������ӵĲ��������ڿ��Ʊ�Ե���������ȡ�
//grdThre������Sobel���ӵ��ݶ���ֵ
//pEdgeImg�����ձ�Եͼ�����ݵ�ָ��
void RmwExtractRiceEdge(uint8_t* pGryImg,uint8_t* pTmpImg,int width,int height,double a0, int grdThre, uint8_t* pEdgeImg)
{
    // step.1------------������-----------------------//
    RmwShenJunGryImg(pGryImg, pTmpImg, width, height, a0, pEdgeImg);
    // step.2------------Sobel����----------------------//
    RmwSobelGryImg(pGryImg, width, height, pTmpImg);
    // step.3------------�����ں�-----------------------//
    for (int i = 0; i < width * height; i++)
    {
        *(pEdgeImg + i) = (pEdgeImg[i] && (pTmpImg[i] > grdThre)) * 255;
    }
    // step.4------------����---------------------------//
    return;
}

////��ʾֱ��ͼ
////histogram��ֱ��ͼ����ָ��
////size��ֱ��ͼ���ݴ�С
////a��b��c��ֱ��ͼ����������,���ȣ��Աȶȣ���ֵ
//void displayHistogram(const int* histogram, int size, double a, double b, double c)
//{
//
//    SDL_Init(SDL_INIT_VIDEO);
//
//    char windowTitle[100];
//    sprintf(windowTitle, "Histogram      Brightness: %.2f  Contrast: %.2f  Median: %.2f", a, b, c);
//    SDL_Window* window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, SDL_WINDOW_SHOWN);
//    //��������Ϊֱ��ͼ�����������������Ǵ�������Ļ�ϵ�λ�ã��ֱ���ˮƽ�ʹ�ֱ��������ʾ�����ڵĿ�Ⱥ͸߶�Ŀǰ��ʱ512��512����������ٵ������ٺ���һ����������˼�Ǵ�������������ʾ
//
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    if (renderer == NULL) {
//        printf("Failed to create renderer: %s\n", SDL_GetError());
//        return;
//    }
//
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//    SDL_RenderClear(renderer);
//
//    int maxBarHeight = 0;
//    int minBarHeight = 0;
//
//    for (int i = 0; i < size; i++)
//    {
//        if (histogram[i] > maxBarHeight)
//        {
//            maxBarHeight = histogram[i];
//        }
//    }
//
//    for (int i = 0; i < size; i++)
//    {
//        if (histogram[i] < minBarHeight)
//        {
//            minBarHeight = histogram[i];
//        }
//    }
//
//
//    int y = 250;
//
//
//    int barWidth = 2;
//    double k = (double)(200) / maxBarHeight;
//
//
//    for (int i = 0; i < size; i++) {
//
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//
//        int barX = i * barWidth + 1;
//        int barY = y - (int)(histogram[i] * k);
//
//        SDL_Rect bar = { barX, barY, barWidth, (int)(histogram[i] * k) };
//
//        SDL_RenderFillRect(renderer, &bar);
//    }
//
//    SDL_RenderPresent(renderer);
//
//    SDL_Event event;
//    int quit = 0;
//    while (!quit) {
//        while (SDL_PollEvent(&event)) {
//            if (event.type == SDL_QUIT) {
//                quit = 1;
//            }
//        }
//    }
//
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//
//    SDL_Quit();
//}

//����ֱ��ͼ�м�ֵ
//histogram��ֱ��ͼ����ָ��
//width��height��ͼ��Ŀ�Ⱥ͸߶�
double Median(const int* histogram, int width, int height)
{
    int size = width * height;
    int cumulativeSum = 0;
    int middleElement = (size + 1) / 2;

    for (int i = 0; i < 256; ++i)
    {
        cumulativeSum += histogram[i];
        if (cumulativeSum >= middleElement)
        {
            return i;
        }
    }

    return 0;
}


// ��ӡ����Ԫ��
void printList(struct Node* head) {
    struct Node* temp = head;  // ��ͷ�ڵ㿪ʼ��������
    while (temp != NULL) {
        printf("%d ", temp->data);  // �����ǰ�ڵ������
        temp = temp->next;           // �ƶ�����һ���ڵ�
    }
    printf("\n");
}

void insertAtEnd(struct Node** head, int value) 
{
    // ����һ���½ڵ�
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;  // �����½ڵ������
    newNode->next = NULL;   // ���½ڵ��nextָ����ΪNULL

    // �������Ϊ�գ����½ڵ㼴Ϊͷ�ڵ�
    if (*head == NULL) {
        *head = newNode;
    }
    else {
        // �ҵ�����β���ڵ�
        struct Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        // ���½ڵ����ӵ�β���ڵ����
        temp->next = newNode;
    }
}

int generateChainCode(uint8_t* image, int width, int height, int x, int y, int flag, struct Node** head, struct Node** tail) 
{
    printf("%d ", flag);
    int flag1 = 0;
    int x1 = x;
    int y1 = y;
    if (flag == 0)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y  * width / 8 + (x+1) / 8] >> (7 - ((x+1) % 8)) & 1 ) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if((image[(y + 1) * width / 8 + (x+1) / 8] >> (7 - ((x+1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x-1) / 8] >> (7 - ((x-1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        } 
        else
            return 0;
    }
    if (flag == 2)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 4)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 6)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 1)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 3)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 5)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 7)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
}

//��������
//image��ͼ������ָ��
//width��height��ͼ��Ŀ�Ⱥ͸߶�
// �޸ĺ���������ɺ���
struct Node* ChainCode(uint8_t* image, int width, int height)
{
    int size = width * height;
    int index = 0;
    int flag = -1;

    int firstx = -1;
    int firsty = -1;

    printf("ͼ���С: %d x %d\n", width, height);

    struct Node* head = NULL;
    printf("\n");

    int found = 0; // ��־���������ڿ������ѭ��

    for (int i = 0; i < height && !found; ++i) {
        for (int j = 0; j < width; ++j) {
            // ��ȡÿ�����ص�ֵ
            uint8_t pixel = (image[i * width / 8 + j / 8] >> (7 - (j % 8))) & 1;
            //[i * width / 8 + j / 8]�ҵ���Ӧ���ֽ�
           // >> (7 - (j % 8)) �ҵ�bit���ֽ��е�λ��
           // & 1 ����λ����0��������ǰֵ
           // printf("%d ", pixel);
            if (pixel == 0) {
                if (flag == -1) {
                    flag = 0;
                    firstx = j;
                    firsty = i;
                    // ��������
                    uint32_t img = (width * height + 7) / 8;  // ����ͼ������������ֽ���
                    uint8_t* imgcopy = (uint8_t*)malloc(img);
                    memcpy(imgcopy, image, img);
                    generateChainCode(imgcopy, width, height, j, i,0, &head, &head);
                    printf("��һ��Ŀ�����ص�Ϊ��(%d, %d)", firstx, firsty);
                    printList(head);
                    found = 1; // ���ñ�־������������ֹ���ѭ��
                }
                break; // �����ڲ�ѭ��
            }
        }
        //printf("\n");
    }

    return head;
}


//��ȡ1λ�Ҷ�ͼƬ
//filename���ַ������ָ�룬����ָ��Ҫ�����ͼ���ļ������ƻ�·����
//imageData���޷��� 8 λ�������ݵ�ָ�룬����Ҫ�����ͼ�����ݡ�
//width��ͼ��Ŀ�ȡ�
//height��ͼ��ĸ߶ȡ�
uint8_t* read1BitBMP(const char* filename, int* width, int* height)
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
    uint32_t imageDataSize = (*width * *height + 7) / 8;  // ����ͼ������������ֽ���
    uint8_t* imageData = (uint8_t*)malloc(imageDataSize);
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
    fseek(file, *(int*)&bmpHeader[10], SEEK_SET);  // ʹ����ȷ��ƫ����
    fread(imageData, 1, imageDataSize, file);  // ��ȡ��ȷ��С��ͼ������

    fclose(file);

    return imageData;
}
