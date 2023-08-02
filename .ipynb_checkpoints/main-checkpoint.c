#include "alg_step_counter.h"

#define MAX_ACC_LEN (10000)
static int16_t acc_x[MAX_ACC_LEN], acc_y[MAX_ACC_LEN], acc_z[MAX_ACC_LEN];

static int read_data(const char *file_name, AccInput *saved_data)
{
    int16_t new_num;
    uint16_t cnt;
    FILE *fd;
    int ret;
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
    while ((ret = fscanf(fd, "%hd", &new_num)) != EOF &&
        cnt < MAX_ACC_LEN * 3) {
        if (cnt % 3 == 0) {
            saved_data->x[cnt / 3] = new_num;
        } else if (cnt % 3 == 1) {
            saved_data->y[cnt / 3] = new_num;
        } else if (cnt % 3 == 2) {
            saved_data->z[cnt / 3] = new_num;
        }
        cnt++;
    }
    if (cnt % 3 != 0) {
        return ALGO_ERR_GENERIC;
    }
    saved_data->len = cnt / 3;

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
    AlgoError ret = ALGO_NORMAL;
    // const char *fn = "./id01_acc_walk_01.txt";
    // const char *fn = "./id01_acc_walk_02.txt";
    // const char *fn = "./id02_acc_walk_01.txt";
    // const char *fn = "../acc_data/id02_acc_walk_02.txt";
    // const char *fn = "../acc_data/sit/IMU-2023-04-26-17_38_42_sit.txt";
    // const char *fn = "../acc_data/trot/"
    //                  "IMU-2023-04-26-17_36_43_trot_186step.txt";
    // const char *fn = "../acc_data/walk/"
    //                  "IMU-2023-04-26-17_30_45_walk_100step.txt";
    // const char *fn = "../acc_data/sit/IMU-2023-04-26-17_47_20_sit.txt";
    // const char *fn = "../acc_data/trot/"
    //                  "IMU-2023-04-26-17_45_10_trot_240step.txt";
    const char *fn       = "../acc_data/walk/"
                           "IMU-2023-04-26-17_42_58_walk_150step.txt";
    AccInput acc_xyz     = { 0, acc_x, acc_y, acc_z };    // store x,y,z three vector and the length
    AccInput acc_xyz_win = { 0, NULL, NULL, NULL };       // store one second data for process (win is window!) 

    ret = read_data(fn, &acc_xyz);   
    if (ret != ALGO_NORMAL) {
        return 0;
    }

    ret = step_counter_init();
    if (ret != ALGO_NORMAL) {
        return 0;
    }

    for (i = 0; i <= acc_xyz.len - ACC_FS; i += ACC_FS) {
        acc_xyz_win.x   = acc_xyz.x + i;
        acc_xyz_win.y   = acc_xyz.y + i;
        acc_xyz_win.z   = acc_xyz.z + i;
        acc_xyz_win.len = ACC_FS;
        ret             = step_counter_process(&acc_xyz_win, &step_num);   // everytime call this func, will get 1s result( data lenth=25 )
        if (ret != ALGO_NORMAL) {
            printf("Error in step_counter_process()!\n");
            return 0;
        }
        step_total += step_num;
        printf("Time = %3d(s)\tTotal steps = %3d\n", i / ACC_FS, step_total);
    }

    return 0;
}
