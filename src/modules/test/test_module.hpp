#pragma once

#include "core/message/message.hpp"
#include "core/module/module.hpp"

namespace dan::modules
{
    class TestModule final : public core::Module
    {
    public:
        [[nodiscard]]
        core::ModuleId GetId() const override;

        [[nodiscard]]
        const char* GetName() const override;

        [[nodiscard]]
        bool HasReceivedMessage() const;

        [[nodiscard]]
        const core::Message& GetLastMessage() const;

    protected:
        bool OnInitialize() override;
        bool OnStart() override;
        void OnRun() override;
        void OnStop() override;
        void OnReceive(const core::Message& message) override;

    private:
        bool hasReceivedMessage = false;
        core::Message lastMessage{};
    };
}
