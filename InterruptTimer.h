// PistonHead2k
#ifndef InterruptTimer
#define InterruptTimer


namespace InterruptTimer
{
    
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_BUILTIN 5

//Interrupt Service Routine 
ISR (TIMER1_OVF_vect)
{
    PORTB ^= (1 << LED_BUILTIN);
    TCNT1 = 65535 - (F_CPU/1024);
}

void Init()
{    
    //Ses PORTB bit 5 as Output
    DDRB |= (1 << LED_BUILTIN); //means 0b00100000
    
    //Timer Overflow Flag to 0 -> Has a max value of 65535
    //Formula: T[s] = 65535 - (F_CPU/Prescaler * Ftarget)
    TCNT1 = 65535 - (F_CPU/1024);

    //Timer Control Register 1B -> Sets Prescaler to 1024
    // FCPU[16M]/1024 = 15625Hz
    TCCR1B = (1 << CS10) | (1 << CS12);

    TCCR1A = 0b00000000; //(0x00)

    //Timer Control Register 1 -> Timer Overflow Interrupt Enabled
    TIMSK1 = (1 << TOIE1);

    //Enables CPU to be Interrupted
    sei();
}
}
#endif