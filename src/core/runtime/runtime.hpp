#pragma once

namespace dan::core
{
    class Runtime final
    {
    public:
        static void Initialize();
        static void Start();
        static void Run();
        static void Stop();

        Runtime() = delete;
    };
}