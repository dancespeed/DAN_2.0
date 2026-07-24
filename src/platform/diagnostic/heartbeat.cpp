#include "heartbeat.hpp"
#include "heartbeat_config.hpp"

#include <avr/io.h>

namespace
{
    uint32_t previousToggleTickMs = 0;
}

namespace dan::platform
{
    void Heartbeat::Initialize()
    {
        /*
         * Настраиваем вывод heartbeat как выход.
         */
        DDRC |=
            static_cast<uint8_t>(
                1U << HeartbeatConfig::PortBit);

        /*
         * Начальное состояние — LOW.
         */
        PORTC &=
            static_cast<uint8_t>(
                ~(1U << HeartbeatConfig::PortBit));

        previousToggleTickMs = 0;
    }

    void Heartbeat::Update(const uint32_t currentTickMs)
    {
        /*
         * Вычитание unsigned-значений корректно работает
         * и при переполнении системного счётчика.
         */
        const uint32_t elapsedMs =
            currentTickMs - previousToggleTickMs;

        if (elapsedMs < HeartbeatConfig::TogglePeriodMs)
        {
            return;
        }

        previousToggleTickMs = currentTickMs;

        PORTC ^=
            static_cast<uint8_t>(
                1U << HeartbeatConfig::PortBit);
    }

    void Heartbeat::Stop()
    {
        PORTC &=
            static_cast<uint8_t>(
                ~(1U << HeartbeatConfig::PortBit));

        previousToggleTickMs = 0;
    }
}