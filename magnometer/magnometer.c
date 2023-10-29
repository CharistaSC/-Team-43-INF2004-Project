#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define MAGNETOMETER_ADDRESS 0x1E
#define ACCELEROMETER_ADDRESS 0x19

uint8_t config[2] = {0x20, 0x27};
uint8_t config_accel[2] = {0x23, 0x00};
uint8_t config_magneto[2] = {0x02, 0x00};
uint8_t config_cra[2] = {0x00, 0x10};
uint8_t config_crb[2] = {0x01, 0x20};

int main() {
    stdio_init_all();

    i2c_init(i2c0, 400000); // 400 kHz
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);
    while (1) {

        // Write magnetometer configuration
        i2c_write_blocking(i2c0, MAGNETOMETER_ADDRESS, config_magneto, 2, false);
        i2c_write_blocking(i2c0, MAGNETOMETER_ADDRESS, config_cra, 2, false);
        i2c_write_blocking(i2c0, MAGNETOMETER_ADDRESS, config_crb, 2, false);
        sleep_ms(10); // Waiting for magnetometer data

        // Read magnetometer data
        uint8_t mag_data = 0x03; // X-axis msb data register
        i2c_write_blocking(i2c0, MAGNETOMETER_ADDRESS, &mag_data, 1, true);
        uint8_t magdata[6] = {0};
        i2c_read_blocking(i2c0, MAGNETOMETER_ADDRESS, magdata, 6, false);

        int16_t magX = (magdata[0] << 8) | magdata[1];
        if (magX > 32767) {magX -= 65536;}

        int16_t magY = (magdata[2] << 8) | magdata[3];
        if (magY > 32767) {magY -= 65536;}

        int16_t magZ = (magdata[2] << 8) | magdata[3];
        if (magZ > 32767) {magZ -= 65536;}

        // Write accelerometer configuration
        i2c_write_blocking(i2c0, ACCELEROMETER_ADDRESS, config, 2, false);
        i2c_write_blocking(i2c0, ACCELEROMETER_ADDRESS, config_accel, 2, false);
        sleep_ms(10); // Waiting for accelerometer data

        // Read accelerometer data
        uint8_t reg_accel = 0x28; // X-axis lsb data register
        i2c_write_blocking(i2c0, ACCELEROMETER_ADDRESS, &reg_accel, 1, true);
        uint8_t acceldata[6] = {0};
        i2c_read_blocking(i2c0, ACCELEROMETER_ADDRESS, acceldata, 6, false);

        int16_t accelX = (acceldata[0] << 8) | acceldata[1];
        if (accelX > 32767) {accelX -= 65536;}

        int16_t accelY = (acceldata[2] << 8) | acceldata[3];
        if (accelY > 32767) {accelY -= 65536;}

        int16_t accelZ = (acceldata[2] << 8) | acceldata[3];
        if (accelZ > 32767) {accelZ -= 65536;}

        printf("Accel: X=%d, Y=%d, Z=%d", accelX, accelY, accelZ);
        printf("Magnetometer Data - X: %d, Y: %d, Z: %d\n", magX, magY, magZ);

        sleep_ms(200); // Sleep for 100ms (adjust as needed).
    }
    return 0;
}