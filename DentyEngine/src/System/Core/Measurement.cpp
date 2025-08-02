#include "stdafx.h"

#include "Measurement.h"

#include "Profiler.h"

namespace DentyEngine
{
    Measurement::Measurement(std::string_view name, Type type) :
        _startTimePoint(), _name(name), _type(type)
    {
    }

    Measurement::~Measurement()
    {
        // Calculate duration.
        double ms = 0.0;
        {
            const auto endTimePoint = std::chrono::high_resolution_clock::now();

            const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimePoint).time_since_epoch().count();
            const auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

            const auto duration = end - start;

            ms = duration * 0.001;
        }

        // Add to profiler.
        {
            switch (_type)
            {
            case Type::Graphics:
                Profiler::GetInstance().AddGraphics(_name, ms);
                break;
            default:
                DENTY_ASSERT(false, "Unknown type! (in Masurement::~Measurement)");
                break;
            }
        }
       
    }
}
