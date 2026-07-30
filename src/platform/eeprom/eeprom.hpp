#pragma once

#include <stdint.h>

namespace dan::platform
{
    class Eeprom final
    {
    public:
        static bool Initialize();
        static bool Read(
            uint16_t address,
            uint8_t* buffer,
            uint16_t size
        );
        static bool Write(
            uint16_t address,
            const uint8_t* data,
            uint16_t size
        );

        Eeprom() = delete;

    private:
        static bool IsRangeValid(uint16_t address, uint16_t size);
    };
}
