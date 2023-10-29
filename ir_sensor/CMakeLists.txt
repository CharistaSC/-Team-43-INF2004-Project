add_executable(ir_sensor ir_sensor.c)

# pull in common dependencies
target_link_libraries(ir_sensor pico_stdlib hardware_pwm hardware_adc hardware_timer pico_time)

# enable usb output, disable uart output
pico_enable_stdio_usb(ir_sensor 1)
pico_enable_stdio_uart(ir_sensor 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(ir_sensor)

# add url via pico_set_program_url
example_auto_set_url(ir_sensor)
