#pragma once

// http://www.sanko-shoko.net/note.php?id=rnfd

// Recreated in 2024/1/19
// https://hakase0274.hatenablog.com/entry/2019/10/06/234413

namespace DentyEngine
{
    struct TimerParameters
    {
    public:
        TimerParameters();

    public:
        LARGE_INTEGER begin;
        LARGE_INTEGER end;
        LARGE_INTEGER freq;

        DISPLAY_DEVICEA displayDevice;
        DEVMODEA devMode;

        float fps;
        float deltaTime;

        int32_t counter;
    };

    class Timer final
    {
    public:
        Timer();
        ~Timer() = default;

        static void Initialize();

        static void BeginFrame();
        static void EndFrame();

        static void Update();
        static void Gui();

        [[nodiscard]]
    	static float GetDeltaTime() { return _parameters.deltaTime; }

        [[nodiscard]]
        static int32_t GetFPS() { return static_cast<int32_t>(_parameters.fps); }

        [[nodiscard]]
        static int32_t GetDisplayFrequency() { return static_cast<int32_t>(_parameters.devMode.dmDisplayFrequency); }
    private:
        static TimerParameters _parameters;

        //
        // Constants.
        //
    private:
        static const double FRAME_PER_SECOND;
    };
};