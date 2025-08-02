#pragma once

namespace DentyEngine
{
    // Reference https://www.youtube.com/watch?v=YG4jexlSAjc

    //
    // Usage
    //

    // {
    //      Measurement measurement;
    // 
    //      Processing something.
    // }
    // Called measurement destructor.

    class Measurement final
    {
    public:
        enum class Type
        {
            Graphics,

            Max
        };
    public:
        Measurement() = delete;
        Measurement(std::string_view name, Type type);

        // Add to measured data to profiler.
        ~Measurement();

        DELETE_COPY_MOVE_CONSTRUCTORS(Measurement)
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> _startTimePoint;

        std::string _name;

        Type _type;
    };
}