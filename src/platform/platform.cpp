#include "platform.hpp"

#include "platform/clock/system_clock.hpp"
#include "platform/diagnostic/heartbeat.hpp"
#include "platform/interrupt/interrupt.hpp"

#include <stdint.h>

namespace dan::platform
{
    void Platform::Initialize()
    {
        /*
         * Во время подготовки платформы глобальные
         * прерывания должны оставаться выключенными.
         */
        Interrupt::Disable();

        SystemClock::Initialize();
        Heartbeat::Initialize();
    }

    void Platform::Start()
    {
        /*
         * Сначала запускаются аппаратные службы.
         * После этого разрешаются глобальные прерывания.
         */
        SystemClock::Start();

        Interrupt::Enable();
    }

    void Platform::Run()
    {
        const uint32_t currentTickMs =
            SystemClock::GetTickMs();

        Heartbeat::Update(currentTickMs);
    }

    void Platform::Stop()
    {
        /*
         * Запрещаем прерывания до остановки
         * аппаратных служб.
         */
        Interrupt::Disable();

        Heartbeat::Stop();
        SystemClock::Stop();
    }
}