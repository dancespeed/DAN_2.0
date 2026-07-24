#include "interrupt.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

namespace dan::platform
{
    void Interrupt::Enable()
    {
        sei();
    }

    void Interrupt::Disable()
    {
        cli();
    }

    Interrupt::State Interrupt::SaveAndDisable()
    {
        const State state = SREG;

        cli();

        return state;
    }

    void Interrupt::Restore(const State state)
    {
        SREG = state;
    }
}