volatile uint8_t backlight_scaled = 210;  // scaled 0–210
volatile uint8_t pwm_counter = 0;         // 0–209 counter

static void pwm_brightness_setup(void){
    DDRD |= (1<<PD5); // PD5 output
    TCCR0A = (1<<COM0A0);    // toggle OC0A     
    TCCR0B = (1<<WGM02) | (1<<CS00); // CTC, no prescaler
    OCR0A = 210; // 38 kHz carrier for IR

    TIMSK0 = (1<<OCIE0A); // enable interrupt for software PWM
}

void backlight_set(uint8_t brightness){
    // map 0–255 to 0–210
    backlight_scaled = ((uint16_t)brightness * 210) / 255;
}

static void adc_setup(void){
    ADMUX  = (0<<MUX0); // ADC0 PC0
    ADMUX |= (1<<REFS0); // AVcc
    ADMUX |= (1<<ADLAR); // use only 8 bits for ADC

    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //enable: adc, interrupt, auto-trigger, prescaler 128 for 125kHz
    ADCSRA |= (1<<ADSC);//start adc measurements
}

ISR(ADC_vect){
    backlight_set(ADCH);
}

// Software PWM
ISR(TIMER0_COMPA_vect){
    pwm_counter++;
    if(pwm_counter >= 210)
        pwm_counter = 0;

    if(pwm_counter < backlight_scaled)
        PORTD |= (1<<PD5);
    else
        PORTD &= ~(1<<PD5);
}

void brightness_init(void){
    adc_setup();
    pwm_brightness_setup();
}
