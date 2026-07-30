#include "eeprom.hpp"

#include <EEPROM.h>

namespace dan::platform
{
    bool Eeprom::Initialize()
    {
        return EEPROM.length() > 0;
    }
}
