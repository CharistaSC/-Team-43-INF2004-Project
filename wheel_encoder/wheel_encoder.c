#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Constants
#define DISTANCE_PER_PULSE 0.01  // Adjust this value based on your system
#define TIMEOUT_INTERVAL_MS 5000 // 10 seconds

// Global variables
volatile uint32_t pulse_count = 0;
volatile uint64_t prev_time = 0;
volatile uint64_t timer_start_time = 0;
volatile bool timer_enabled = false;

void gpio_callback(uint gpio, uint32_t events) {
    // Increment the pulse count on each encoder pulse
    pulse_count++;

    // Calculate time elapsed since the last pulse
    uint64_t current_time = time_us_64();
    uint64_t time_elapsed = current_time - prev_time;

    // Calculate speed (distance traveled per unit time)
    float speed = DISTANCE_PER_PULSE / (time_elapsed / 1e6);  // Convert time to seconds
    printf("Speed: %.2f cm/second\n", speed);

    // Update the previous time
    prev_time = current_time;

    // Enable the timer
    timer_enabled = true;
    timer_start_time = current_time;
}

int main() {
    stdio_init_all();

    printf("Hello GPIO IRQ\n");
    gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    while (1) {
        // Check if the timer is enabled and has expired
        if (timer_enabled) {
            uint64_t current_time = time_us_64();
            uint64_t time_elapsed = current_time - timer_start_time;

            // If 10 seconds have passed without an interrupt, disable the counter
            if (time_elapsed >= TIMEOUT_INTERVAL_MS * 1000) {
                float total_distance = pulse_count * DISTANCE_PER_PULSE;
                printf("Total Distance: %.2f cm\n", total_distance);

                // Reset pulse count and disable the timer
                pulse_count = 0;
                timer_enabled = false;
            }
        }

        // Sleep for a short duration to avoid busy-waiting
        sleep_ms(100);
    }
}
