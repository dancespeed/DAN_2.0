#include "test_module.hpp"
#include "core/module/module_ids.hpp"

namespace dan::modules
{
    core::ModuleId TestModule::GetId() const
    {
        return core::ModuleIds::Test;
    }

    bool TestModule::OnInitialize()
    {
        return true;
    }

    bool TestModule::OnStart()
    {
        return true;
    }

    void TestModule::OnRun()
    {
    }

    void TestModule::OnStop()
    {
    }
}