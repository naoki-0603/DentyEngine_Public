#include "stdafx.h"

#include "ShaderID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
    ShaderID::ShaderID()
        : _shaderID(Random::GetRandomValuesUint64T())
    {
    }

    ShaderID::ShaderID(const uint64_t id)
        : _shaderID(id)
    {
    }

    void ShaderID::ReCalculate()
    {
        _shaderID = Random::GetRandomValuesUint64T();
    }
};