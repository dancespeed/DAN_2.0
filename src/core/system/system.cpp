#include "system.hpp"

#include "core/runtime/runtime.hpp"
#include "platform/platform.hpp"

namespace
{
    bool initialized = false;
    bool running = false;
}

namespace dan::core
{
    void System::Initialize(GlobalId globalId)
    {
        if (initialized)
        {
            return;
        }

        platform::Platform::Initialize();
        Runtime::Initialize(globalId);

        initialized = true;
    }

    void System::Start()
    {
        if (!initialized || running)
        {
            return;
        }

        platform::Platform::Start();
        Runtime::Start();

        running = true;
    }

    void System::Run()
    {
        if (!running)
        {
            return;
        }

        platform::Platform::Run();
        Runtime::Run();
    }

    void System::Stop()
    {
        if (!running)
        {
            return;
        }

        Runtime::Stop();
        platform::Platform::Stop();

        running = false;
    }
}
