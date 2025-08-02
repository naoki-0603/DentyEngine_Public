#include "stdafx.h"

#include "UUID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
    UUID::UUID() :
        _uuid(Random::GetRandomValuesUint64T())
    {
    }

    UUID::UUID(const uint64_t uuid) :
        _uuid(uuid)
    {
    }

    void UUID::ReCalculate()
    {
        _uuid = Random::GetRandomValuesUint64T();
    }
};