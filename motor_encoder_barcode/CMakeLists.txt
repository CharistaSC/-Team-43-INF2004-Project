add_executable(motor_encoder_barcode motor_encoder_barcode.c)

# pull in common dependencies
target_link_libraries(motor_encoder_barcode pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(motor_encoder_barcode)
pico_enable_stdio_usb(motor_encoder_barcode 1)
# add url via pico_set_program_url
example_auto_set_url(motor_encoder_barcode)
