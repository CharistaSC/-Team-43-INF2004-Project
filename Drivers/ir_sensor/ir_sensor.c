#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define irBarcode 1
#define MAX_BARCODE_LENGTH 30
#define intervalDelay 640

int barcodeStarted = 0;
size_t barcodeIndex = 0;
size_t intervalsSinceLastHigh = 0;
int barcode[MAX_BARCODE_LENGTH];

void processBarcode(const int* barcode, size_t length) {
    // Replace this function with your actual barcode processing logic
    printf("Barcode: ");
    for (size_t i = 0; i < length; ++i) {
        printf("%d \n", barcode[i]);
    }
    printf("\n");
}

int main(){
    stdio_init_all();
    gpio_init(irBarcode);
    gpio_set_dir(irBarcode, GPIO_IN);
    while(1){
        if(barcodeStarted == 0 && !gpio_get(irBarcode)){
            printf("waiting for first bit \n");
            sleep_ms(intervalDelay);
        }
        if(barcodeStarted == 0 && gpio_get(irBarcode)){
            printf("Starter bit detected \n");
            barcodeStarted = 1;
            barcode[barcodeIndex] = 1;
            printf("1\n");
        }
        while(barcodeStarted == 1){
            if(intervalsSinceLastHigh <= 6){
                if(gpio_get(irBarcode)){
                    barcode[barcodeIndex++] = 1;
                    printf("1\n");
                    intervalsSinceLastHigh = 0;
                    sleep_ms(intervalDelay);
                }
                else if(!gpio_get(irBarcode)){
                    barcode[barcodeIndex++] = 0;
                    printf("0\n");
                    intervalsSinceLastHigh++;
                    sleep_ms(intervalDelay);
                }
            }
            else if(intervalsSinceLastHigh > 6){
                // Process the complete barcode
                processBarcode(barcode, barcodeIndex);
                // Reset for the next barcode
                intervalsSinceLastHigh = 0;
                barcodeIndex = 0;
                barcodeStarted = 0;
                sleep_ms(intervalDelay);
            }
        }
    }
    return 0;
}

/*
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_BARCODE_LENGTH 100

// Function to process barcode data
void processBarcode(const uint8_t* barcode, size_t length) {
    // Replace this function with your actual barcode processing logic
    printf("Barcode: ");
    for (size_t i = 0; i < length; ++i) {
        printf("%d", barcode[i]);
    }
    printf("\n");
}

int main() {
    // Simulated barcode data (replace with actual data)
    uint8_t simulatedBarcode[] = {0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1};

    // Variables for barcode processing
    bool barcodeStarted = false;
    uint8_t barcode[MAX_BARCODE_LENGTH];
    size_t barcodeIndex = 0;
    size_t intervalsSinceLastHigh = 0;

    // Simulated reading loop (replace with actual reading logic)
    for (size_t i = 0; i < sizeof(simulatedBarcode) / sizeof(simulatedBarcode[0]); ++i) {
        uint8_t currentBit = simulatedBarcode[i];

        // Check for the first HIGH bit to start barcode reading
        if (!barcodeStarted && currentBit == 1) {
            barcodeStarted = true;
            barcodeIndex = 0;
            intervalsSinceLastHigh = 0;
            barcode[barcodeIndex++] = currentBit;
        } else if (barcodeStarted) {
            // Save the data at specified intervals
            if (intervalsSinceLastHigh >= 2) {  // Adjust this interval as needed
                barcode[barcodeIndex++] = currentBit;
                intervalsSinceLastHigh = 0;
            } else {
                intervalsSinceLastHigh++;
            }

            // Check for the last HIGH bit indicating the end of barcode
            if (currentBit == 1 && i == sizeof(simulatedBarcode) / sizeof(simulatedBarcode[0]) - 1) {
                // Process the complete barcode
                processBarcode(barcode, barcodeIndex);
                // Reset for the next barcode
                barcodeStarted = false;
            }
        }
    }

    return 0;
}


        // Check for the first HIGH bit to start barcode reading
        if ((barcodeStarted == 0) && (barcodeIndex == 0)) {
            printf("Start bit detected\n");
            barcodeStarted = 1;
            intervalsSinceLastHigh = 0;
            barcode[barcodeIndex] = gpio_get(irBarcode);
        } else if (barcodeStarted) {
            // Save the data at specified intervals
            if (intervalsSinceLastHigh <= 6 && gpio_get(irBarcode)) {  // Adjust this interval as needed
                barcode[barcodeIndex] = gpio_get(irBarcode);
                printf(barcode[barcodeIndex]);
                barcodeIndex++;
                intervalsSinceLastHigh = 0;
                sleep_ms(400);
            } else {
                barcode[barcodeIndex] = gpio_get(irBarcode);
                printf(barcode[barcodeIndex]);
                barcodeIndex++;
                intervalsSinceLastHigh++;
                sleep_ms(400);
            }

            // Check for the last HIGH bit indicating the end of barcode
            if (intervalsSinceLastHigh > 6) {
                // Process the complete barcode
                processBarcode(barcode, barcodeIndex);
                // Reset for the next barcode
                barcodeStarted = false;
            }
        }

*/