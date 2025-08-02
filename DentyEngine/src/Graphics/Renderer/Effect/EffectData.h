#pragma once

#include "Graphics/Color.h"

namespace DentyEngine
{
    struct EffectData
    {
    public:
        EffectData();

    public:
        Vector3 position;
        Vector3 velocity;
        Color color;

        float age;
        float size;
    };
}
