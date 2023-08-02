/*
 * Internal Header for clock module
 *
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 */

#ifndef __CLK_DRV_H__
#define __CLK_DRV_H__

/** @addtogroup DRV_CLOCK
 *  @brief CLOCK Driver
 *  @ingroup DRIVER
 *  @{
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief - Generic clock config structure
 */
typedef struct ClkHwCfg {
    const char *name;
    uint8_t type;
} ClkHwCfg;

/**
 * @brief - Callback operations for each clock.
 *
 * @param init Init the clock hardware with config if necessary.
 * @param set_rate Change the rate of this clock.
 * @param recalc_rate Recalculate the rate of this clock, by querying hardware.
 *                 Parent rate will be return if clock HW not change rate.
 * @param set_parent Change the input source of this clock. Optional, used for
 *                 MUX clock.
 * @param get_parent return clock' parent ID. MUST be implemented.
 * @param enable Disable/Enable the clock.
 */
typedef struct ClkFuncs {
    int (*init)(const ClkHwCfg *config);
    int (*set_rate)(const ClkHwCfg *config, uint32_t rate,
                    uint32_t parent_rate);
    uint32_t (*recalc_rate)(const ClkHwCfg *config, uint32_t parent_rate);
    int (*set_parent)(const ClkHwCfg *config, uint8_t parent_id);
    uint8_t (*get_parent)(const ClkHwCfg *config);
    void (*enable)(const ClkHwCfg *config, bool enable);
    int (*get_enable)(const ClkHwCfg *config, bool *enable);
} ClkFuncs;

extern const ClkFuncs fix_funcs;
extern const ClkFuncs gate_funcs;
extern const ClkFuncs div_funcs;
extern const ClkFuncs fix_div_funcs;
extern const ClkFuncs mux_funcs;
extern const ClkFuncs comp_funcs;
extern const ClkFuncs pll_funcs;

/** @} */

#endif /* __CLK_DRV_H__ */
