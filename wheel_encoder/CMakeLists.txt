add_executable(wheel_encoder wheel_encoder.c)

# pull in common dependencies
target_link_libraries(wheel_encoder pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(wheel_encoder)
pico_enable_stdio_usb(wheel_encoder 1)
# add url via pico_set_program_url
example_auto_set_url(wheel_encoder)
