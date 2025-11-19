//Mathematics Functions (float) by PistonHead2k

#ifndef MathF
#define MathF

//NEEDS TO BE INCLUDED IN main.cpp 
#include <math.h>

/* Integer Mathematics Library */
namespace MathI
{

/* Returns The Module of |a| */
int abs(int a)
{
    int absint = (a < 0) ? -a : a;
    return absint;
}

/* Compare With Margin */
/* Compares 'a' against 'b' with a margin 'c' */
bool CmpMrg(int a, int b, int c)
{
    return abs(a - b) < c;
}

/* Compare With Margin */
/* Compares 'a' against 'b' with a margin 'c' */
bool CmpMrg(uint32_t a, uint32_t b, uint32_t c)
{
    return (a - b) < c;
}

}

/* Float Mathematics Library */
namespace MathF
{

/* Compare With Margin */
/* Compares 'a' against 'b' with a margin 'c' */
bool CmpMrg(float a, float b, float c)
{
    return fabsf(a - b) < c;
}

/* Clamps a value between a minimum float and maximum float value. */
float Clamp(float value, float min, float max)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
    return value;
}

/* Returns largest of two values. */
float Max(float a, float b) 
{
    return a > b ? a : b; 
}

/* Gradually changes a value towards a desired goal over time. */
/* WARNING: SLOW AS HELL */
/*float SmoothDamp(float Current, float Target, float SmoothTime, float MaxSpeed = INFINITY, float DeltaTime = Timer::ProgramTime)
{   

            static float CurrentVelocity;

            //Based on Game Programming Gems 4 Chapter 1.10
            SmoothTime = MathF::Max(0.0001f, SmoothTime);
            float omega = 2.0f / SmoothTime;

            float x = omega * DeltaTime;
            float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
            float change = Current - Target;
            float originalTo = Target;

            // Clamp maximum speed
            float maxChange = MaxSpeed * SmoothTime;
            change = MathF::Clamp(change, -maxChange, maxChange);
            Target = Current - change;

            float temp = (CurrentVelocity + omega * change) * DeltaTime;
            CurrentVelocity = (CurrentVelocity - omega * temp) * exp;
            float output = Target + (change + temp) * exp;

            // Prevent overshooting
            if (originalTo - Current > 0.0f == output > originalTo)
            {
                output = originalTo;
                CurrentVelocity = (output - originalTo) / DeltaTime;
            }

            return output;
}*/

}


#endif