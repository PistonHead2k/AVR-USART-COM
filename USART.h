// PistonHead2k
#ifndef USART
#define USART


/* Serial Data Manipulation Library */
namespace USART
{

#include <string.h>
#include <avr/io.h>
#include <math.h>

/* Initializes USART0 as Serial Transmission Port */
void Init(uint32_t BAUDRATE)
{
    // Baudrate Prescalar (see Page 146 Table 19-1)
    uint8_t UBRR = (((F_CPU / (BAUDRATE * 16UL))) - 1);

    // Set baud rate
    UBRR0H = (uint8_t)(UBRR >> 8);
    UBRR0L = (uint8_t)UBRR;

    // Set Frame Format to 8 Data Bits, no Parity, 1 Stop Bit
    //UCSR0C |= (1 << UCSZ01)|(1 << UCSZ00);

    /* Set frame format: 8data, 2stop bit, Odd Parity */
    UCSR0C = (1 << USBS0) | ( 1 << UCSZ00) | ( 1 << UCSZ01) | ( 1 << UPM00) | ( 1 << UPM01);

    

    // Enable Transmission and Reception
    UCSR0B |= (1 << RXEN0)|(1 << TXEN0);

    // Double Speed Operation
    UCSR0A = U2X0;
}

/* Sends a Byte */
void SendByte(uint8_t byte)
{
    // Halts Execution While Previous Byte is Completed
    while(!(UCSR0A&(1 << UDRE0))){asm("");};

    // Transmit data
    UDR0 = byte;
}

/* Halts Execution Until it Receives a Byte */
uint8_t PollByte()
{
    // Halts Execution for byte to be received
    while(!(UCSR0A&(1 << RXC0))){asm("");};

    // Return received data
    return UDR0;
}

#if defined(DEBUG)
/* Debug not for Release */
namespace Debug
{

/* Sends a String of Bytes */
void SendString(const char* String)
{
    // Halts Execution While Previous Byte is Completed
    while(!(UCSR0A&(1 << UDRE0))){asm("");};

    for (uint16_t i = 0; i < strlen(String); i++)
    {
        while(!(UCSR0A&(1 << UDRE0))){asm("");};

        UDR0 = (uint8_t)String[i];
    }
}

#endif

}
}

#if defined(DEBUG)
#include <stdio.h>

/* Casts Signed 32-bit int to string */
const char* ToString(int32_t Value)
{
    static char Buffer[32];
    memset(Buffer, 0, sizeof(Buffer[0]) * 32); // Clear String
    sprintf(Buffer, "Value: %li\n", Value);
    return Buffer;
}

/* Casts Unsigned 32-bit int to string */
const char* ToString(uint32_t Value)
{
    static char Buffer[32];
    memset(Buffer, 0, sizeof(Buffer[0]) * 32); // Clear String
    sprintf(Buffer, "Value: %lu\n", Value);
    return Buffer;
}

/* Casts Signed 32-bit float to string */
const char* ToString(float Value)
{
    static char Buffer[32];
    memset(Buffer, 0, sizeof(Buffer[0]) * 32); // Clear String
    
    const char* tmpSign = (Value < 0) ? "-" : ""; //Sign (+ or -) 
    float tmpVal = (Value < 0) ? -Value : Value;

    uint32_t tmpInt1 = tmpVal;                  // Get the integer (678).
    float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
    uint32_t tmpInt2 = trunc(tmpFrac * 1e4);  // Turn into integer (123).

    sprintf(Buffer, "Value: %s%lu.%04lu\n", tmpSign, tmpInt1, tmpInt2);

    return Buffer;
}

/* Casts Signed 64-bit float to string */
const char* ToString(double Value)
{
    static char Buffer[32];
    memset(Buffer, 0, sizeof(Buffer[0]) * 32); // Clear String
    
    const char* tmpSign = (Value < 0) ? "-" : ""; //Sign (+ or -) 
    double tmpVal = (Value < 0) ? -Value : Value;

    uint32_t tmpInt1 = tmpVal;                  // Get the integer (678).
    double tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
    uint32_t tmpInt2 = trunc(tmpFrac * 1e8);  // Turn into integer (123).

    sprintf(Buffer, "Value: %s%lu.%08lu\n", tmpSign, tmpInt1, tmpInt2);

    return Buffer;
}
#endif

#if defined(DEBUG)
#include <stdio.h>


/* Casts Unsigned 8-bit integer to string in binary representation */
const char* ToStringBinary(uint8_t Value)
{
    static char Buffer[9];
    memset(Buffer, 0, sizeof(Buffer[0]) * 9); // Clear String
    
    for (int i = 7; i >= 0; i--)
    {
        if ((Value >> i) & 1) Buffer[i] = *"1";
        else Buffer[i] = *"0";
    }
    
    Buffer[8] = *"\n";
    Buffer[9] = *"\0";
    return Buffer;
}
#endif

#endif