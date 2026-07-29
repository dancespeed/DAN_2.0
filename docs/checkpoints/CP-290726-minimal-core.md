# Чекпоинт: минимальное ядро без прикладных модулей

**Проект:** DAN Platform 2.0
**Дата:** 2026-07-29
**Статус:** реализовано и проверено host-тестами

## Зафиксированный результат

Ядро DAN запускается и выполняет полный жизненный цикл без `Logic`,
`TestModule` и любых других прикладных модулей:

```text
System::Initialize()
    -> Runtime::Initialize()
    -> пустая ModuleTable
    -> MessageBus
    -> MessageDispatcher

System::Start()
System::Run()
System::Stop()
```

`Runtime` больше не создаёт и не регистрирует `TestModule`.
Обычный запуск прошивки больше не запускает
`RoutingDeviceTest::Run()`.

Исходники `TestModule` и routing-теста сохранены как диагностический код,
но не входят в стандартный состав запущенного ядра.

## Граница кирпичика

Этот чекпоинт не добавляет:

- системный реестр `ModuleID`;
- EEPROM;
- `ConfigurationTable` и её проигрывание;
- назначение `GlobalID`;
- новые прикладные модули.

`LocalDeviceId` пока остаётся compile-time значением из
`runtime_config.hpp`.

## Проверка

Пройдены:

- host smoke-тест пустого жизненного цикла `Runtime`;
- существующий `message_core_test`;
- компиляция host-тестов с `-Wall -Wextra -Werror`;
- `git diff --check`.

AVR-сборка в этом окружении не выполнялась: PlatformIO и `avr-g++`
недоступны.

## Следующий кирпичик

Отдельно закрепить неприкосновенный реестр системных `ModuleID`:

```text
System  = 0x01
Message = 0x02
EEPROM  = 0x03
Network = 0x04
Runtime = 0x05
```

EEPROM и механизм конфигурации в этот следующий шаг не включать.
