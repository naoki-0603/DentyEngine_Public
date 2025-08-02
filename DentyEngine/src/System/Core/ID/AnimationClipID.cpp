#include "stdafx.h"

#include "AnimationClipID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
    AnimationClipID::AnimationClipID()
        : _animationClipID(Random::GetRandomValuesUint64T())
    {
    }

    AnimationClipID::AnimationClipID(const uint64_t id)
        : _animationClipID(id)
    {
    }

    void AnimationClipID::ReCalculate()
    {
        _animationClipID = Random::GetRandomValuesUint64T();
    }
};