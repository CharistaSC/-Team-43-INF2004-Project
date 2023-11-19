#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "motor_setup.h"
#include "code39_barcode.h"

// GPIO Pin Configurations
#define barcode_IR 6
#define encoderPin 7

// Constants
#define DISTANCE_PER_PULSE 1.05  // Per divit in the wheel encoder. 21 cm diameter and 20 divits in the wheel encoder
#define TIMEOUT_INTERVAL_MS 5000 // 10 seconds

// variables for encoder
volatile uint32_t pulse_count = 0;
volatile uint64_t encoder_prev_time = 0;
volatile uint64_t timer_start_time = 0;
volatile bool timer_enabled = false;

// varibales for barcode
typedef enum {
    FIRST_STATE,
    READING_STATE,
    DECODING_STATE
} BarcodeState;
BarcodeState barcode_state = FIRST_STATE;
volatile uint16_t cumulative_barcode_value = 0b000000000;
volatile uint32_t bit_count = 0;
volatile uint32_t threshold = 0;
volatile uint32_t current_time, start_time;
volatile uint32_t duration = 0;
char decoded_char, delimiter; // Initialize to a default value
char hidden_char[3];
volatile uint16_t barcode[3];
int char_count;
bool is_thin, current_pin_state, previous_interrupt_pin_state, skip;

void printBinary(uint16_t value) {
    //for (int i = sizeof(value) * 8 - 1; i >= 0; i--) {
    for (int i = 8; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}

void encoderISR() {
    printf("encoderISR triggered");
    // Increment the pulse count on each encoder pulse
    pulse_count++;

    // Calculate time elapsed since the last pulse
    uint64_t encoder_current_time = time_us_64();
    uint64_t time_elapsed = encoder_current_time - encoder_prev_time;

    // Calculate speed (distance traveled per unit time)
    float speed = DISTANCE_PER_PULSE / (time_elapsed / 1e6);  // Convert time to seconds
    printf("Speed: %.2f cm/second\n", speed);

    // Update the previous time
    encoder_prev_time = encoder_current_time;

    // Enable the timer
    timer_enabled = true;
    timer_start_time = encoder_current_time;
}

void barcodeISR() {
    current_pin_state = gpio_get(barcode_IR);
    if (current_pin_state != previous_interrupt_pin_state) {
        current_time = time_us_32();
        printf("Current Time: %lu  ", current_time);
        switch (barcode_state) {
            case FIRST_STATE:
                printf("FIRST STATE \n");
                start_time = current_time;
                barcode_state = READING_STATE;
                break;

            case READING_STATE:
                printf("READING STATE ");
                duration = current_time - start_time;
                start_time = current_time;
                if(!skip){
                    printf("Current bit count: %d  Char count: %d  ", bit_count, char_count);
                    if (bit_count == 0 && char_count == 0) {threshold = duration * 1.32;}// First bar, set threshold
                    printf("Threshold: %lu, Duration: %lu  ", threshold, duration);
                    is_thin = (duration < threshold);
                    cumulative_barcode_value = (cumulative_barcode_value << 1) | (is_thin ? 1 : 0);
                    printf("Cumulative_barcode_value: %lu  \n", cumulative_barcode_value);
                    bit_count++;

                    if (bit_count > 8){
                        printf("Cumulative_barcode_value is binary is: ");
                        printBinary(cumulative_barcode_value);
                        printf("\n");
                        barcode[char_count] = cumulative_barcode_value;
                        printf("barcode[%d] is %lu  \n", char_count, barcode[char_count]);
                        cumulative_barcode_value = 0;
                        if(char_count == 2){
                            barcode_state = DECODING_STATE;
                            printf("\n"); printf("\n"); 
                        }else{
                            char_count++;
                            skip = true;
                        }
                        bit_count = 0;
                    }
                }else{skip = false;}
                break;
            case DECODING_STATE:
                printf("DECODING STATE \n");
                break;
        }
        previous_interrupt_pin_state = current_pin_state;
    }
    gpio_acknowledge_irq (barcode_IR, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);
}

uint16_t flipBinary(uint16_t num) {
    uint16_t result = 0;
    int numBits = 9;

    for (int i = 0; i < numBits; ++i) {
        result <<= 1;
        result ^= num & 1;
        num >>= 1;
    }

    return result;
}

void decodeBarcode() {
    printf("decodingBarcode Function \n");
    for (int x = 0; x < char_count + 1; x++) {
        bool match_found = false;
        printf("Char num decoding %d  \n", x);

        for (int i = 0; i < 4; i++) {
            printf("Barcode %d is %lu and code39_binary is %lu ", x, barcode[x], code39_binary[i]);
            printf("Binary for both are ");
            printBinary(barcode[x]);
            printf(" ");
            printBinary(code39_binary[i]);
            printf("\n");
            if (barcode[x] == code39_binary[i]) {
                hidden_char[x] = code39_characters[i];
                printf("hidden_char: '%c' \n", hidden_char[x]);
                match_found = true;
                break;
            }
        }

        // If no match is found, flip the binary
        if (!match_found) {
            // You can adjust this based on the actual binary size
            barcode[x] = flipBinary(barcode[x]);

            for (int i = 0; i < 4; i++) {
                printf("Barcode %d is %lu and code39_binary is %lu ", x, barcode[x], code39_binary[i]);
                printf("Binary for both are ");
                printBinary(barcode[x]);
                printf(" ");
                printBinary(code39_binary[i]);
                printf("\n");
                if (barcode[x] == code39_binary[i]) {
                    hidden_char[x] = code39_characters[i];
                    printf("hidden_char: '%c' \n", hidden_char[x]);
                    match_found = true;
                    break;
                }
            }
        }
    }
}

int main() {

    printf("Initialize standard I/O \n");
    // Initialize standard I/O
    stdio_init_all();
    gpio_init(barcode_IR);
    gpio_set_dir(barcode_IR, GPIO_IN);
    gpio_pull_down(barcode_IR);
    // Delay to before moving off
    for(int i = 10; i > 0; i--) {sleep_ms(1000); printf("%d Sec delay reminding \n", i);}
    current_pin_state = gpio_get(barcode_IR);
    previous_interrupt_pin_state = current_pin_state;
    gpio_set_irq_enabled_with_callback(encoderPin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoderISR);
    gpio_set_irq_enabled_with_callback(barcode_IR, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &barcodeISR);
    motor_int();
    move_forward();

    // Main control loop
    while(1) {
        if (timer_enabled) {
            uint64_t encoder_current_time = time_us_64();
            uint64_t time_elapsed = encoder_current_time - timer_start_time;

            // If 10 seconds have passed without an interrupt, disable the counter
            if (time_elapsed >= TIMEOUT_INTERVAL_MS * 1000) {
                float total_distance = pulse_count * DISTANCE_PER_PULSE;
                printf("Total Distance: %.2f cm\n", total_distance);

                // Reset pulse count and disable the timer
                pulse_count = 0;
                timer_enabled = false;
            }
        }
        if(barcode_state == READING_STATE){
            
        }
        else if (barcode_state == DECODING_STATE){
            gpio_set_irq_enabled_with_callback(barcode_IR, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false, &barcodeISR);
            printf("DECODING STATE \n");
            move_stop();
            decodeBarcode();

            delimiter = hidden_char[0];
            for(int x = 0; x < char_count; x++){
                if(delimiter != hidden_char[x]){
                    decoded_char = hidden_char[x];
                    printf("Decoded Char is %c", decoded_char);
                }
            }
            barcode_state = FIRST_STATE;
        }
        sleep_ms(100);
    }
}
