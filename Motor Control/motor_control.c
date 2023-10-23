/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define motorPWMR 0
#define motorPWML 1

#define motorDirR01 2
#define motorDirR02 3
#define motorDirL01 5
#define motorDirL02 4

int main() {
    stdio_init_all();
    gpio_init(motorDirR01);
    gpio_init(motorDirR02);
    gpio_init(motorDirL01);
    gpio_init(motorDirL02);
    gpio_set_dir(motorDirR01, GPIO_OUT);
    gpio_set_dir(motorDirR02, GPIO_OUT);
    gpio_set_dir(motorDirL01, GPIO_OUT);
    gpio_set_dir(motorDirL02, GPIO_OUT);
    gpio_set_function(motorPWMR, GPIO_FUNC_PWM);
    gpio_set_function(motorPWML, GPIO_FUNC_PWM);
    uint slice_num01 = pwm_gpio_to_slice_num(motorPWMR);
    uint slice_num02 = pwm_gpio_to_slice_num(motorPWML);
    pwm_set_clkdiv(slice_num01, 100);
    pwm_set_wrap(slice_num01, 12500);
    pwm_set_clkdiv(slice_num02, 100); 
    pwm_set_wrap(slice_num02, 12500);
    pwm_set_enabled(slice_num01, true);
    pwm_set_enabled(slice_num02, true);
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/2);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/2);

    while(1){
        pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/1);
        pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/1);
        gpio_put(motorDirR01, 1);
        gpio_put(motorDirR02, 0);
        gpio_put(motorDirL01, 1);
        gpio_put(motorDirL02, 0);
        sleep_ms(4000);
        pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/2);
        pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/2);
        gpio_put(motorDirR01, 0);
        gpio_put(motorDirR02, 1);
        gpio_put(motorDirL01, 0);
        gpio_put(motorDirL02, 1);
        sleep_ms(2000);
    }

}
