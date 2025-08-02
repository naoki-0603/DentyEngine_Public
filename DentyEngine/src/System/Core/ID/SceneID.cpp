#include "stdafx.h"

#include "SceneID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
	SceneID::SceneID() :
		_sceneID(Random::GetRandomValuesUint64T())
	{
	}

	SceneID::SceneID(const uint64_t uuid) :
		_sceneID(uuid)
	{
	}

	void SceneID::ReCalculate()
	{
		_sceneID = Random::GetRandomValuesUint64T();
	}
}