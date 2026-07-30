#pragma once

namespace dan::platform
{
    class Eeprom final
    {
    public:
        static bool Initialize();

        Eeprom() = delete;
    };
}
