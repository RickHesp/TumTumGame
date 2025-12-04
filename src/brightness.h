static void pwm_brightness_setup(void){
    DDRD |= (1<<PD5); // PD5 output
    TCCR0A |= (1<<COM0B1) | (1<<WGM00) | (1<<WGM01); // Non-inverting PWM on OC0B, Fast PWM mode
    TCCR0B = (1<<CS00); // No prescaler
    OCR0B = 255; // Start with full brightness
}

void backlight_set(uint8_t brightness)
{
    OCR0B = brightness;
}

static void adc_setup(void){
    ADMUX  = (0<<MUX0);//ADC channel 0(PC0).
    ADMUX = (1<<REFS0);//AVcc as reference.
    ADMUX |= (1<<ADLAR);//use only 8 bits for ADC

    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);//enable: adc, interrupt, auto-trigger, prescaler 128 for 125kHz
    ADCSRA |= (1<<ADSC);//start adc measurements
}

ISR(ADC_vect){  
    uint8_t brightness = ADCH;
    backlight_set(brightness);
}

void brightness_init(void){
    adc_setup();
    pwm_brightness_setup();
}

