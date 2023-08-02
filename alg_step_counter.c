/**
 * Copyright (C) 2023 VeriSilicon Holdings Co., Ltd.
 *
 * @file alg_step_counter.c
 * @brief step counter implementation with accelerometer data
 * @author Sam
 */

#include "alg_step_counter.h"
#include <string.h>
#include <stdlib.h>
#include "MyNN/alg_mlp_network.h"

#define ABS(a) (((a) >= 0) ? (a) : (-(a)))

// buffer size to save historical data
#define BUF_SEC (3)
#define BUF_LEN (BUF_SEC * ACC_FS)
// window size to save new data
#define WIN_SEC (5)
#define WIN_LEN (WIN_SEC * ACC_FS)
// the total size to save data for each processing
#define BUF_WIN_LEN (BUF_LEN + WIN_LEN)

#define MEAN_LEN1 (11)
#define MEAN_LEN2 (5)
#define ACC_SENSOR_GRAVITY (4096)
#define STEP_ACC_DIFF_THRESHOLD (ACC_SENSOR_GRAVITY / 10) // 0.1g = 4096/10
#define PEAK_VALLEY_NUM (50)
#define PEAK_VALLEY_DIFFERENCE (ACC_SENSOR_GRAVITY / 14) // 1g/14 = 4096/14
// 4 steps max per second, points number between peak & valley is FS/4/2
#define TIME_THRESHOLD1 (4)
// 0.3 steps min per second, points number between peak & valley is FS/0.3/2
#define TIME_THRESHOLD2 (40)
#define LEFT_DATA_NUM (2)

/**
 * @brief for store the peaks/valleys number and locations
 * @param p_cnt: peak number
 * @param v_cnt: valley number
 * @param p_loc: peak locations
 * @param v_loc: valley locations
 */
typedef struct PeakValley {
    uint16_t p_cnt;
    uint16_t v_cnt;
    uint16_t *p_loc;
    uint16_t *v_loc;
} PeakValley;

/**
 * @brief struct for storing the features of a complete waveform,
 * including a peak and it's left valley and right valley
 */
typedef struct WaveFormFeature {
    uint16_t peak;
    uint16_t left_valley;
    uint16_t right_valley;
    uint16_t left_height;
    uint16_t rignt_height;
    uint16_t left_time;
    uint16_t right_time;
} WaveFormFeature;

/**
 * @brief struct for storing acceleration data
 */
typedef struct AccData {
    uint16_t len;
    uint16_t x_cnt;
    uint16_t y_cnt;
    uint16_t z_cnt;
    int16_t *x;
    int16_t *y;
    int16_t *z;
    uint16_t x_gyro_cnt;
    uint16_t y_gyro_cnt;
    uint16_t z_gyro_cnt;
    int16_t *gyro_x;
    int16_t *gyro_y;
    int16_t *gyro_z;
} AccData;

typedef struct AccDataHub {
    AccData win;
    AccData buf;
} AccDataHub;

/**
 * @brief struct for storing mean filter parameters and data
 */
typedef struct MeanFilter {
    uint16_t len;
    uint16_t index;
    int16_t *buf;
    uint16_t buf_full;
} MeanFilter;

typedef struct MeanFilterGroup {
    MeanFilter f1;
    MeanFilter f2;
} MeanFilterGroup;

typedef struct MeanFilterHub {
    MeanFilterGroup gyro_x;
    MeanFilterGroup gyro_y;
    MeanFilterGroup gyro_z;
    MeanFilterGroup x;
    MeanFilterGroup y;
    MeanFilterGroup z;
} MeanFilterHub;

/**
 * @brief struct for storing the output of mean filter
 */
typedef struct MeanOutput {
    int16_t raw;
    int16_t filt;
} MeanOutput;

uint16_t peak_loc[PEAK_VALLEY_NUM]   = { 0 };
uint16_t valley_loc[PEAK_VALLEY_NUM] = { 0 };
PeakValley peak_valley               = { 0, 0, NULL, NULL };

int16_t x_win[WIN_LEN] = { 0 }, y_win[WIN_LEN] = { 0 }, z_win[WIN_LEN] = { 0 };
int16_t x_buf[WIN_LEN] = { 0 }, y_buf[WIN_LEN] = { 0 }, z_buf[WIN_LEN] = { 0 };
int16_t x_gyro_win[WIN_LEN] = { 0 }, y_gyro_win[WIN_LEN] = { 0 },
        z_gyro_win[WIN_LEN] = { 0 };
int16_t x_gyro_buf[WIN_LEN] = { 0 }, y_gyro_buf[WIN_LEN] = { 0 },
        z_gyro_buf[WIN_LEN] = { 0 };
AccDataHub acc_data_hub;

int16_t xmean_filter1_buf[MEAN_LEN1] = { 0 },
        ymean_filter1_buf[MEAN_LEN1] = { 0 },
        zmean_filter1_buf[MEAN_LEN1] = { 0 };
int16_t xmean_filter2_buf[MEAN_LEN2] = { 0 },
        ymean_filter2_buf[MEAN_LEN2] = { 0 },
        zmean_filter2_buf[MEAN_LEN2] = { 0 };

int16_t x_gyro_mean_filter1_buf[MEAN_LEN1] = { 0 },
        y_gyro_mean_filter1_buf[MEAN_LEN1] = { 0 },
        z_gyro_mean_filter1_buf[MEAN_LEN1] = { 0 };
int16_t x_gyro_mean_filter2_buf[MEAN_LEN2] = { 0 },
        y_gyro_mean_filter2_buf[MEAN_LEN2] = { 0 },
        z_gyro_mean_filter2_buf[MEAN_LEN2] = { 0 };
MeanFilterHub mean_filter_hub;
static AlgoError array_max_min(int16_t *a, uint16_t a_len, int16_t direction,
                               int16_t *a_max_min)
{
    uint16_t i = 0;
    if (!a || !a_max_min || a_len == 0 ||
        (direction != -1 && direction != 1)) {
        return ALGO_ERR_GENERIC;
    }
    *a_max_min = *a;
    for (i = 1; i < a_len; i++) {
        if (*(a + i) * direction > (*a_max_min) * direction) {
            *a_max_min = *(a + i);
        }
    }
    return ALGO_NORMAL;
}

static AlgoError array_mean(int16_t *a, uint16_t a_len, int16_t *a_mean)
{
    int32_t sum = 0;
    uint16_t i  = 0;
    if (!a || !a_mean || a_len == 0) {
        return ALGO_ERR_GENERIC;
    }
    for (i = 0; i < a_len; i++) {
        sum += (int32_t)(a[i]);
    }
    *a_mean = (int16_t)(sum / a_len);
    return ALGO_NORMAL;
}

static AlgoError mean_filtering(int16_t input_data, MeanFilter *filter,
                                MeanOutput *output)
{
    uint16_t j        = 0;
    uint16_t half_len = 0;
    AlgoError ret     = ALGO_ERR_GENERIC;

    if (!filter || !output) {
        return ALGO_ERR_GENERIC;
    }

    half_len = (filter->len - 1) >> 1;

    if (filter->buf_full == 0) {
        if (filter->index < filter->len) {
            filter->buf[filter->index++] = input_data;
            if (filter->index % 2 == 1) {
                j = (filter->index - 1) >> 1;
                array_mean(filter->buf, filter->index, &(output->filt));
                output->raw = filter->buf[j];
                ret         = ALGO_NORMAL;
            }
            if (filter->index >= filter->len) {
                filter->buf_full = 1;
                filter->index    = 0;
            }
        }
    } else {
        if (filter->index < filter->len) {
            filter->buf[filter->index++] = input_data;
            if (filter->index >= filter->len) {
                filter->index = 0;
            }
        }
        j = (filter->index + half_len) % (filter->len);
        array_mean(filter->buf, filter->len, &(output->filt));
        output->raw = filter->buf[j];
        ret         = ALGO_NORMAL;
    }
    return ret;
}

static AlgoError if_a_in_A(uint16_t a, uint16_t *A, uint16_t A_len,
                           int16_t *in_flag)
{
    uint16_t i = 0;
    if (!A || !in_flag || A_len == 0) {
        return ALGO_ERR_GENERIC;
    }
    *in_flag = 0;
    while (i < A_len) {
        if (*(A + i) == a) {
            *in_flag = 1;
            break;
        }
        i++;
    }
    return ALGO_NORMAL;
}

static AlgoError delete_ith_A(uint16_t *A, uint16_t A_len, uint16_t i)
{
    uint16_t j = 0;
    if (!A || A_len == 0 || i >= A_len) {
        return ALGO_ERR_GENERIC;
    }
    for (j = i + 1; j < A_len; j++) {
        A[j - 1] = A[j];
    }
    A[A_len - 1] = 0;
    return ALGO_NORMAL;
}

static AlgoError find_possible_peak_valley(int16_t *a, uint16_t a_len,
                                           PeakValley *peak_valley)
{
    uint16_t i = 0;
    if (!a || !peak_valley || a_len == 0) {
        return ALGO_ERR_GENERIC;
    }
    for (i = 1; i < a_len - 1; i++) {
        if (peak_valley->p_cnt < PEAK_VALLEY_NUM) {
            if ((*(a + i) >= *(a + i - 1)) && (*(a + i) > *(a + i + 1))) {
                peak_valley->p_loc[peak_valley->p_cnt++] = i;
            }
        }
        if (peak_valley->v_cnt < PEAK_VALLEY_NUM) {
            if ((*(a + i) <= *(a + i - 1)) && (*(a + i) < *(a + i + 1))) {
                peak_valley->v_loc[peak_valley->v_cnt++] = i;
            }
        }
    }
    return ALGO_NORMAL;
}

static AlgoError merge_close_pole(int16_t *a, PeakValley *peak_valley,
                                  int16_t direction)
{
    uint16_t *pole_loc = NULL;
    uint16_t *v_loc    = NULL;
    uint16_t pole_cnt = 0, v_cnt = 0;
    uint16_t i = 0, j = 0;
    int16_t in_flag = 0;
    if (!a || !peak_valley || (direction != 1 && direction != -1)) {
        return ALGO_ERR_GENERIC;
    }
    if (direction == 1) {
        pole_loc = peak_valley->p_loc;
        pole_cnt = peak_valley->p_cnt;
        v_loc    = peak_valley->v_loc;
        v_cnt    = peak_valley->v_cnt;
    } else if (direction == -1) {
        pole_loc = peak_valley->v_loc;
        pole_cnt = peak_valley->v_cnt;
        v_loc    = peak_valley->p_loc;
        v_cnt    = peak_valley->p_cnt;
    }
    if (pole_cnt > 1) {
        i = 1;
        while (i < pole_cnt) {
            j = pole_loc[i - 1];
            while (j < pole_loc[i]) {
                if_a_in_A(j, v_loc, v_cnt, &in_flag);
                if (in_flag) {
                    break;
                }
                j++;
            }
            if (!in_flag) {
                if (a[pole_loc[i - 1]] * direction >
                    a[pole_loc[i]] * direction) {
                    delete_ith_A(pole_loc, pole_cnt--, i);
                } else {
                    delete_ith_A(pole_loc, pole_cnt--, i - 1);
                }
            } else {
                i++;
            }
        }
    }
    if (direction == 1) {
        peak_valley->p_cnt = pole_cnt;
    } else if (direction == -1) {
        peak_valley->v_cnt = pole_cnt;
    }
    return ALGO_NORMAL;
}

static AlgoError merge_close_peak_valley(int16_t *a, PeakValley *peak_valley)
{
    if (!a || !peak_valley) {
        return ALGO_ERR_GENERIC;
    }
    if (peak_valley->p_cnt > 1) {
        merge_close_pole(a, peak_valley, 1);
    }
    if (peak_valley->v_cnt > 1) {
        merge_close_pole(a, peak_valley, -1);
    }
    // make sure the first valley is before the first peak
    if (peak_valley->p_cnt > 0 &&
        ((peak_valley->v_cnt == 0) ||
         (peak_valley->v_cnt > 0 &&
          *(peak_valley->p_loc) < *(peak_valley->v_loc)))) {
        delete_ith_A(peak_valley->p_loc, peak_valley->p_cnt--, 0);
    }
    return ALGO_NORMAL;
}

static AlgoError remove_false_pole(int16_t *a, int16_t a_mean,
                                   uint16_t *pole_loc, uint16_t *pole_cnt,
                                   int16_t direction)
{
    uint16_t i = 0, cur_loc = 0;

    if (!a || !pole_loc || !pole_cnt || (direction != 1 && direction != -1)) {
        return ALGO_ERR_GENERIC;
    }

    while (i < *pole_cnt) {
        cur_loc = *(pole_loc + i);
        if (*(a + cur_loc) * direction < a_mean * direction) {
            delete_ith_A(pole_loc, *pole_cnt, i);
            (*pole_cnt)--;
        } else {
            i++;
        }
    }
    return ALGO_NORMAL;
}

static AlgoError remove_false_peak_valley(int16_t *a, uint16_t a_len,
                                          PeakValley *peak_valley)
{
    int16_t a_mean = 0;
    if (!a || !peak_valley || a_len == 0) {
        return ALGO_ERR_GENERIC;
    }
    array_mean(a, a_len, &a_mean);
    remove_false_pole(a, a_mean, peak_valley->p_loc, &(peak_valley->p_cnt), 1);
    remove_false_pole(a, a_mean, peak_valley->v_loc, &(peak_valley->v_cnt),
                      -1);
    return ALGO_NORMAL;
}

static AlgoError calculate_waveform_feature(int16_t *a, uint16_t a_len,
                                            WaveFormFeature *waveform)
{
    int16_t lh = 0, rh = 0;
    if (!a || !waveform || waveform->peak >= a_len ||
        waveform->left_valley >= a_len || waveform->right_valley >= a_len ||
        waveform->left_valley >= waveform->right_valley ||
        waveform->peak <= waveform->left_valley ||
        waveform->peak >= waveform->right_valley) {
        return ALGO_ERR_GENERIC;
    }
    lh = *(a + waveform->peak) - *(a + waveform->left_valley);
    rh = *(a + waveform->peak) - *(a + waveform->right_valley);
    if (lh < 0 || rh < 0) {
        return ALGO_ERR_GENERIC;
    }
    waveform->left_height  = lh;
    waveform->rignt_height = rh;
    waveform->left_time    = waveform->peak - waveform->left_valley;
    waveform->right_time   = waveform->right_valley - waveform->peak;
    return ALGO_NORMAL;
}

static AlgoError remove_asymmetric_peaks(int16_t *a, uint16_t a_len,
                                         PeakValley *peak_valley)
{
    uint16_t i = 0, j = 0, r1 = 0, r2 = 0;
    uint16_t *p_loc, *v_loc;
    WaveFormFeature waveform = { 0, 0, 0, 0, 0, 0, 0 };
    uint16_t last_valley_loc = 0;
    AlgoError ret            = ALGO_ERR_GENERIC;

    if (!a || !peak_valley) {
        return ALGO_ERR_GENERIC;
    }

    p_loc = peak_valley->p_loc;
    v_loc = peak_valley->v_loc;

    if (peak_valley->v_cnt > 1) {
        i = 0;
        while (i < peak_valley->p_cnt) {
            r1 = 0;
            r2 = 0;
            j  = 1;
            while (j < peak_valley->v_cnt) {
                if (v_loc[j - 1] < p_loc[i] && v_loc[j] > p_loc[i]) {
                    // find the valid peak with two valid valleys
                    r1                    = 1;
                    waveform.peak         = p_loc[i];
                    waveform.left_valley  = v_loc[j - 1];
                    waveform.right_valley = v_loc[j];
                    ret = calculate_waveform_feature(a, a_len, &waveform);
                    if (ret == ALGO_NORMAL) {
                        if (waveform.left_height > PEAK_VALLEY_DIFFERENCE &&
                            waveform.rignt_height > PEAK_VALLEY_DIFFERENCE &&
                            waveform.left_height > waveform.rignt_height / 2 &&
                            waveform.left_height < waveform.rignt_height * 2) {
                            last_valley_loc = v_loc[j];
                        } else {
                            // the peak is invalid
                            r2 = 1;
                            delete_ith_A(p_loc, peak_valley->p_cnt, i);
                            peak_valley->p_cnt--;
                            if (v_loc[j - 1] != last_valley_loc) {
                                delete_ith_A(v_loc, peak_valley->v_cnt, j - 1);
                                peak_valley->v_cnt--;
                            }
                        }
                    } else {
                        return ret;
                    }
                    break;
                }
                j++;
            }
            if (r1 == 0) {
                // there is no valid valleys for the peak
                delete_ith_A(p_loc, peak_valley->p_cnt, i);
                peak_valley->p_cnt--;
            } else if (r2 == 0) {
                i++;
            }
        }
    }
    return ALGO_NORMAL;
}

AlgoError group_mean_filtering(int16_t data, MeanFilterGroup *filter_group,
                               MeanOutput *mean_output)
{
    AlgoError ret = ALGO_ERR_GENERIC;
    MeanOutput output1, output2;
    if (!filter_group || !mean_output) {
        return ALGO_ERR_GENERIC;
    }
    ret = mean_filtering(data, &(filter_group->f1), &output1);
    if (ret == ALGO_NORMAL) {
        ret = mean_filtering(output1.filt, &(filter_group->f2), &output2);
        if (ret == ALGO_NORMAL) {
            mean_output->raw  = data;
            mean_output->filt = output2.filt;
            return ALGO_NORMAL;
        }
    }
    return ret;
}

static AlgoError load_data(int16_t *input, uint16_t len, int16_t *buf,
                           uint16_t buf_len, uint16_t *buf_cnt)
{
    uint16_t i = 0, j = 0;
    if (!input || !buf || !buf_cnt || buf_len == 0) {
        return ALGO_ERR_GENERIC;
    }
    if (*buf_cnt + len <= buf_len) {
        for (i = 0; i < len; i++) {
            buf[(*buf_cnt)++] = input[i];
        }
    } else {
        if (len >= buf_len) {
            for (i = 0; i < buf_len; i++) {
                buf[i] = input[len - buf_len + i];
            }
        } else {
            j = len + (*buf_cnt) - buf_len;
            for (i = j; i < (*buf_cnt); i++) {
                buf[i - j] = buf[i];
            }
            for (i = 0; i < len; i++) {
                buf[buf_len - len + i] = input[i];
            }
        }
        *buf_cnt = buf_len;
    }
    return ALGO_NORMAL;
}

static AlgoError acc_data_preprocess(AccInput *acc_input,
                                     MeanFilterHub *filter_hub,
                                     AccDataHub *acc_data_hub)
{
    AlgoError ret = ALGO_ERR_GENERIC;
    int16_t data  = 0;
    uint16_t i = 0, j = 0, array_cnt = 0;
    MeanOutput mean_output;
    int16_t array[WIN_LEN] = { 0 };

    if (!acc_input || !filter_hub || !acc_data_hub ||
        acc_input->len > WIN_LEN) {
        return ALGO_ERR_GENERIC;
    }

    int16_t *arr_p[6] = { acc_input->gyro_x, acc_input->gyro_y,
                          acc_input->gyro_z, acc_input->x,
                          acc_input->y,      acc_input->z };

    MeanFilterGroup *filt_p[6] = {
        &(filter_hub->gyro_x), &(filter_hub->gyro_y), &(filter_hub->gyro_z),
        &(filter_hub->x),      &(filter_hub->y),      &(filter_hub->z)
    };
    int16_t *acc_win_p[6] = {
        acc_data_hub->win.gyro_x, acc_data_hub->win.gyro_y,
        acc_data_hub->win.gyro_z, acc_data_hub->win.x,
        acc_data_hub->win.y,      acc_data_hub->win.z
    };
    uint16_t *acc_win_cnt[6] = {
        &(acc_data_hub->win.x_gyro_cnt), &(acc_data_hub->win.y_gyro_cnt),
        &(acc_data_hub->win.z_gyro_cnt), &(acc_data_hub->win.x_cnt),
        &(acc_data_hub->win.y_cnt),      &(acc_data_hub->win.z_cnt)
    };

    for (i = 0; i < 6; i++) {
        array_cnt = 0;
        for (j = 0; j < acc_input->len; j++) {
            data = *(arr_p[i] + j);
            ret  = group_mean_filtering(data, filt_p[i], &mean_output);
            if (ret == ALGO_NORMAL) {
                array[array_cnt++] = mean_output.filt;
            }
        }
        load_data(array, array_cnt, acc_win_p[i], acc_data_hub->win.len,
                  acc_win_cnt[i]);
    }
    if (*acc_win_cnt[0] != *acc_win_cnt[1] ||
        *acc_win_cnt[0] != *acc_win_cnt[2]) {
        return ALGO_ERR_GENERIC;
    }
    return ALGO_NORMAL;
}

AlgoError step_counter_init(void)
{
    peak_valley.p_loc = peak_loc;
    peak_valley.p_cnt = 0;

    peak_valley.v_loc = valley_loc;
    peak_valley.v_cnt = 0;

    mean_filter_hub.x.f1.buf      = xmean_filter1_buf;
    mean_filter_hub.x.f1.buf_full = 0;
    mean_filter_hub.x.f1.index    = 0;
    mean_filter_hub.x.f1.len      = MEAN_LEN1;

    mean_filter_hub.x.f2.buf      = xmean_filter2_buf;
    mean_filter_hub.x.f2.buf_full = 0;
    mean_filter_hub.x.f2.index    = 0;
    mean_filter_hub.x.f2.len      = MEAN_LEN2;

    mean_filter_hub.y.f1.buf      = ymean_filter1_buf;
    mean_filter_hub.y.f1.buf_full = 0;
    mean_filter_hub.y.f1.index    = 0;
    mean_filter_hub.y.f1.len      = MEAN_LEN1;

    mean_filter_hub.y.f2.buf      = ymean_filter2_buf;
    mean_filter_hub.y.f2.buf_full = 0;
    mean_filter_hub.y.f2.index    = 0;
    mean_filter_hub.y.f2.len      = MEAN_LEN2;

    mean_filter_hub.z.f1.buf      = zmean_filter1_buf;
    mean_filter_hub.z.f1.buf_full = 0;
    mean_filter_hub.z.f1.index    = 0;
    mean_filter_hub.z.f1.len      = MEAN_LEN1;

    mean_filter_hub.z.f2.buf      = zmean_filter2_buf;
    mean_filter_hub.z.f2.buf_full = 0;
    mean_filter_hub.z.f2.index    = 0;
    mean_filter_hub.z.f2.len      = MEAN_LEN2;

    mean_filter_hub.gyro_x.f1.buf      = x_gyro_mean_filter1_buf;
    mean_filter_hub.gyro_x.f1.buf_full = 0;
    mean_filter_hub.gyro_x.f1.index    = 0;
    mean_filter_hub.gyro_x.f1.len      = MEAN_LEN1;

    mean_filter_hub.gyro_x.f2.buf      = x_gyro_mean_filter2_buf;
    mean_filter_hub.gyro_x.f2.buf_full = 0;
    mean_filter_hub.gyro_x.f2.index    = 0;
    mean_filter_hub.gyro_x.f2.len      = MEAN_LEN2;

    mean_filter_hub.gyro_y.f1.buf      = y_gyro_mean_filter1_buf;
    mean_filter_hub.gyro_y.f1.buf_full = 0;
    mean_filter_hub.gyro_y.f1.index    = 0;
    mean_filter_hub.gyro_y.f1.len      = MEAN_LEN1;

    mean_filter_hub.gyro_y.f2.buf      = y_gyro_mean_filter2_buf;
    mean_filter_hub.gyro_y.f2.buf_full = 0;
    mean_filter_hub.gyro_y.f2.index    = 0;
    mean_filter_hub.gyro_y.f2.len      = MEAN_LEN2;

    mean_filter_hub.gyro_z.f1.buf      = z_gyro_mean_filter1_buf;
    mean_filter_hub.gyro_z.f1.buf_full = 0;
    mean_filter_hub.gyro_z.f1.index    = 0;
    mean_filter_hub.gyro_z.f1.len      = MEAN_LEN1;

    mean_filter_hub.gyro_z.f2.buf      = z_gyro_mean_filter2_buf;
    mean_filter_hub.gyro_z.f2.buf_full = 0;
    mean_filter_hub.gyro_z.f2.index    = 0;
    mean_filter_hub.gyro_z.f2.len      = MEAN_LEN2;

    acc_data_hub.buf.x          = x_buf;
    acc_data_hub.buf.x_cnt      = 0;
    acc_data_hub.buf.y          = y_buf;
    acc_data_hub.buf.y_cnt      = 0;
    acc_data_hub.buf.z          = z_buf;
    acc_data_hub.buf.z_cnt      = 0;
    acc_data_hub.buf.gyro_x     = x_gyro_buf;
    acc_data_hub.buf.x_gyro_cnt = 0;
    acc_data_hub.buf.gyro_y     = y_gyro_buf;
    acc_data_hub.buf.y_gyro_cnt = 0;
    acc_data_hub.buf.gyro_z     = z_gyro_buf;
    acc_data_hub.buf.z_gyro_cnt = 0;
    acc_data_hub.buf.len        = WIN_LEN;

    acc_data_hub.win.x          = x_win;
    acc_data_hub.win.x_cnt      = 0;
    acc_data_hub.win.y          = y_win;
    acc_data_hub.win.y_cnt      = 0;
    acc_data_hub.win.z          = z_win;
    acc_data_hub.win.z_cnt      = 0;
    acc_data_hub.win.gyro_x     = x_gyro_win;
    acc_data_hub.win.x_gyro_cnt = 0;
    acc_data_hub.win.gyro_y     = y_gyro_win;
    acc_data_hub.win.y_gyro_cnt = 0;
    acc_data_hub.win.gyro_z     = z_gyro_win;
    acc_data_hub.win.z_gyro_cnt = 0;
    acc_data_hub.win.len        = WIN_LEN;
    return ALGO_NORMAL;
}

float calculateVariance(int16_t *arr, int size)
{
    float sum      = 1;
    float mean     = 0;
    float variance = 0;

    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    mean = sum / size;

    for (int i = 0; i < size; i++) {
        variance += (arr[i] - mean) * (arr[i] - mean);
    }
    variance /= size;

    return variance;
}

int16_t right_shift_square(int16_t n, int shift)
{
    return (n / shift) * (n / shift);
}

float calculateMeanFeature(AccData *arr, int size, int type)
{
    float sum = 0;
    int16_t *x, *y, *z;
    x = arr->x;
    y = arr->y;
    z = arr->z;
    if (!type) {
        x = arr->gyro_x;
        y = arr->gyro_y;
        z = arr->gyro_z;
    }
    for (int i = 0; i < size; i++) {
        sum += right_shift_square(x[i], 2);
        sum += right_shift_square(y[i], 2);
        sum += right_shift_square(z[i], 2);
    }
    return sum / size;
}

// calculate cov matrix
void calculateCovarianceMatrix(float **arr, int rows, int cols,
                               float **covMatrix)
{
    float *means = (float *)malloc(cols * sizeof(float));
    for (int j = 0; j < cols; j++) {
        float sum = 0;
        for (int i = 0; i < rows; i++) {
            sum += arr[i][j];
        }
        means[j] = sum / rows;
    }

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < cols; j++) {
            float sum = 0;
            for (int k = 0; k < rows; k++) {
                sum += (arr[k][i] - means[i]) * (arr[k][j] - means[j]);
            }
            covMatrix[i][j] = sum / (rows - 1);
        }
    }

    free(means);
}

float make_peak_sub_mean(uint16_t *p_loc, uint16_t p_cnt)
{
    float sum = 0;
    for (int i = 0; i < p_cnt - 1; ++i) {
        sum += p_loc[i + 1] - p_loc[i];
    }
    // printf("[[%d]] ,%d", (int)sum, p_cnt);
    return sum / (p_cnt - 1);
}

int get_max_var_index(float *var_list, int n)
{
    int ret = 0;
    for (int i = 1; i < n; ++i) {
        if (var_list[ret] < var_list[i]) {
            ret = i;
        }
    }
    return ret;
}

AlgoError step_counter_process(AccInput *acc_input, uint16_t *step_num)
{
    uint16_t i = 0, j = 0;
    int16_t buf_win[BUF_WIN_LEN] = { 0 }; // buffer window len = history buffer
                                          // + new data buffer
    uint16_t buf_win_cnt = 0, last_v_loc = 0, left_len = 0;
    AccData *win = &(acc_data_hub.win), *buf = &(acc_data_hub.buf);
    int16_t *buf_axis_p[6] = { buf->gyro_x, buf->gyro_y, buf->gyro_z,
                               buf->x,      buf->y,      buf->z };
    int16_t *win_axis_p[6] = { win->gyro_x, win->gyro_y, win->gyro_z,
                               win->x,      win->y,      win->z };
    uint16_t *buf_cnt_p[6] = { &(buf->x_gyro_cnt), &(buf->y_gyro_cnt),
                               &(buf->z_gyro_cnt), &(buf->x_cnt),
                               &(buf->y_cnt),      &(buf->z_cnt) };
    uint16_t *win_cnt_p[6] = { &(win->x_gyro_cnt), &(win->y_gyro_cnt),
                               &(win->z_gyro_cnt), &(win->x_cnt),
                               &(win->y_cnt),      &(win->z_cnt) };
    int16_t buf_win_max = 0, buf_win_min = 0, max_min_diff = 0;
    int16_t xyz_steps[6] = { 0 };
    AlgoError ret        = ALGO_ERR_GENERIC;
    // uint16_t median_step = 0;
    uint16_t mean_step = 0;
    uint16_t var_step  = 0;
    float mean_gyro    = 0;
    float mean_acc     = 0;
    int class;

    if (!acc_input || !step_num) {
        return ALGO_ERR_GENERIC;
    }

    *step_num = 0;
    ret = acc_data_preprocess(acc_input, &mean_filter_hub, &acc_data_hub);
    if (ret != ALGO_NORMAL) {
        return ret;
    }

    if (acc_data_hub.win.x_cnt % ACC_FS != 0) {
        acc_data_hub.win.x_cnt      = 0;
        acc_data_hub.win.y_cnt      = 0;
        acc_data_hub.win.z_cnt      = 0;
        acc_data_hub.win.x_gyro_cnt = 0;
        acc_data_hub.win.y_gyro_cnt = 0;
        acc_data_hub.win.z_gyro_cnt = 0;
        return ret;
    }

    if (win->x_cnt >= win->len) {
        uint8_t is_p_loc_lt_6 = 0;
        float lambda[6];
        // float freq[6];

        int16_t sec_max_x = 0, sec_min_x = 0, sec_max_y = 0, sec_min_y = 0,
                sec_max_z = 0, sec_min_z = 0;
        int16_t sec_max_gyro_x = 0, sec_min_gyro_x = 0, sec_max_gyro_y = 0,
                sec_min_gyro_y = 0, sec_max_gyro_z = 0, sec_min_gyro_z = 0;

        array_max_min(win->x, win->x_cnt, 1, &sec_max_x);
        array_max_min(win->x, win->x_cnt, -1, &sec_min_x);
        array_max_min(win->y, win->y_cnt, 1, &sec_max_y);
        array_max_min(win->y, win->y_cnt, -1, &sec_min_y);
        array_max_min(win->z, win->z_cnt, 1, &sec_max_z);
        array_max_min(win->z, win->z_cnt, -1, &sec_min_z);
        array_max_min(win->gyro_x, win->x_gyro_cnt, 1, &sec_max_gyro_x);
        array_max_min(win->gyro_x, win->x_gyro_cnt, -1, &sec_min_gyro_x);
        array_max_min(win->gyro_y, win->y_gyro_cnt, 1, &sec_max_gyro_y);
        array_max_min(win->gyro_y, win->y_gyro_cnt, -1, &sec_min_gyro_y);
        array_max_min(win->gyro_z, win->z_gyro_cnt, 1, &sec_max_gyro_z);
        array_max_min(win->gyro_z, win->z_gyro_cnt, -1, &sec_min_gyro_z);
        float var_x       = calculateVariance(win->x, win->x_cnt);
        float var_y       = calculateVariance(win->y, win->y_cnt);
        float var_z       = calculateVariance(win->z, win->z_cnt);
        float var_gyro_x  = calculateVariance(win->gyro_x, win->x_gyro_cnt);
        float var_gyro_y  = calculateVariance(win->gyro_y, win->y_gyro_cnt);
        float var_gyro_z  = calculateVariance(win->gyro_z, win->z_gyro_cnt);
        float **cov_input = (float **)malloc((win->x_cnt) * sizeof(float *));
        mean_acc          = calculateMeanFeature(win, win->x_cnt, 0);
        mean_gyro         = calculateMeanFeature(win, win->x_cnt, 1);
        for (int i = 0; i < win->x_cnt; ++i) {
            cov_input[i] = (float *)malloc(3 * sizeof(float));
        }
        for (int j = 0; j < win->x_cnt; ++j) {
            cov_input[j][0] = win->x[j];
            cov_input[j][1] = win->y[j];
            cov_input[j][2] = win->z[j];
        }
        float **covariance_matrix = (float **)malloc(3 * sizeof(float *));
        for (int i = 0; i < 3; i++) {
            covariance_matrix[i] = (float *)malloc(3 * sizeof(float));
        }
        calculateCovarianceMatrix(cov_input, acc_data_hub.win.x_cnt, 3,
                                  covariance_matrix);
        float covariance_xy = fabs(covariance_matrix[0][1]);
        float covariance_xz = fabs(covariance_matrix[0][2]);
        float covariance_yz = fabs(covariance_matrix[1][2]);

        // gyro
        for (int j = 0; j < win->x_gyro_cnt; ++j) {
            cov_input[j][0] = win->gyro_x[j];
            cov_input[j][1] = win->gyro_y[j];
            cov_input[j][2] = win->gyro_z[j];
        }
        calculateCovarianceMatrix(cov_input, win->x_gyro_cnt, 3,
                                  covariance_matrix);
        float covariance_gyro_xy = fabs(covariance_matrix[0][1]);
        float covariance_gyro_xz = fabs(covariance_matrix[0][2]);
        float covariance_gyro_yz = fabs(covariance_matrix[1][2]);
        // free memory
        for (int i = 0; i < acc_data_hub.win.x_cnt; i++) {
            free(cov_input[i]);
        }
        free(cov_input);
        for (int i = 0; i < 3; i++) {
            free(covariance_matrix[i]);
        }
        free(covariance_matrix);

        for (i = 0; i < 6; i++) {
            peak_valley.p_cnt = 0;
            peak_valley.v_cnt = 0;

            buf_win_cnt = 0;
            if (*buf_cnt_p[i] > 0) {
                load_data(buf_axis_p[i], *buf_cnt_p[i], buf_win, BUF_WIN_LEN,
                          &buf_win_cnt);
                *buf_cnt_p[i] = 0;
            }
            load_data(win_axis_p[i], *win_cnt_p[i], buf_win, BUF_WIN_LEN,
                      &buf_win_cnt);
            *win_cnt_p[i] = 0;

            array_max_min(buf_win, buf_win_cnt, 1, &buf_win_max);
            array_max_min(buf_win, buf_win_cnt, -1, &buf_win_min);

            max_min_diff = buf_win_max - buf_win_min;
            if (max_min_diff > STEP_ACC_DIFF_THRESHOLD) {
                ret = find_possible_peak_valley(buf_win, buf_win_cnt,
                                                &peak_valley);
                if (ret != ALGO_NORMAL) {
                    break;
                }

                ret = remove_false_peak_valley(buf_win, buf_win_cnt,
                                               &peak_valley);
                if (ret != ALGO_NORMAL) {
                    break;
                }

                ret = merge_close_peak_valley(buf_win, &peak_valley);
                if (ret != ALGO_NORMAL) {
                    break;
                }

                ret = remove_asymmetric_peaks(buf_win, buf_win_cnt,
                                              &peak_valley);
                if (ret != ALGO_NORMAL) {
                    break;
                }

                if (peak_valley.v_cnt >= 1) {
                    last_v_loc = peak_valley.v_loc[peak_valley.v_cnt - 1];
                    left_len   = buf_win_cnt - last_v_loc + LEFT_DATA_NUM;
                    if (left_len < BUF_LEN) {
                        for (j = 0; j < left_len; j++) {
                            *(buf_axis_p[i] + j) =
                                buf_win[buf_win_cnt - left_len + j];
                            (*buf_cnt_p[i])++;
                        }
                    }
                }

                if (is_p_loc_lt_6 || peak_valley.p_cnt == 0) {
                    is_p_loc_lt_6 = 1;
                    lambda[i]     = 0;
                    // freq[i]       = 0;
                    // printf("[%d %d] \n", is_p_loc_lt_6, peak_valley.p_cnt);
                } else {
                    if (!is_p_loc_lt_6 && peak_valley.p_cnt > 1) {
                        lambda[i] = make_peak_sub_mean(peak_valley.p_loc,
                                                       peak_valley.p_cnt);
                    }
                    // freq[i] = peak_valley.p_cnt;
                }

                xyz_steps[i] = ((peak_valley.p_cnt));
            }
        }

        float input_data[20] = {
            mean_gyro,
            var_gyro_x,
            var_gyro_y,
            var_gyro_z,
            covariance_gyro_xy,
            covariance_gyro_xz,
            covariance_gyro_yz,
            lambda[0],
            lambda[1],
            lambda[2],
            mean_acc,
            var_x,
            var_y,
            var_z,
            covariance_xy,
            covariance_xz,
            covariance_yz,
            lambda[3],
            lambda[4],
            lambda[5],
        };
        // for (int i = 0; i < 20; ++i) {
        //     printf("input:%d ", (int)input_data[i]);
        // }
        // printf("\n");
        LayerData input_layer_data;
        input_layer_data.data = input_data;
        input_layer_data.size = 20;

        foward_process(&input_layer_data, &class);

        float var_list[] = {
            var_gyro_x, var_gyro_y, var_gyro_z, var_x, var_y, var_z,
        };
        var_step = xyz_steps[get_max_var_index(var_list, 6)];
    }

    if (ret == ALGO_NORMAL) {
        array_max_min(xyz_steps, 6, 1, &buf_win_max);
        array_max_min(xyz_steps, 6, -1, &buf_win_min);
        mean_step =
            (xyz_steps[0] + xyz_steps[1] + xyz_steps[2] + xyz_steps[3] +
             xyz_steps[4] + xyz_steps[5] - buf_win_max - buf_win_min) /
            4;
        if (class == 5 || class == 9) {
            *step_num = var_step;
        } else {
            *step_num = mean_step;
        }
        if (class == 6 || class == 5) {
            *step_num *= 2;
        }
        // printf("class:%d\n", class);
    }
    //  printf("x=%d, y=%d, z=%d, step=%d\t", xyz_steps[3], xyz_steps[4],
    //         xyz_steps[5], *step_num);
    return ret;
}
