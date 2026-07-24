#include "system.hpp"
#include "system_config.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

namespace
{
    volatile uint32_t tickMs = 0;
}

ISR(TIMER2_COMPA_vect)
{
    tickMs++;
}

namespace System
{
    void Init()
    {
        // PC1 — выход диагностического heartbeat.
        DDRC |=
            (1 << SystemConfig::HeartbeatBit);

        // Начальное состояние heartbeat — LOW.
        PORTC &=
            ~(1 << SystemConfig::HeartbeatBit);

        // Timer2:
        // CTC mode;
        // период системного тика — 1 мс.
        TCCR2A = 0;
        TCCR2B = 0;
        TCNT2 = 0;

        OCR2A =
            SystemConfig::Timer2CompareValue;

        // CTC mode: сброс Timer2 при совпадении с OCR2A.
        TCCR2A |=
            (1 << WGM21);

        // Предделитель Timer2 = 64.
        TCCR2B |=
            (1 << CS22);

        // Разрешаем прерывание Compare Match A.
        TIMSK2 |=
            (1 << OCIE2A);

        sei();
    }

    uint32_t GetTickMs()
    {
        uint32_t value;

        const uint8_t sreg = SREG;

        cli();

        value = tickMs;

        SREG = sreg;

        return value;
    }
}