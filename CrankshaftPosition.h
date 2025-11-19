//PistonHead2k
#ifndef CrankshaftPosition
#define CrankshaftPosition



namespace CrankshaftPosition
{

#include "std.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "USART.h"
#include "Timer.h"

#include "Math.h"

/* Synchronous Tone Wheel Sensor Decoder */
struct SynchronousToneWheel
{
    /*---Reluctor Configuration---*/

    /* Short Tooth Count */
    public: const uint16_t STC = 44;
    /* Long Tooth Count */
    public: const uint16_t LTC = 1;

    //----------------------------//



    /* CKP Frequency in Hz */
    public: uint16_t Frequency;

    /* Frequency Interrupt Register */
    private: uint16_t FIR;

    /* Returns CKP Frequency in Hz */
    private: uint16_t f_Frequency(uint8_t TimeStep)
    { 
        return FIR / TimeStep;
    }

    
    /* Time Between CKP Pulses */
    public: uint32_t DeltaT;

    /* Time Between Last CKP Pulse */
    private: uint32_t l_DeltaT;

    //Time Interrupt Register
    private: uint32_t TIR;

    /* Returns Time Between CKP Pulses */
    private: uint32_t f_DeltaT(void)
    {   
        //Time Interrupt Buffer
        uint32_t TIB = Micros();

        uint32_t DeltaT = TIB - TIR;
        TIR = TIB;

        return DeltaT;
    }

    /* Raw Revolutions Per Second */
    private: uint16_t rps;



    private: void f_Reluctor(void)
    {
        if (MathF::CmpMrg(DeltaT * 1.5f, l_DeltaT, 100)) 
        {
            rps++;
        }

    }

    //Returns Synchronous Tonewheel Position
    public: void Degrees()
    {
        return //TO BE CONTINUED
    }

    /* This Function Should Be Called In Every Interrupt Pulse From The Sensor */
    public: volatile void CaptureEvent(void)
    {
        //Counts CKP Pulses
        FIR++;
    
        //Time Between Pulses
        l_DeltaT = DeltaT;
        DeltaT = f_DeltaT();

        f_Reluctor();
    }

    /* Revolutions Per Second */
    public: uint16_t RPS;

    /* This Function Should Be Called Every 1 Second of Runtime */
    public: void TimeEvent(void)
    {
        Frequency = f_Frequency(1e6/1e6);
        FIR = 0;

        RPS = rps;
        rps = 0;
    }

};





/* CKP Sensor Object */
ToneWheel CKP;

/* CKP Interrupt Service Routine */
/* PORT D Pin 2 */
ISR (INT0_vect)
{
    CKP.CaptureEvent();
}



/* Setups The Crankshaft Position Sensor Input */
void SetInterrupt()
{
    //Sets PD2 as INT0 on Rising Edge
    EICRA |= INT0FALLING;

    //Enables Port D Pin 2 Interrupts
    EIMSK |= (ENABLE << INT0);

    //Enables All Interrupts
    sei();
}
    
/* CKP Time Event (1s) */
void CKPTE(void)
{
    CKP.TimeEvent();
}

/* Gets The Current Crankshaft Position */
void Loop(void)
{
    /* CKP Timer 0 */
    static Timer::Subroutine CKPT0;
    CKPT0.Wait(1e6, &CKPTE, Timer::Micros());

    
    
}



}

#endif