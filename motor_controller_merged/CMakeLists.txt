add_executable(motor_control motor_control.c)

# pull in common dependencies and additional pwm hardware support
target_link_libraries(motor_control pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(motor_control)

# add url via pico_set_program_url
example_auto_set_url(motor_control)
