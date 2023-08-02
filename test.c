#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 1000
#define MAX_NUMBERS_PER_LINE 100

int main() {
    FILE *file = fopen("../IMU_Dataset/walk/IMU-2023-05-11-16_43_43_walk.txt", "r"); // 打开文本文件，文件名为data.txt

    if (file == NULL) {
        printf("无法打开文件\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) { // 逐行读取文件内容
        int numbers[MAX_NUMBERS_PER_LINE];
        int num_count = 0;

        char *token = strtok(line, ","); // 使用逗号分割每个整数
        while (token != NULL && num_count < MAX_NUMBERS_PER_LINE) {
            int number = atoi(token); // 将字符串转换为整数
            numbers[num_count++] = number;
            token = strtok(NULL, ",");
        }

        // 打印提取出的整数
        for (int i = 0; i < num_count; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\n");
    }

    fclose(file); // 关闭文件

    return 0;
}