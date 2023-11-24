#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define echpin 0
#define trgpin 1

int timeout = 26000;

int main(){
    stdio_init_all();
    gpio_init(trgpin);
    gpio_init(echpin);
    gpio_set_dir(trgpin, GPIO_OUT);
    gpio_set_dir(echpin, GPIO_IN);
    while(1){
        gpio_put(trgpin, 0);
        sleep_us(10);
        gpio_put(trgpin, 1);
        sleep_us(10);
        gpio_put(trgpin, 0);
        uint64_t width = 0;
        // Wait for the pulse to return or timeout
        uint32_t start_time = time_us_32();
        while ((gpio_get(echpin) == 0) && (time_us_32() - start_time < 1000000)) {}
        if (gpio_get(echpin) == 0) {
            // Timeout, return a large value to indicate no echo received
            printf("no trigger recieved\n");
        }
        else if(gpio_get(echpin) == 1){
            absolute_time_t startTime = get_absolute_time();
            while (gpio_get(echpin) == 1)
            {
                width++;
                sleep_us(1);
                if (width > timeout)
                    return 0;
            }
            absolute_time_t endTime = get_absolute_time();
            // Calculate distance based on the time it took for the pulse to return
            uint64_t pulseLength = absolute_time_diff_us(startTime, endTime);  // Convert to seconds
            float distance_cm = (float)pulseLength * 0.01715;
            printf("The distance is %.2f cm\n", distance_cm);
            sleep_ms(400);
        }
    }
    return 0;
}