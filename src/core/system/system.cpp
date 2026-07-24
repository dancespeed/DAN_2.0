#include "system.hpp"

#include "platform/clock/system_clock.hpp"
#include "platform/diagnostic/heartbeat.hpp"
#include "platform/interrupt/interrupt.hpp"

namespace
{
    bool initialized = false;
    bool running = false;
}

namespace dan::core
{
    void System::Initialize()
    {
        if (initialized)
        {
            return;
        }

        platform::SystemClock::Initialize();
        platform::Heartbeat::Initialize();

        initialized = true;
    }

    void System::Start()
    {
        if (!initialized || running)
        {
            return;
        }

        platform::SystemClock::Start();
        platform::Interrupt::Enable();

        running = true;
    }

    void System::Run()
    {
        if (!running)
        {
            return;
        }

        const uint32_t currentTickMs =
            platform::SystemClock::GetTickMs();

        platform::Heartbeat::Update(currentTickMs);

        /*
         * В дальнейшем здесь появится вызов обработки
         * очереди сообщений:
         *
         * MessageBus::Process();
         *
         * Пока транспорт на этом этапе не изменяем.
         */
    }

    void System::Stop()
    {
        if (!running)
        {
            return;
        }

        platform::Interrupt::Disable();

        platform::Heartbeat::Stop();
        platform::SystemClock::Stop();

        running = false;
    }
}