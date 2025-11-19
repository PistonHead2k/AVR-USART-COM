//PistonHead2k
#ifndef Timer
#define Timer


/* Standard Timer Library. Uses Timer/Counter0 */
namespace Timer
{

#include <avr/interrupt.h>

/* Timer 0 Calibration */
#define T0_CAL 0

/* Time Tick Interval */
const double TimerFrequency = double(F_CPU) / (255 + T0_CAL) / 64.0; //F_CPU / TimerOverflow[256] / TimerPrescaler[64] = 976.5625Hz
const double TimeConstant = 1.0 / TimerFrequency;

/* Counts The Number Of Times The Interrupt Has Been Fired */
/* Note: Overflows in 49 days */
volatile uint32_t OverflowCount;


/* Executes Every 0.001024 seconds */
void (*TIMER0_OVF_INT)(void) = nullptr;

ISR (TIMER0_OVF_vect) //Gets Called Every 0.001024 seconds
{
    OverflowCount++;
    TCNT0 = 0x00;

    /* Fixed Loop */
    TIMER0_OVF_INT();
}

/* Current Program Time After The Last Restart in Microseconds */
/* Note: Updates Every 4us */
/* Note: Overflows in 70 minutes */
uint32_t Micros()
{
    return OverflowCount * TimeConstant * 1e6 + TCNT0 * 4;
}

/* Current Program Time After The Last Restart in Miliseconds */
/* Unaccurate */
/* Note: Updates Every 1.024ms */
/* Note: Overflows in x minutes */
/*uint32_t Millis()
{
    return OverflowCount * TimeConstant * 1e3;
}*/

/* Current Program Time After The Last Restart in Seconds */
/* Note: Overflows in 49 days */
double Seconds()
{
    return OverflowCount * TimeConstant + TCNT0 * 4e-6;
}

/* Current Execution Time Of The Program in Seconds */
volatile double DeltaTime;

/* Calculates Loop Execution Time in Microseconds */
void DeltaT(uint32_t TimeRef)
{
    static uint32_t LastProgramTime;
    DeltaTime = TimeRef - LastProgramTime;
    LastProgramTime = TimeRef;
}

void Start()
{
    //Timer Overflow Flag to 0 -> Has a max value of 255
    //Formula: T[s] = 255 - (F_CPU/Prescaler * Ftarget)
    TCNT0 = 0x00;

    //Timer Control Register 1B -> Sets Prescaler to 8
    TCCR0B = (1 << CS01 | 1 << CS00);

    TCCR0A = 0b00000000; //(0x00)

    //Timer Control Register 1 -> Timer Overflow Interrupt Enabled and Input Capure Interrupt
    TIMSK0 = (1 << TOIE0);

    //Enables CPU to be Interrupted
    sei();
}

void Stop()
{
    //Timer Control Register 1B -> Sets Prescaler to No Clock Source
    //Clk/8;
    TCCR0B = (0 << CS00 & 0 << CS01 & 0 << CS02);

    TCCR0A = 0b00000000; //(0x00)

    //Timer Control Register 1 -> Timer Overflow Interrupt Enabled and Input Capure Interrupt
    TIMSK0 = (0 << TOIE0);
}

/* Subroutine Class */
struct Subroutine
{
    private: double TimeElapsed;
    /* Waits The Specified Time In Seconds */
    public: void Wait(double Time, void (*Subroutine)(void) = nullptr, double TimeRef = Timer::Seconds())
    {
        //fabs is for Overflow Protection
        if (fabs(TimeRef - TimeElapsed) > Time)
        {
            TimeElapsed = TimeRef;
            Subroutine();
        }
    }
};

}

#endif