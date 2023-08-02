#ifndef __MLP_NETWORK_H_
#define __MLP_NETWORK_H_


// #define NET_ERR_GENERIC -1
// #define NET_NORMAL 0

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum ClassResult
{
    CLASS_0 = 0,
    CLASS_1 = 1
} ClassResult;



typedef int NETError;

typedef struct LayerData
{
    uint16_t size;
    float *data;
} LayerData;

int networks_init(void);

/* input_data size should be 2 */
int foward_process(LayerData *input_data, int *class);

#endif