#include "pico/stdlib.h"

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
char hidden_char[];
volatile uint16_t barcode[];
int char_count;
bool is_thin, current_pin_state, previous_interrupt_pin_state, skip, barcode_reading;


const uint code39_binary[27] = {
    0b011110110, // A
    0b110110110, // B
    0b010110111, // C
    0b111100110, // D
    0b011100111, // E
    0b110100111, // F
    0b111110010, // G
    0b011110011, // H
    0b111100011, // J
    0b011111100, // K
    0b110111100, // L
    0b010111101, // M
    0b111101100, // N
    0b011101101, // O
    0b110101101, // P
    0b111111000, // Q
    0b011111001, // R
    0b110111001, // S
    0b111101001, // T
    0b001111110, // U
    0b100111110, // V
    0b000111111, // W
    0b101101110, // X
    0b001101111, // Y
    0b100101111, // Z
    0b101101011  // * (Asterisk)
};

const char code39_characters[27] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 
    'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
    'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '*'
};

void printBinary(uint16_t value) {
    for (int i = 8; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}

uint16_t flipBinary(uint16_t num) {
    uint16_t result = 0;
    while (num > 0) {
        result = (result << 1) | (num & 1);
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
