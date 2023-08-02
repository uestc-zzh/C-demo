#ifndef __ALG_STEP_COUNTER_H_
#define __ALG_STEP_COUNTER_H_

#include <stdint.h>
#include <stdio.h>

#define ACC_FS (25)

/**
 * @brief the struct of input data
 */
typedef struct AccInput {
    uint16_t len;
    int16_t *x;
    int16_t *y;
    int16_t *z;
} AccInput;

/**
 * @brief error code
 */
typedef enum AlgoError { ALGO_NORMAL = 0, ALGO_ERR_GENERIC = 1 } AlgoError;

/**
 * @brief initialize global variables for step counter algorithm
 */
AlgoError step_counter_init(void);

/**
 * @brief obtain step counts for current input data
 * @param acc_input: input data
 * @param step_num: the step counting results
 */
AlgoError step_counter_process(AccInput *acc_input, uint16_t *step_num);

#endif