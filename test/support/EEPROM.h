#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

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

    uint8_t read(int address) const
    {
        return storage_[static_cast<size_t>(address)];
    }

    void update(int address, uint8_t value)
    {
        const size_t index = static_cast<size_t>(address);
        if (storage_[index] != value)
        {
            storage_[index] = value;
            ++updateCount_;
        }
    }

    void Reset()
    {
        storage_.fill(0xFF);
        updateCount_ = 0;
    }

    size_t GetUpdateCount() const
    {
        return updateCount_;
    }

private:
    std::array<uint8_t, 1024> storage_{};
    int length_ = 1024;
    size_t updateCount_ = 0;
};

extern EEPROMClass EEPROM;
