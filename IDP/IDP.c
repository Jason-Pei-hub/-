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
void RmwDoSumGryImg_SSE(uint8_t* pGryImg,int width,int height,int* pSumImg)
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
            *(pRes++) = (sum >= sumThreshold) * 255; //�����������ʽ�ĺ���
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