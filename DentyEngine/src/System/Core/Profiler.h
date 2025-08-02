#pragma once

namespace DentyEngine
{
    class Profiler final
    {
    private:
        using MeasuredData = std::unordered_map<std::string, double>;
    public:
        //
        // Define singleton
        //
        DEFINE_SINGLETON_CLASS(Profiler)

        //
        // public functions.
        //

        // Fps etc...
        void AddSystem(std::string_view name, double microseconds);

        // Rendering time etc...
        void AddGraphics(std::string_view name, double microseconds);

        void OnGui();

        // Call on after called "Timer::EndFrame".
        void Clear();

        //
        // private variables.
        //
    private:
        MeasuredData _systemMeasuredData;
        MeasuredData _graphicsMeasuredData;
    };
}