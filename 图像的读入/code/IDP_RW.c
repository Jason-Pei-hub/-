#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


uint8_t* readGrayScaleBMP(const char* filename, int* width, int* height) {
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
void saveGrayScaleBMP(const char* filename, const uint8_t* imageData, int width, int height) {
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
uint8_t* readColorBMP(const char* filename, int* width, int* height) {
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

void saveColorBMP(const char* filename, const uint8_t* imageData, int width, int height) {
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


int main() {
    const char* grayScaleFilename = "gray_scale.bmp";
    const char* colorFilename = "FXT12111.bmp";

    int grayScaleWidth, grayScaleHeight;
    int width, height;

    // ��ȡ8λ�Ҷ�ͼ��
    uint8_t* grayScaleImageData = readGrayScaleBMP(grayScaleFilename, &grayScaleWidth, &grayScaleHeight);
    if (!grayScaleImageData) {
        fprintf(stderr, "Failed to read gray scale BMP image\n");
        return 1;
    }

    // ��ȡ24λ��ɫͼ��
    uint8_t* colorImageData = readColorBMP(colorFilename, &width, &height);
    if (!colorImageData) {
        fprintf(stderr, "Failed to read color BMP image\n");
        return 1;
    }

    // ִ�жԻҶ�ͼ��Ĳ���
    // ����8λ�Ҷ�ͼ������ΪBMP�ļ�
    saveGrayScaleBMP("out.bmp", grayScaleImageData, grayScaleWidth, grayScaleHeight);

    // ����24λ�Ҷ�ͼ������ΪBMP�ļ�
    saveColorBMP("outcolor.bmp", colorImageData, width, height);
    

    free(grayScaleImageData);
    free(colorImageData);


    return 0;
}