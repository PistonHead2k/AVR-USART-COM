//Pastebin for functions and stuff

struct ToneWheel
{
    /* Stores The Last Time an Interrupt has been Fired */
    protected: uint32_t LastTimeRef;

    /* Time Between Interrupts */
    public: uint32_t DeltaT;

    /* Time Between Last Interrupts */
    protected: uint32_t LastDeltaT;

    /* Counts The Amount Of Interrupts */
    protected: volatile uint8_t IntCount;

    protected: volatile bool MissingTooth;

    /* Time Differential Between Crank Pulses [us] */
    /* TimePulses()->DeltaT */
    public: void TimePulses(uint32_t TimeRef)
    {
        LastDeltaT = DeltaT;

        DeltaT = TimeRef - LastTimeRef;
        LastTimeRef = TimeRef;
    }


    /* Synchronizes ToneWheel With Crank Position */
    public: bool Sync()
    {
        switch (MathI::CmpMrg(DeltaT, LastDeltaT - DeltaT, uint32_t(100)))
        { 
            case true: 
            return true;
            break;
            default: return false;
        }
        return false;
    }
};