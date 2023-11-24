add_executable(magnometer magnometer.c)

# Link the Pico SDK libraries and pico_i2c
target_link_libraries(magnometer
    pico_stdlib
    hardware_i2c
    hardware_pwm
)

# Enable USB output and start the program when the Pico is connected
pico_enable_stdio_usb(magnometer 1)
pico_enable_stdio_uart(magnometer 0)

# Create a .uf2 file for flashing
pico_add_extra_outputs(magnometer)
