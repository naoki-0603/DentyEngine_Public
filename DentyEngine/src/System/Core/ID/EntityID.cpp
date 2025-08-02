#include "stdafx.h"

#include "EntityID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
    EntityID::EntityID()
        : _entityID(Random::GetRandomValuesUint32T())
    {
        /*if (EditorSceneManager::HasActiveScene())
        {
            while (EditorSceneManager::GetActiveScene()->IsDuplicate(_entityID))
            {
                _entityID = Random::GetRandomValuesUint32T();
            }
        }*/
    }

    EntityID::EntityID(const uint32_t id)
        : _entityID(id)
    {
    }

    void EntityID::ReCalculate()
    {
        _entityID = Random::GetRandomValuesUint32T();
    }
};