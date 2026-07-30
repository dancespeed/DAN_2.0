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
    bool System::Initialize(
        GlobalId globalId,
        Module* const* modules,
        size_t moduleCount)
    {
        if (initialized)
        {
            return true;
        }

        platform::Platform::Initialize();

        if (!Runtime::Initialize(globalId, modules, moduleCount))
        {
            return false;
        }

        initialized = true;
        return true;
    }

    bool System::Start()
    {
        if (running)
        {
            return true;
        }

        if (!initialized)
        {
            return false;
        }

        platform::Platform::Start();

        if (!Runtime::Start())
        {
            platform::Platform::Stop();
            return false;
        }

        running = true;
        return true;
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
