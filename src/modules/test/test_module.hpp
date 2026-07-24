#pragma once

#include "core/module/module.hpp"

namespace dan::modules
{
    class TestModule final : public core::Module
    {
    public:
        [[nodiscard]]
        core::ModuleId GetId() const override;

    protected:
        bool OnInitialize() override;
        bool OnStart() override;
        void OnRun() override;
        void OnStop() override;
    };
}