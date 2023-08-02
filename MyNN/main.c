#include <stdio.h>
#include "alg_mlp_networks.h"
// float data[21];
int label[4000];

int main()
{
    uint16_t size = 20;
    LayerData input_data;
    int res, num = 0, cnt = 0;

    FILE *fp = fopen("label.csv", "r");
    char str[201];
    while (fgets(str, 200, fp) != NULL) {
        label[num] = atoi(str);
        num++;
    }
    fclose(fp);

    networks_init();

    FILE *fp2 = fopen("feature.csv", "r");
    num       = 0;
    while (fgets(str, 200, fp2) != NULL) {
        float data[20];
        int num_read =
            sscanf(str,
                   "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%"
                   "f",
                   &data[0], &data[1], &data[2], &data[3], &data[4], &data[5],
                   &data[6], &data[7], &data[8], &data[9], &data[10],
                   &data[11], &data[12], &data[13], &data[14], &data[15],
                   &data[16], &data[17], &data[18], &data[19]);
        input_data.data = data;
        input_data.size = size;
        // for (int i = 0; i < size; i++) {
        //     printf("%f ", input_data.data[i]);
        // }
        // printf("\n %d \n",size);

        foward_process(&input_data, &res);

        // printf("pred:%d, label:%d\n", res, label[num]);
        if (res == label[num])
            cnt++;

        num++;
    }
    printf("accuracy:%f", (float)cnt / num);
    fclose(fp2);

    /*** strtok_s函数读取csv文件存在问题，会丢失数据 ***/

    // FILE *fp = fopen("label.csv", "r");
    // char str[101];
    // while (fgets(str, 100, fp) != NULL) {
    //     label[num] = atoi(str);
    //     num++;
    // }
    // fclose(fp);

    // networks_init();

    // FILE *fp2 = fopen("feature.csv", "r");
    // num       = 0;
    // while (fgets(str, 100, fp2) != NULL) {
    //     char *save_ptr;
    //     data[0] = atof(strtok_s(str, ",", &save_ptr));
    //     printf("%f ",data[0]);
    //     for (int i = 1; i < 20; i++) {
    //         data[i] = atof(strtok_s(NULL, ",", &save_ptr));
    //         printf("%f ",data[i]);
    //     }
    //     // printf("%f,%f,", data[0], data[1]);
    //     input_data.data = data;
    //     input_data.size = size;

    //     foward_process(&input_data, &res);

    //     printf("pred:%d, label:%d\n", res, label[num]);
    //     if (res == label[num])
    //         cnt++;
    //     num++;
    // }
    // printf("accuracy:%f", (float)cnt / num);
    // fclose(fp2);

    /*** 单个测试 ***/

    // networks_init();

    // float input[30] = { 155023.3425, 174359.7802, 1897872.179, 404364.7055,
    //                     563529.0166, 254092.2954, 858075.2719, 1.0,
    //                     1.0,         1.0,         1151436.627, 6479623.688,
    //                     1751098.447, 1726270.579, 3319715.195, 3177914.259,
    //                     1662349.201, 2.0,         1.0,         1.0 };
    // input_data.data = input;
    // input_data.size = 20;
    // foward_process(&input_data, &res);
    // printf("pred:%d\n", res);
    return 0;
}
