//Mateo Proruk
#ifndef IO
#define IO

namespace Bit
{   
    #include <avr/io.h>
    /* Sets Specific Bit in the 8 byte register Port */
    void Set(volatile uint8_t* PORT, uint8_t Shift)
    {
        *PORT |= (1 << Shift); 
    }

    /* Reset Specific Bit in the 8 byte register Port */
    void Reset(volatile uint8_t* PORT, uint8_t Shift)
    {
        *PORT &= ~(1 << Shift); 
    }

    /* Toggle Specific Bit in the 8 byte register Port */
    void Toggle(volatile uint8_t* PORT, uint8_t Shift)
    {
        *PORT ^= (1 << Shift); 
    }

    /* Copy a Specific Bit in the 8 byte register Port */
    void Out(volatile uint8_t* PORT, uint8_t Shift, bool Equals)
    {
        *PORT = (*PORT & ~(1 << Shift)) | (Equals << Shift);
    }

};

/* Dependencys: Timer.h */
namespace IO
{

/* Bit Type */
#define bit bool

/* Port Output Class */
struct PORT
{

//PORTB
 bit B0; 
 bit B1; 
 bit B2; 
 bit B3; 
 bit B4; 
 bit B5; 
 //XTAL
 bit B6; 
 //XTAL
 bit B7; 

//PORTC
 bit C0; 
 bit C1; 
 bit C2; 
 bit C3; 
 bit C4; 
 bit C5; 
 /* RESET */
 bit C6; 
 /* Unused */
 bit C7;

//PORTD
 bit D0; 
 bit D1; 
 bit D2; 
 bit D3; 
 bit D4; 
 bit D5; 
 bit D6; 
 bit D7;

/* Copy Bits To PORT Register */
public: void BitsToRegister()
{
    Bit::Out(&PORTB, 0, B0);
    Bit::Out(&PORTB, 1, B1);
    Bit::Out(&PORTB, 2, B2);
    Bit::Out(&PORTB, 3, B3);
    Bit::Out(&PORTB, 4, B4);
    Bit::Out(&PORTB, 5, B5);
    Bit::Out(&PORTB, 6, B6);
    Bit::Out(&PORTB, 7, B7);

    Bit::Out(&PORTC, 0, C0);
    Bit::Out(&PORTC, 1, C1);
    Bit::Out(&PORTC, 2, C2);
    Bit::Out(&PORTC, 3, C3);
    Bit::Out(&PORTC, 4, C4);
    Bit::Out(&PORTC, 5, C5);
    Bit::Out(&PORTC, 6, C6);
    Bit::Out(&PORTC, 7, C7);

    Bit::Out(&PORTD, 0, D0);
    Bit::Out(&PORTD, 1, D1);
    Bit::Out(&PORTD, 2, D2);
    Bit::Out(&PORTD, 3, D3);
    Bit::Out(&PORTD, 4, D4);
    Bit::Out(&PORTD, 5, D5);
    Bit::Out(&PORTD, 6, D6);
    Bit::Out(&PORTD, 7, D7);
}

};

static IO::PORT P;

#include "Timer.h"
struct Debounce
{

    #define ON 1
    #define OFF 0

    /* Time wich the input has to be at the same logic level */
    public: double Time = 0.1;
    
    private: bool Actual;
    
    /* Debounce Input Toggle Class */
    public: bool InputT(uint8_t PIN, uint8_t PINNumber, double TimeRef = Timer::Seconds())
    {   
        bool LastCycle = Actual;
        Actual = OnFallingEdge(PIN, PINNumber);//(PIN & ON << PINNumber) >> PINNumber;
        
        bool Out;

        if (LastCycle && !Actual)
        {
            /* Toggle (XOR) */
            #define TOGGLE ^= 1
            Out TOGGLE;
        }

        return Out;


    }

    /* Debounce Input Falling Edge Class */
    public: bool Input(uint8_t PIN, uint8_t PINNumber, double TimeRef = Timer::Seconds())
    {   
        bool LastCycle = Actual;
        Actual = OnFallingEdge(PIN, PINNumber);//(PIN & ON << PINNumber) >> PINNumber;
        
        bool Out = OFF;

        if (LastCycle && !Actual)
        {
            Out = ON;
        }

        return Out;
    }

    /* Debounce Input Rising Edge Class */
    public: bool InputR(uint8_t PIN, uint8_t PINNumber, double TimeRef = Timer::Seconds())
    {   
        bool LastCycle = Actual;
        Actual = OnRisingEdge(PIN, PINNumber);//(PIN & ON << PINNumber) >> PINNumber;
        
        bool Out = OFF;

        if (LastCycle && !Actual)
        {
            Out = ON;
        }

        return Out;
    }

    private: double TimeElapsedR;
    /* Debounce Input Rising Edge Class */
    public: bool OnRisingEdge(uint8_t PIN, uint8_t PINNumber, double TimeRef = Timer::Seconds())
    {   
        bool Actual = (PIN & ON << PINNumber) >> PINNumber;
        
        bool Out;

        if (Actual) 
        {
            TimeElapsedR = TimeRef;
            Out = OFF;
        }

        //fabs is for Overflow Protection
        if (fabs(TimeRef - TimeElapsedR) > Time)
        {
            Out = ON;
        }

 

        return Out;
    }

    private: double TimeElapsedF;
    /* Debounce Input Falling Edge Class */
    public: bool OnFallingEdge(uint8_t PIN, uint8_t PINNumber, double TimeRef = Timer::Seconds())
    {   
        bool Actual = (PIN & ON << PINNumber) >> PINNumber;
        
        bool Out;

        if (Actual)
        {
            TimeElapsedF = TimeRef;
            Out = ON;
        }

        //fabs is for Overflow Protection
        if (fabs(TimeRef - TimeElapsedF) > Time)
        {
            Out = OFF;
        }
     

        return Out;
    }
};

};

/* Electric Functions */
namespace Electric
{
/* Ouptuts one cycle pulse for a given input */
struct Pulse
{   

#define bit bool
#define flag bit

    
    private: flag ActualF;
    /* Ouptuts one cycle pulse at the Falling Edge for a given input */
    public: inline bit Falling(bit In)
    {
       bool LastCycle = ActualF;
        ActualF = !In;
        bit Out = OFF;
        
        Out = (!ActualF & LastCycle); //If i want to out 2 cycles instead of 1: Out = Actual ^ LastCycle;
     
        if (Out) USART::Debug::SendString("ON");

        return Out;
    }

    private: flag ActualR;
    /* Ouptuts one cycle pulse at the Rising Edge for a given input */
    public: bit Rising(bit In)
    {
        bool LastCycle = ActualR;
        ActualR = In;
        bit Out = OFF;
        
        Out = (!ActualR & LastCycle); //If i want to out 2 cycles instead of 1: Out = Actual ^ LastCycle;
     
        if (Out) USART::Debug::SendString("ON");

        return Out;
    }
    

};
};

#endif