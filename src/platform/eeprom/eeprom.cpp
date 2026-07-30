#include "eeprom.hpp"

#include <EEPROM.h>

namespace dan::platform
{
    bool Eeprom::Initialize()
    {
        return EEPROM.length() > 0;
    }

    bool Eeprom::Read(
        uint16_t address,
        uint8_t* buffer,
        uint16_t size)
    {
        if (buffer == nullptr || !IsRangeValid(address, size))
        {
            return false;
        }

        for (uint16_t index = 0; index < size; ++index)
        {
            buffer[index] = EEPROM.read(address + index);
        }

        return true;
    }

    bool Eeprom::Write(
        uint16_t address,
        const uint8_t* data,
        uint16_t size)
    {
        if (data == nullptr || !IsRangeValid(address, size))
        {
            return false;
        }

        for (uint16_t index = 0; index < size; ++index)
        {
            EEPROM.update(address + index, data[index]);
        }

        return true;
    }

    bool Eeprom::IsRangeValid(uint16_t address, uint16_t size)
    {
        if (size == 0)
        {
            return false;
        }

        const uint32_t end =
            static_cast<uint32_t>(address) + size;
        return end <= static_cast<uint32_t>(EEPROM.length());
    }
}
