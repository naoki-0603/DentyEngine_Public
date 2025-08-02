#include "stdafx.h"

#include "AnimatorControllerID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
    AnimatorControllerID::AnimatorControllerID()
        : _animatorControllerID(Random::GetRandomValuesUint64T())
    {
    }

    AnimatorControllerID::AnimatorControllerID(const uint64_t id)
        : _animatorControllerID(id)
    {
    }

    void AnimatorControllerID::ReCalculate()
    {
        _animatorControllerID = Random::GetRandomValuesUint64T();
    }
};