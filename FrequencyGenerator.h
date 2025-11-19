//Frequency Generator Lib By PistonHead2k
#ifndef FrequencyGenerator
#define FrequencyGenerator


/* Uses Timer 2 */
namespace FrequencyGenerator
{

#include <avr/io.h>
#include <avr/interrupt.h>

#define PORTD_3 3

//Interrupt Service Routine //491HZ (F*2)
ISR (TIMER2_OVF_vect)
{
    static uint16_t TOC;
    TOC++;

    switch (TOC) 
    {
        case 43:
        PORTD |= (1 << PORTD_3);
        TOC = 0;
        return;
    }

    PORTD ^= (1 << PORTD_3);
    TCNT2 = 255 - 255;



}

void Init()
{    
    //Ses PORTB bit 5 as Output
    DDRD |= (1 << PORTD_3); //means 0b00100000
    
    //Timer Overflow Flag to 0 -> Has a max value of 65535
    //Formula: T[s] = 65535 - (F_CPU/Prescaler * Ftarget)
    TCNT2 = 255 - 255;

    //Timer Control Register 2B -> Sets Prescaler to 256
    TCCR2B = (1 << CS22);

    TCCR2A = 0b00000000; //(0x00)

    //Timer Control Register 2 -> Timer Overflow Interrupt Enabled
    TIMSK2 = (1 << TOIE2);

    //Enables CPU Global Interrupted
    sei();
}

}

#endif