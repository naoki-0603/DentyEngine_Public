#include "stdafx.h"

#include "PrefabID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
	PrefabID::PrefabID() :
		_prefabID(Random::GetRandomValuesUint64T())
	{
	}

	PrefabID::PrefabID(const uint64_t prefabID) :
		_prefabID(prefabID)
	{
	}

	void PrefabID::ReCalculate()
	{
		_prefabID = Random::GetRandomValuesUint64T();
	}
}