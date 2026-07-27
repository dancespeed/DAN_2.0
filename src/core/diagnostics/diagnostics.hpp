#pragma once

#include <Arduino.h>

namespace dan::core
{

class Diagnostics final
{
public:
    static void Initialize(Stream& output);

    static void Enable();
    static void Disable();
    static bool IsEnabled();

    static void Print(const char* text);
    static void PrintLine(const char* text);

private:
    static Stream* output_;
    static bool enabled_;
};

} // namespace dan::core
