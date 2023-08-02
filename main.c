#include "alg_step_counter.h"
#include "MyNN/alg_mlp_network.h"
#define MAX_ACC_LEN (10000)
#define MAX_LINE_LENGTH (100)
#define MAX_INTEGER_LENGTH (8)
static int16_t acc_x[MAX_ACC_LEN], acc_y[MAX_ACC_LEN], acc_z[MAX_ACC_LEN];
static int16_t gyro_x[MAX_ACC_LEN], gyro_y[MAX_ACC_LEN], gyro_z[MAX_ACC_LEN];

static int read_data(const char *file_name, AccInput *saved_data)
{
    int16_t new_num;
    uint16_t cnt;
    FILE *fd;
    int ret;
    char line[MAX_LINE_LENGTH];
    if (!file_name || !saved_data) {
        return ALGO_ERR_GENERIC;
    }

    saved_data->len = 0;

    // open data file
    if ((fd = fopen(file_name, "r")) == NULL) {
        printf("Fail to open the file in read_data()\n");
        return ALGO_ERR_GENERIC;
    }
    // read data file
    cnt = 0;
    while ((fgets(line, MAX_LINE_LENGTH, fd)) != NULL) {
        int numbers[MAX_INTEGER_LENGTH];
        int num_count = 0;
        char *token   = strtok(line, ",");
        while (token && num_count < MAX_INTEGER_LENGTH) {
            int number           = atoi(token);
            numbers[num_count++] = number;
            token                = strtok(NULL, ",");
        }
        saved_data->gyro_x[cnt] = numbers[0];
        saved_data->gyro_y[cnt] = numbers[1];
        saved_data->gyro_z[cnt] = numbers[2];
        saved_data->x[cnt]      = numbers[3];
        saved_data->y[cnt]      = numbers[4];
        saved_data->z[cnt]      = numbers[5];
        cnt++;
    }

    saved_data->len = cnt;

    // close data file
    ret = fclose(fd);
    if (ret != 0) {
        printf("Fail to close the file in read_data()\n");
        return ALGO_ERR_GENERIC;
    }
    return ALGO_NORMAL;
}

int main(void)
{
    uint16_t step_num = 0, step_total = 0, i;
    AlgoError ret        = ALGO_NORMAL;
    uint16_t delay_point = ACC_FS - 5 - 2;
    const char *fn       = "D:\\Projects\\Versilicon_Embedded_Contest_2023\\C代码待验证\\IMU_Dataset\\jumping_"
                           "squat\\IMU-2023-06-01-09_48_38_jumping_squat.txt";
    AccInput acc_xyz     = {
        0, gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z
    }; // store x,y,z three vector and the length
    AccInput acc_xyz_win = {
        0, NULL, NULL, NULL, NULL, NULL, NULL
    }; // store one second data for process (win is window!)

    ret = read_data(fn, &acc_xyz);

    if (ret != ALGO_NORMAL) {
        return 0;
    }
    ret = step_counter_init();
    ret = networks_init();
    if (ret != ALGO_NORMAL) {
        return 0;
    }
    acc_xyz_win.gyro_x = acc_xyz.gyro_x;
    acc_xyz_win.gyro_y = acc_xyz.gyro_y;
    acc_xyz_win.gyro_z = acc_xyz.gyro_z;
    acc_xyz_win.x      = acc_xyz.x;
    acc_xyz_win.y      = acc_xyz.y;
    acc_xyz_win.z      = acc_xyz.z;
    acc_xyz_win.len    = delay_point;
    ret = step_counter_process(&acc_xyz_win, &step_num); // everytime callthis
                                                         // func, will get1s
                                                         // result( data
                                                         // lenth=25 )

    for (i = delay_point; i <= acc_xyz.len - ACC_FS; i += ACC_FS) {
        acc_xyz_win.x      = acc_xyz.x + i;
        acc_xyz_win.y      = acc_xyz.y + i;
        acc_xyz_win.z      = acc_xyz.z + i;
        acc_xyz_win.gyro_x = acc_xyz.gyro_x + i;
        acc_xyz_win.gyro_y = acc_xyz.gyro_y + i;
        acc_xyz_win.gyro_z = acc_xyz.gyro_z + i;
        acc_xyz_win.len    = ACC_FS;
        ret = step_counter_process(&acc_xyz_win, &step_num); // everytime call
                                                             // this func, will
        // get 1sresult( datalenth = 25)
        if (ret != ALGO_NORMAL) {
            printf("Error in step_counter_process()!\n");
            return 0;
        }
        step_total += step_num;
        if ((i - delay_point) % (ACC_FS * 5) == 0) {
            printf("Time = %3d(s)\tTotal steps = %3d\n", i / ACC_FS,
                   step_total);
        }
    }

    // foward_process(&ld, &class);
    // printf("%d  ", class);

    return 0;
}
