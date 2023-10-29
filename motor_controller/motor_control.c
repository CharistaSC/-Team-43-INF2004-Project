/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Include necessary libraries and header files
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define GPIO pins for PWM signals, motor directions, etc.
#define motorPWMR 0
#define motorPWML 1

#define motorDirR01 2
#define motorDirR02 3
#define motorDirL01 5
#define motorDirL02 4

int main() {
    // Initialize standard I/O
    stdio_init_all();

    // Initialize GPIO pins for motor directions
    gpio_init(motorDirR01);
    gpio_init(motorDirR02);
    gpio_init(motorDirL01);
    gpio_init(motorDirL02);

    // Set GPIO directions to output
    gpio_set_dir(motorDirR01, GPIO_OUT);
    gpio_set_dir(motorDirR02, GPIO_OUT);
    gpio_set_dir(motorDirL01, GPIO_OUT);
    gpio_set_dir(motorDirL02, GPIO_OUT);

    // Set GPIO functions for PWM
    gpio_set_function(motorPWMR, GPIO_FUNC_PWM);
    gpio_set_function(motorPWML, GPIO_FUNC_PWM);

    // Get PWM slice numbers for the specified GPIO pins
    // Here I am creating 2 PWM, that way we can control both wheels at
    // different speed. Example for turning.
    uint slice_num01 = pwm_gpio_to_slice_num(motorPWMR);
    uint slice_num02 = pwm_gpio_to_slice_num(motorPWML);

    // Configure PWM settings (clock divider and wrap value)
    
    pwm_set_clkdiv(slice_num01, 100);
    pwm_set_wrap(slice_num01, 12500);
    pwm_set_clkdiv(slice_num02, 100);
    pwm_set_wrap(slice_num02, 12500);

    // Enable PWM slices
    pwm_set_enabled(slice_num01, true);
    pwm_set_enabled(slice_num02, true);

    // Set initial PWM duty cycle levels
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/2);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/2);

    // Main control loop
    while(1) {
        // Set PWM and motor directions for forward motion
        pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/1);
        pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/1);
        gpio_put(motorDirR01, 1);
        gpio_put(motorDirR02, 0);
        gpio_put(motorDirL01, 1);
        gpio_put(motorDirL02, 0);

        // Wait for a specified duration (4000 milliseconds)
        sleep_ms(4000);

        // Set PWM and motor directions for backward motion
        pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/2);
        pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/2);
        gpio_put(motorDirR01, 0);
        gpio_put(motorDirR02, 1);
        gpio_put(motorDirL01, 0);
        gpio_put(motorDirL02, 1);

        // Wait for a specified duration (2000 milliseconds)
        sleep_ms(4000);

        pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/1);
        pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/1);
        gpio_put(motorDirR01, 0);
        gpio_put(motorDirR02, 1);
        gpio_put(motorDirL01, 1);
        gpio_put(motorDirL02, 0);

        // Wait for a specified duration (4000 milliseconds)
        sleep_ms(4000);

        // Set PWM and motor directions for backward motion
        pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/1);
        pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/1);
        gpio_put(motorDirR01, 1);
        gpio_put(motorDirR02, 0);
        gpio_put(motorDirL01, 0);
        gpio_put(motorDirL02, 1);

        // Wait for a specified duration (2000 milliseconds)
        sleep_ms(4000);
    }
}
