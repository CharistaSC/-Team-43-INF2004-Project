#define motorDirR01 10
#define motorDirR02 11
#define motorDirL01 13
#define motorDirL02 12

#define motorPWMR 14
#define motorPWML 15

// Speed adjustment to accommodate physical deformities. Higher value means lower speed
#define dc_adjust_right 1400 
#define dc_adjust_left 500

uint slice_num01;
uint slice_num02;

void motor_int(){
    printf("motor_int \n");
    // Initialize GPIO pins for motor directions
    gpio_init(motorDirR01);
    gpio_init(motorDirR02);
    gpio_init(motorDirL01);
    gpio_init(motorDirL02);

    // Set GPIO directions to output
    gpio_set_dir(motorDirR01, GPIO_OUT);
    gpio_set_dir(motorDirR02, GPIO_OUT);
    gpio_set_dir(motorDirL01, GPIO_OUT);
    gpio_set_dir(motorDirL02, GPIO_OUT);

    // Set GPIO functions for PWM
    gpio_set_function(motorPWMR, GPIO_FUNC_PWM);
    gpio_set_function(motorPWML, GPIO_FUNC_PWM);

    // Get PWM slice numbers for the specified GPIO pins
    // Here I am creating 2 PWM, that way we can control both wheels at
    // different speed. Example for turning.
    slice_num01 = pwm_gpio_to_slice_num(motorPWMR);
    slice_num02 = pwm_gpio_to_slice_num(motorPWML);

    // Configure PWM settings (clock divider and wrap value)
    
    pwm_set_clkdiv(slice_num01, 100);
    pwm_set_wrap(slice_num01, 12500);
    pwm_set_clkdiv(slice_num02, 100);
    pwm_set_wrap(slice_num02, 12500);

    // Enable PWM slices
    pwm_set_enabled(slice_num01, true);
    pwm_set_enabled(slice_num02, true);
}

void move_stop(){
    printf("move_stop \n");
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 0);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 0);
    gpio_put(motorDirR01, 0);
    gpio_put(motorDirR02, 0);
    gpio_put(motorDirL01, 0);
    gpio_put(motorDirL02, 0);
}
void move_forward(){
    printf("move_forward \n");
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/1 - dc_adjust_right);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/1 - dc_adjust_left);
    gpio_put(motorDirR01, 1);
    gpio_put(motorDirR02, 0);
    gpio_put(motorDirL01, 1);
    gpio_put(motorDirL02, 0);
}
void move_backward(){
    printf("move_backward \n");
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/1 - dc_adjust_right);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/1 - dc_adjust_left);
    gpio_put(motorDirR01, 0);
    gpio_put(motorDirR02, 1);
    gpio_put(motorDirL01, 0);
    gpio_put(motorDirL02, 1);
}
void move_foward_right(){
    printf("move_foward_right \n");
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/2);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/1);
    gpio_put(motorDirR01, 1);
    gpio_put(motorDirR02, 0);
    gpio_put(motorDirL01, 1);
    gpio_put(motorDirL02, 0);
}
void move_forward_left(){
    printf("move_forward_left \n");
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/1);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/2);
    gpio_put(motorDirR01, 1);
    gpio_put(motorDirR02, 0);
    gpio_put(motorDirL01, 1);
    gpio_put(motorDirL02, 0);
}
void move_backward_right(){
    printf("move_backward_right \n");
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/2);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/1);
    gpio_put(motorDirR01, 0);
    gpio_put(motorDirR02, 1);
    gpio_put(motorDirL01, 0);
    gpio_put(motorDirL02, 1);
}
void move_backward_left(){
    printf("move_backward_left \n");
    pwm_set_chan_level(slice_num01, PWM_CHAN_A, 12500/1);
    pwm_set_chan_level(slice_num02, PWM_CHAN_B, 12500/2);
    gpio_put(motorDirR01, 0);
    gpio_put(motorDirR02, 1);
    gpio_put(motorDirL01, 0);
    gpio_put(motorDirL02, 1);
}