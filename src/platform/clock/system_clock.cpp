#include "system_clock.hpp"
#include "system_clock_config.hpp"

#include "platform/interrupt/interrupt.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

namespace
{
    volatile uint32_t tickMs = 0;
}

ISR(TIMER2_COMPA_vect)
{
    ++tickMs;
}

namespace dan::platform
{
    void SystemClock::Initialize()
    {
        const Interrupt::State interruptState =
            Interrupt::SaveAndDisable();

        tickMs = 0;

        /*
         * Останавливаем Timer2 и сбрасываем его конфигурацию.
         */
        TCCR2A = 0;
        TCCR2B = 0;
        TCNT2 = 0;

        /*
         * Timer2 работает в режиме CTC.
         * Счётчик сбрасывается при совпадении с OCR2A.
         */
        TCCR2A =
            static_cast<uint8_t>(1U << WGM21);

        OCR2A =
            static_cast<uint8_t>(
                SystemClockConfig::TimerCompareValue);

        /*
         * Прерывание и источник тактирования пока выключены.
         * Фактический запуск выполняется методом Start().
         */
        TIMSK2 &=
            static_cast<uint8_t>(~(1U << OCIE2A));

        TCCR2B &=
            static_cast<uint8_t>(
                ~((1U << CS22) |
                  (1U << CS21) |
                  (1U << CS20)));

        /*
         * Сбрасываем возможный ожидающий флаг Compare Match A.
         * Флаги Timer2 очищаются записью единицы.
         */
        TIFR2 =
            static_cast<uint8_t>(1U << OCF2A);

        Interrupt::Restore(interruptState);
    }

    void SystemClock::Start()
    {
        const Interrupt::State interruptState =
            Interrupt::SaveAndDisable();

        TCNT2 = 0;

        /*
         * Сбрасываем ожидающее событие перед включением
         * прерывания.
         */
        TIFR2 =
            static_cast<uint8_t>(1U << OCF2A);

        /*
         * Разрешаем Compare Match A.
         */
        TIMSK2 |=
            static_cast<uint8_t>(1U << OCIE2A);

        /*
         * Предделитель Timer2 = 64:
         *
         * CS22 = 1
         * CS21 = 0
         * CS20 = 0
         */
        TCCR2B =
            static_cast<uint8_t>(
                (TCCR2B &
                 ~((1U << CS22) |
                   (1U << CS21) |
                   (1U << CS20))) |
                (1U << CS22));

        Interrupt::Restore(interruptState);
    }

    void SystemClock::Stop()
    {
        const Interrupt::State interruptState =
            Interrupt::SaveAndDisable();

        /*
         * Запрещаем прерывание Timer2.
         */
        TIMSK2 &=
            static_cast<uint8_t>(~(1U << OCIE2A));

        /*
         * Удаление источника тактирования останавливает Timer2.
         */
        TCCR2B &=
            static_cast<uint8_t>(
                ~((1U << CS22) |
                  (1U << CS21) |
                  (1U << CS20)));

        Interrupt::Restore(interruptState);
    }

    uint32_t SystemClock::GetTickMs()
    {
        const Interrupt::State interruptState =
            Interrupt::SaveAndDisable();

        const uint32_t value = tickMs;

        Interrupt::Restore(interruptState);

        return value;
    }
}