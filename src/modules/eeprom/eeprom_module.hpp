#pragma once

#include "core/message/message.hpp"
#include "core/module/module.hpp"
#include "core/module/module_ids.hpp"

namespace dan::modules
{
    class EepromModule final : public core::Module
    {
    public:
        inline static constexpr core::ModuleId Id =
            core::ModuleIds::Eeprom;

        [[nodiscard]]
        core::ModuleId GetId() const override;

        [[nodiscard]]
        const char* GetName() const override;

    protected:
        bool OnInitialize() override;
        bool OnStart() override;
        void OnRun() override;
        void OnStop() override;
        void OnReceive(const core::Message& message) override;
    };
}
