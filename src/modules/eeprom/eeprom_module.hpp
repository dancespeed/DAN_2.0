#pragma once

#include <stdint.h>

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

        bool Read(
            uint16_t address,
            uint8_t* buffer,
            uint16_t size
        );
        bool Write(
            uint16_t address,
            const uint8_t* data,
            uint16_t size
        );

    protected:
        bool OnInitialize() override;
        bool OnStart() override;
        void OnRun() override;
        void OnStop() override;
        void OnReceive(const core::Message& message) override;
    };
}
