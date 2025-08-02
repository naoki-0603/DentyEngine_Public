#include "stdafx.h"

#include "Timer.h"

namespace DentyEngine
{
    TimerParameters::TimerParameters() :
        begin(), end(), freq(), displayDevice(), devMode(), fps(), deltaTime(), counter()
    {
    }

    TimerParameters Timer::_parameters = {};

    const double Timer::FRAME_PER_SECOND = 1.0 / 60.0;

    Timer::Timer()
    {
    }

    void Timer::Initialize()
    {
        QueryPerformanceFrequency(&_parameters.freq);

        // Obtain display settings.
        {
            _parameters.displayDevice.cb = sizeof(DISPLAY_DEVICE);

            EnumDisplayDevicesA(nullptr, 0, &_parameters.displayDevice, 0);

            _parameters.devMode.dmSize = sizeof(DEVMODEA);
            _parameters.devMode.dmDriverExtra = sizeof(DEVMODEA);
            EnumDisplaySettingsA( _parameters.displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &_parameters.devMode);
        }
    }

    void Timer::BeginFrame()
    {
        QueryPerformanceCounter(&_parameters.begin);
    }

    void Timer::EndFrame()
    {
        QueryPerformanceCounter(&_parameters.end);
    }

    void Timer::Update()
    {
        // Calculate delta time.
        _parameters.deltaTime = static_cast<float>(_parameters.end.QuadPart - _parameters.begin.QuadPart) / static_cast<float>(_parameters.freq.QuadPart);

        if (_parameters.counter >= static_cast<int32_t>(_parameters.devMode.dmDisplayFrequency))
        {
            _parameters.fps = 1.0f / _parameters.deltaTime;
            _parameters.counter = 0;
        }
        else
        {
            ++_parameters.counter;
        }
    }

    void Timer::Gui()
    {
    }
};