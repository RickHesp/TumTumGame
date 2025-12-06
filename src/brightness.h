#include <avr/io.h>
#include <avr/interrupt.h>

#define BACKLIGHT DDD5

// Backlight PWM variables
volatile uint8_t backlight_counter = 0;
volatile uint8_t backlight_threshold = 255; // 0-255 brightness

void init_backlight() {
    DDRD |= (1 << BACKLIGHT); // PD5
    
    TIMSK0 |= (1<<OCIE0A);
}

// Timer0 Compare A interrupt; runs at 76kHz
ISR(TIMER0_COMPA_vect)
{
    // Software PWM for backlight
    backlight_counter++;
    if (backlight_counter >= OCR0A) { // Reset at OCR0A value
        backlight_counter = 0;
    }
    
    if (backlight_counter < backlight_threshold) {
        PORTD |= (1 << BACKLIGHT);  // Backlight ON
    } else {
        PORTD &= ~(1 << BACKLIGHT); // Backlight OFF
    }
}

void backlight_set(uint8_t brightness)
{
    backlight_threshold = brightness;
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
    init_backlight();
}