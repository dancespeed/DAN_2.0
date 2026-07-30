#pragma once

#include <stddef.h>

class __FlashStringHelper;

class Stream
{
public:
    virtual ~Stream() = default;

    virtual size_t print(const char* text) = 0;
    virtual size_t println(const char* text) = 0;

    size_t print(const __FlashStringHelper* text)
    {
        return print(reinterpret_cast<const char*>(text));
    }

    size_t println(const __FlashStringHelper* text)
    {
        return println(reinterpret_cast<const char*>(text));
    }
};

class HardwareSerial final : public Stream
{
public:
    void begin(unsigned long)
    {
    }

    size_t print(const char*) override
    {
        return 0;
    }

    size_t println(const char*) override
    {
        return 0;
    }
};

extern HardwareSerial Serial;

inline void delay(unsigned long)
{
}

#define F(text) reinterpret_cast<const __FlashStringHelper*>(text)
