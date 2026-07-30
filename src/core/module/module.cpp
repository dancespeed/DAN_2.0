#include "module.hpp"

namespace dan::core
{
    Module::~Module() = default;

    bool Module::Initialize()
    {
        if (state == ModuleState::Initialized)
        {
            return true;
        }

        if (state != ModuleState::Stopped)
        {
            return false;
        }

        if (!OnInitialize())
        {
            return false;
        }

        state = ModuleState::Initialized;
        return true;
    }

    bool Module::Start()
    {
        if (state == ModuleState::Running)
        {
            return true;
        }

        if (state != ModuleState::Initialized)
        {
            return false;
        }

        if (!OnStart())
        {
            return false;
        }

        state = ModuleState::Running;
        return true;
    }

    void Module::Run()
    {
        if (state != ModuleState::Running)
        {
            return;
        }

        OnRun();
    }

    void Module::Stop()
    {
        if (state != ModuleState::Running)
        {
            return;
        }

        OnStop();

        state = ModuleState::Stopped;
    }

    void Module::Receive(const Message& message)
    {
        OnReceive(message);
    }

    ModuleState Module::GetState() const
    {
        return state;
    }
}
