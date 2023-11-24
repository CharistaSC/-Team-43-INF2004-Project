#define echpin 0
#define trgpin 1
#define encoderRightPin 6
#define encoderLeftPin 7

// Constants
#define DISTANCE_PER_PULSE 1.05  // Per divit in the wheel encoder. 21 cm diameter and 20 divits in the wheel encoder
#define encoder_timer_distance 5000 // 10 seconds

volatile uint32_t pulse_count = 0;
volatile uint64_t encoder_prev_time = 0;
volatile uint64_t timer_start_time = 0;
volatile bool timer_enabled = false;
int timeout = 24000;
absolute_time_t startTime, endTime;

void ultra_encoder_init(){
    gpio_init(trgpin);
    gpio_init(echpin);
    gpio_init(encoderLeftPin);
    gpio_init(encoderRightPin);
    gpio_set_dir(trgpin, GPIO_OUT);
    gpio_set_dir(echpin, GPIO_IN);
    gpio_set_dir(encoderLeftPin, GPIO_IN);
    gpio_set_dir(encoderRightPin, GPIO_IN);
}

float ultrasonic_send(){
    gpio_put(trgpin, 0);
    sleep_us(10);
    gpio_put(trgpin, 1);
    sleep_us(10);
    gpio_put(trgpin, 0);
    uint64_t width = 0;
    // Wait for the pulse to return or timeout
    uint32_t start_time = time_us_32();
    while ((gpio_get(echpin) == 0) && (time_us_32() - start_time < 150000)) {}
    if (gpio_get(echpin) == 0) {
        // Timeout, return a large value to indicate no echo received
        printf("no trigger recieved\n");
    }
    else if(gpio_get(echpin) == 1){
        startTime = get_absolute_time();
        while (gpio_get(echpin) == 1)
        {
            width++;
            sleep_us(1);
            if (width > timeout)
                return 0;
        }
        endTime = get_absolute_time();
        // Calculate distance based on the time it took for the pulse to return
        uint64_t pulseLength = absolute_time_diff_us(startTime, endTime);  // Convert to seconds
        float distance_cm = (float)pulseLength * 0.01715;
        printf("The distance is %.2f cm\n", distance_cm);
        return distance_cm;
    }
}

void encoder_distance(){
    if (timer_enabled) {
        uint64_t encoder_current_time = time_us_64();
        uint64_t time_elapsed = encoder_current_time - timer_start_time;

        // If 10 seconds have passed without an interrupt, disable the counter
        if (time_elapsed >= encoder_timer_distance * 1000) {
            float total_distance = pulse_count * DISTANCE_PER_PULSE;
            printf("Total Distance: %.2f cm\n", total_distance);

            // Reset pulse count and disable the timer
            pulse_count = 0;
            timer_enabled = false;
        }
    }
}
