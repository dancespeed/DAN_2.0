#pragma once

namespace dan::core
{
    class System final
    {
    public:
        static void Initialize();
        static void Start();
        static void Run();
        static void Stop();

        System() = delete;
    };
}