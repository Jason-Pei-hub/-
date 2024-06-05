#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_WIDTH  512  // ͼ����
#define IMAGE_HEIGHT 512  // ͼ��߶�

// �������뷽��
enum Direction {
    N = 0, NE, E, SE, S, SW, W, NW
};

// ��������ṹ��
typedef struct {
    int x;
    int y;
    int direction;
} ChainCode;

// ��ȡBMPͼ��
void readBMP(const char* filename, int image[IMAGE_HEIGHT][IMAGE_WIDTH]) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("�޷����ļ���%s\n", filename);
        exit(1);
    }

    // ��ȡBMP�ļ�ͷ
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    // ��ȡͼ���Ⱥ͸߶�
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    if (width != IMAGE_WIDTH || height != IMAGE_HEIGHT) {
        printf("ͼ��ߴ粻��ȷ��%dx%d\n", width, height);
        exit(1);
    }

    // ��ȡͼ����������
    unsigned char data[IMAGE_HEIGHT][IMAGE_WIDTH];
    fread(data, sizeof(unsigned char), IMAGE_WIDTH * IMAGE_HEIGHT, file);

    // ����������ת��Ϊ�Ҷ�ͼ��
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            image[y][x] = data[y][x];
        }
    }

    fclose(file);
}

// ��ȡ����ֵ
int getPixelValue(int image[IMAGE_HEIGHT][IMAGE_WIDTH], int x, int y) {
    // ��������Ƿ�Խ��
    if (x < 0 || x >= IMAGE_WIDTH || y < 0 || y >= IMAGE_HEIGHT) {
        return 0;  // ����Ĭ��ֵ
    }
    return image[y][x];
}

// ����׷��
void traceContour(int image[IMAGE_HEIGHT][IMAGE_WIDTH], int start_x, int start_y, ChainCode chainCode[], int* chainCodeLength) {
    // ��ʼ�����볤��
    *chainCodeLength = 0;

    // ��ʼ����ǰλ��
    int curr_x = start_x;
    int curr_y = start_y;

    // ��ʼ����ǰ����
    int curr_direction = NE;

    // ��������׷��
    do {
        // ��¼��ǰλ�úͷ���
        chainCode[*chainCodeLength].x = curr_x;
        chainCode[*chainCodeLength].y = curr_y;
        chainCode[*chainCodeLength].direction = curr_direction;

        // �������볤��
        (*chainCodeLength)++;

        // ���ݵ�ǰ����ѡ����һ�����ص�
        switch (curr_direction) {
        case N:
            curr_y--;
            break;
        case NE:
            curr_x++;
            curr_y--;
            break;
        case E:
            curr_x++;
            break;
        case SE:
            curr_x++;
            curr_y++;
            break;
        case S:
            curr_y++;
            break;
        case SW:
            curr_x--;
            curr_y++;
            break;
        case W:
            curr_x--;
            break;
        case NW:
            curr_x--;
            curr_y--;
            break;
        }

        // ���ݰ˷�������ģ��ѡ����һ������
        int next_direction = (curr_direction + 1) % 8;
        int min_pixel_value = getPixelValue(image, curr_x, curr_y);
        for (int i = 0; i < 8; i++) {
            int x = curr_x;
            int y = curr_y;
            int pixel_value = getPixelValue(image, x, y);

            // Ѱ����С����ֵ�ķ���
            if (pixel_value < min_pixel_value) {
                min_pixel_value = pixel_value;
                next_direction = (curr_direction + i) % 8;
            }

            // ������һ�����ص������
            switch (next_direction) {
            case N:
                y--;
                break;
            case NE:
                x++;
                y--;
                break;
            case E:
                x++;
                break;
            case SE:
                x++;
                y++;
                break;
            case S:
                y++;
                break;
            case SW:
                x--;
                y++;
                break;
            case W:
                x--;
                break;
            case NW:
                x--;
                y--;
                break;
            }

            // ���µ�ǰ����
            curr_direction = next_direction;
            curr_x = x;
            curr_y = y;
        }
    } while (curr_x != start_x || curr_y != start_y);  // ֱ���ص���ʼ��Ϊֹ
}

// �������
void printChainCode(ChainCode chainCode[], int chainCodeLength) {
    printf("Chain Code: ");
    for (int i = 0; i < chainCodeLength; i++) {
        printf("%d ", chainCode[i].direction);
    }
    printf("\n");
}

// ����������ײ�ѭ������
void computeCircularCode(ChainCode chainCode[], int chainCodeLength) {
    printf("Circular Code: ");
    for (int i = 0; i < chainCodeLength - 1; i++) {
        int circularCode = (chainCode[i + 1].direction - chainCode[i].direction + 8) % 8;
        printf("%d ", circularCode);
    }
    printf("%d\n", (chainCode[0].direction - chainCode[chainCodeLength - 1].direction + 8) % 8);
}

// �����������С�ײ�ѭ������
void computeMinimalCircularCode(ChainCode chainCode[], int chainCodeLength) {
    int minCircularCode = chainCode[0].direction;
    int minIndex = 0;

    // �ҵ���С���ײ�ѭ������
    for (int i = 1; i < chainCodeLength - 1; i++) {
        int circularCode = (chainCode[i + 1].direction - chainCode[i].direction + 8) % 8;
        if (circularCode < minCircularCode) {
            minCircularCode = circularCode;
            minIndex = i;
        }
    }

    // �����С���ײ�ѭ������
    printf("Minimal Circular Code: ");
    for (int i = minIndex; i < chainCodeLength - 1; i++) {
        int circularCode = (chainCode[i + 1].direction - chainCode[i].direction + 8) % 8;
        printf("%d ", circularCode);
    }
    for (int i = 0; i < minIndex; i++) {
        int circularCode = (chainCode[i + 1].direction - chainCode[i].direction + 8) % 8;
        printf("%d ", circularCode);
    }
    printf("\n");
}

int main() 
{
    int image[IMAGE_HEIGHT][IMAGE_WIDTH];  // �洢ͼ������ֵ������
    const char* filename = "input/�������ʵ��ͼ��.bmp";    // ͼ���ļ���

    // ��ȡBMPͼ��
    readBMP(filename, image);

    // ����׷��
    int start_x = 0;  // ������ʼ�������
    int start_y = 0;
    ChainCode chainCode[IMAGE_WIDTH * IMAGE_HEIGHT];
    int chainCodeLength;
    traceContour(image, start_x, start_y, chainCode, &chainCodeLength);

    // �������
    printChainCode(chainCode, chainCodeLength);

    // ����������ײ�ѭ������
    computeCircularCode(chainCode, chainCodeLength);

    // �����������С�ײ�ѭ������
    computeMinimalCircularCode(chainCode, chainCodeLength);

    return 0;
}