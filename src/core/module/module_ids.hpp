#pragma once

#include "core/module/module_id.hpp"

namespace dan::core::ModuleIds
{
    inline constexpr ModuleId System = 0x01;
    inline constexpr ModuleId Message = 0x02;
    inline constexpr ModuleId Eeprom = 0x03;
    inline constexpr ModuleId Network = 0x04;
    inline constexpr ModuleId Runtime = 0x05;
    inline constexpr ModuleId Configuration = 0x06;

    inline constexpr ModuleId FirstCore = System;
    inline constexpr ModuleId LastCore = 0x0F;

    inline constexpr ModuleId FirstApplication = 0x10;
    inline constexpr ModuleId LastApplication = 0x3E;

    constexpr bool IsCore(ModuleId id)
    {
        return id >= FirstCore && id <= LastCore;
    }

    constexpr bool IsApplication(ModuleId id)
    {
        return id >= FirstApplication && id <= LastApplication;
    }

    static_assert(System == 0x01);
    static_assert(Message == 0x02);
    static_assert(Eeprom == 0x03);
    static_assert(Network == 0x04);
    static_assert(Runtime == 0x05);
    static_assert(Configuration == 0x06);
    static_assert(LastCore == 0x0F);
    static_assert(FirstApplication == 0x10);
    static_assert(LastApplication == 0x3E);
    static_assert(LastCore + 1U == FirstApplication);
}
