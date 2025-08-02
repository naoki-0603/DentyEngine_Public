#include "stdafx.h"

#include "PreviewImageID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
    PreviewImageID::PreviewImageID()
        : _id(Random::GetRandomValuesUint64T())
    {
    }

    PreviewImageID::PreviewImageID(const uint64_t id)
        : _id(id)
    {
    }

    void PreviewImageID::ReCalculate()
    {
        _id = Random::GetRandomValuesUint64T();
    }
}