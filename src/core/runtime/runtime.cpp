#include "runtime.hpp"

namespace
{
    bool initialized = false;
    bool running = false;
}

namespace dan::core
{
    void Runtime::Initialize()
    {
        if (initialized)
        {
            return;
        }

        /*
         * Здесь позже будут:
         *
         * Configuration
         * ModuleTable
         * MessageBus
         * Dispatcher
         */

        initialized = true;
    }

    void Runtime::Start()
    {
        if (!initialized || running)
        {
            return;
        }

        /*
         * Запуск Runtime.
         */

        running = true;
    }

    void Runtime::Run()
    {
        if (!running)
        {
            return;
        }

        /*
         * Позже здесь появится:
         *
         * MessageBus::Process();
         * Dispatcher::Dispatch();
         */
    }

    void Runtime::Stop()
    {
        if (!running)
        {
            return;
        }

        running = false;
    }
}