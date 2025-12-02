#include <avr/io.h>
#include <avr/interrupt.h>

static void pwm_brightness_setup(void){
    //PD5 as output
    DDRD |= (1<<PD5);
    //fast PWM mode.
    TCCR2A |= (1 << WGM20) | (1 << WGM21);
    TCCR2B &= ~(1 << WGM22);
    //non-inverting mode on OC2B(PD5)
    TCCR2A |= (1 << COM2B1);
    TCCR2A &= ~(1 << COM2B0);
    //no prescaler
    TCCR2B |= (0 << CS21) | (1 << CS20); 
    OCR2B = 255;
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
    sei();
}

ISR(ADC_vect){  
    uint8_t brightness = ADC;
    backlight_set(brightness);
}

void brightness_init(void){
    adc_setup();
    pwm_brightness_setup();
}

