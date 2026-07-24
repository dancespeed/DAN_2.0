#include "module.hpp"

namespace dan::core
{
    Module::~Module() = default;

    void Module::Initialize()
    {
        if (state != ModuleState::Stopped)
        {
            return;
        }

        if (!OnInitialize())
        {
            return;
        }

        state = ModuleState::Initialized;
    }

    void Module::Start()
    {
        if (state != ModuleState::Initialized)
        {
            return;
        }

        if (!OnStart())
        {
            return;
        }

        state = ModuleState::Running;
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

    ModuleState Module::GetState() const
    {
        return state;
    }
}