#include <stdio.h>
#include <stdlib.h>

// �������뷽��
enum Direction {
    N = 0, NE, E, SE, S, SW, W, NW
};

// ��ȡBMPͼ��
void readBMP(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("�޷����ļ���%s\n", filename);
        exit(1);
    }

    // ��ȡBMP�ļ�ͷ
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    // ��ȡͼ���Ⱥ͸߶�
    *width = *(int*)&header[18];
    *height = *(int*)&header[22];

    fclose(file);
}

// ��ȡ����ֵ
int getPixelValue(const char* filename, int x, int y) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("�޷����ļ���%s\n", filename);
        exit(1);
    }

    // ����BMP�ļ�ͷ
    fseek(file, 54, SEEK_SET);

    // ��λ��ָ������λ��
    fseek(file, (x + y * 512) * sizeof(unsigned char), SEEK_CUR);

    // ��ȡ����ֵ
    unsigned char pixel_value;
    fread(&pixel_value, sizeof(unsigned char), 1, file);

    fclose(file);

    return pixel_value;
}

// �������
void printChainCode(const char* chain_code_filename) {
    FILE* chain_code_file = fopen(chain_code_filename, "r");
    if (chain_code_file == NULL) {
        printf("�޷����ļ���%s\n", chain_code_filename);
        exit(1);
    }

    int code;
    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        printf("%d ", code);
    }

    fclose(chain_code_file);
}

// ����ײ�ѭ������
void printFirstDiffCycleCode(const char* chain_code_filename) {
    FILE* chain_code_file = fopen(chain_code_filename, "r");
    if (chain_code_file == NULL) {
        printf("�޷����ļ���%s\n", chain_code_filename);
        exit(1);
    }

    int code;
    int prev_code;
    fscanf_s(chain_code_file, "%d", &prev_code);  // ��ȡ��һ������
    printf("%d ", prev_code);

    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        int diff = (code - prev_code + 8) % 8;
        printf("%d ", diff);
        prev_code = code;
    }

    fclose(chain_code_file);
}

// �����С�ײ�ѭ������
void printMinFirstDiffCycleCode(const char* chain_code_filename) {
    FILE* chain_code_file = fopen(chain_code_filename, "r");
    if (chain_code_file == NULL) {
        printf("�޷����ļ���%s\n", chain_code_filename);
        exit(1);
    }

    int code;
    int prev_code;
    fscanf_s(chain_code_file, "%d", &prev_code);  // ��ȡ��һ������

    // Ѱ����С�ײ�
    int min_diff = 8;
    int min_diff_code = prev_code;

    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        int diff = (code - prev_code + 8) % 8;
        if (diff < min_diff) {
            min_diff = diff;
            min_diff_code = code;
        }
        prev_code = code;
    }

    fclose(chain_code_file);

    // �����С�ײ�ѭ������
    chain_code_file = fopen_s(&chain_code_file, chain_code_filename, "r");
    if (chain_code_file == NULL) {
        printf("�޷����ļ���%s\n", chain_code_filename);
        exit(1);
    }

    // ��λ����С�ײ��λ��
    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        if (code == min_diff_code) {
            printf("%d ", code);
            break;
        }
    }

    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        int diff = (code - prev_code + 8) % 8;
        printf("%d ", diff);
        prev_code = code;
    }

    fclose(chain_code_file);
}

int main() {
    const char* chain_code_filename = "chaincode.txt";

    printf("����: ");
    printChainCode(chain_code_filename);
    printf("\n");

    printf("�ײ�ѭ������: ");
    printFirstDiffCycleCode(chain_code_filename);
    printf("\n");

    printf("��С�ײ�ѭ������: ");
    printMinFirstDiffCycleCode(chain_code_filename);
    printf("\n");

    return 0;
}