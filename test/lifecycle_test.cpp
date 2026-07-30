#include <assert.h>

#include "../src/core/message/message.hpp"
#include "../src/core/module/module.hpp"
#include "../src/core/module/module_ids.hpp"
#include "../src/core/module/module_state.hpp"
#include "../src/core/module/module_table.hpp"
#include "../src/core/system/system.hpp"
#include "../src/platform/platform.hpp"

using namespace dan::core;

namespace
{
    class LifecycleModule final : public Module
    {
    public:
        LifecycleModule(
            ModuleId id,
            const char* name,
            bool initializeResult,
            bool startResult)
            : id_(id),
              name_(name),
              initializeResult_(initializeResult),
              startResult_(startResult)
        {
        }

        ModuleId GetId() const override
        {
            return id_;
        }

        const char* GetName() const override
        {
            return name_;
        }

    protected:
        bool OnInitialize() override
        {
            return initializeResult_;
        }

        bool OnStart() override
        {
            return startResult_;
        }

        void OnRun() override
        {
        }

        void OnStop() override
        {
        }

        void OnReceive(const Message&) override
        {
        }

    private:
        ModuleId id_;
        const char* name_;
        bool initializeResult_;
        bool startResult_;
    };
}

namespace dan::platform
{
    void Platform::Initialize()
    {
    }

    void Platform::Start()
    {
    }

    void Platform::Run()
    {
    }

    void Platform::Stop()
    {
    }
}

int main()
{
    LifecycleModule readyModule(
        ModuleIds::FirstApplication,
        "ReadyModule",
        true,
        true
    );

    LifecycleModule initializeFailure(
        static_cast<ModuleId>(ModuleIds::FirstApplication + 1U),
        "InitializeFailure",
        false,
        true
    );

    LifecycleModule startFailure(
        static_cast<ModuleId>(ModuleIds::FirstApplication + 2U),
        "StartFailure",
        true,
        false
    );

    assert(readyModule.Initialize());
    assert(readyModule.Initialize());
    assert(readyModule.Start());
    assert(readyModule.Start());
    assert(readyModule.GetState() == ModuleState::Running);

    assert(!initializeFailure.Initialize());
    assert(!initializeFailure.Start());
    assert(initializeFailure.GetState() == ModuleState::Stopped);

    assert(startFailure.Initialize());
    assert(!startFailure.Start());
    assert(startFailure.GetState() == ModuleState::Initialized);

    readyModule.Stop();
    startFailure.Stop();

    assert(!System::Initialize(MessageProtocol::InvalidGlobal));
    assert(!System::Start());

    assert(System::Initialize(MessageProtocol::FirstDevice));
    assert(System::Initialize(MessageProtocol::FirstDevice));
    assert(System::Start());
    assert(System::Start());

    System::Run();
    System::Stop();

    ModuleTable::Initialize();
    assert(ModuleTable::Register(readyModule));
    assert(ModuleTable::Register(initializeFailure));
    assert(ModuleTable::Register(startFailure));

    ModuleTable::InitializeModules();
    ModuleTable::Start();

    assert(readyModule.GetState() == ModuleState::Running);
    assert(initializeFailure.GetState() == ModuleState::Stopped);
    assert(startFailure.GetState() == ModuleState::Initialized);

    ModuleTable::Stop();
    return 0;
}
