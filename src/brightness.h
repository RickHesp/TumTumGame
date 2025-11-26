#include <avr/io.h>
#include <avr/interrupt.h>

static void pwm_brightness_setup(void){
    //PD5 as output
    DDRD |= (1<<PD5);

    // Timer0: Fast PWM, 8-bit, TOP = 0xFF
    // WGM01:0 = 0b11 (Fast PWM)
    TCCR0A |= (1 << WGM00) | (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // Non-inverting mode on OC0B (clear on compare match, set at BOTTOM)
    // COM0B1:0 = 0b10
    TCCR0A |= (1 << COM0B1);
    TCCR0A &= ~(1 << COM0B0);

    // Clock select: prescaler = 64 → ~976 Hz at 16 MHz
    // CS02:0 = 0b011 (clk/64)
    TCCR0B |= (0 << CS01) | (1 << CS00); 
    OCR0B = 255;
}

void backlight_set(uint8_t brightness)
{
    OCR0B = brightness;
}

static void adc_setup(void){
    ADMUX  = (0<<MUX0);//ADC channel 0(PC0).
    ADMUX = (1<<REFS0);//AVcc as reference.

    ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);//enable: adc, interrupt, auto-trigger, prescaler 128 for 125kHz
    ADCSRA |= (1<<ADSC);//start adc measurements
    sei();
}

ISR(ADC_vect){
    uint16_t pot = ADC;//value up to 1023
    uint8_t brightness = (uint8_t)(pot / 4);//convert to 255 max
    backlight_set(brightness);
}

void brightness_init(void){
    adc_setup();
    pwm_brightness_setup();
}

