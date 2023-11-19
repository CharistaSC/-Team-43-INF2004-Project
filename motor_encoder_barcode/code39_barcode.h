#include "pico/stdlib.h"

const uint code39_binary[4] = {
    0b011110110, // A
    0b110100111, // F
    0b100101111, // Z
    0b101101011  // * (Asterisk)
};

const char code39_characters[4] = {
    'A', 'F', 'Z', '*'
};