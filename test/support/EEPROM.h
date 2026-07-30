#pragma once

class EEPROMClass
{
public:
    int length() const
    {
        return length_;
    }

    void SetLength(int length)
    {
        length_ = length;
    }

private:
    int length_ = 1024;
};

extern EEPROMClass EEPROM;
