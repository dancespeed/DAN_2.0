#pragma once

#include "core/module/module_id.hpp"
#include "core/module/module_state.hpp"

namespace dan::core
{
    class Module
    {
    public:
        virtual ~Module();

        void Initialize();
        void Start();
        void Run();
        void Stop();

        [[nodiscard]]
        virtual ModuleId GetId() const = 0;

        [[nodiscard]]
        ModuleState GetState() const;

    protected:
        Module() = default;

        virtual bool OnInitialize() = 0;
        virtual bool OnStart() = 0;
        virtual void OnRun() = 0;
        virtual void OnStop() = 0;

    private:
        ModuleState state = ModuleState::Stopped;
    };
}