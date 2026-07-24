#pragma once

namespace dan::platform
{
    class Platform final
    {
    public:
        static void Initialize();
        static void Start();
        static void Run();
        static void Stop();

        Platform() = delete;
    };
}