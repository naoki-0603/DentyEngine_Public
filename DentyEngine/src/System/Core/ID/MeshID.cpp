#include "stdafx.h"

#include "MeshID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
	MeshID::MeshID() :
		_meshID(Random::GetRandomValuesUint64T())
	{
	}

	MeshID::MeshID(const uint64_t uuid) :
		_meshID(uuid)
	{
	}

	void MeshID::ReCalculate()
	{
		_meshID = Random::GetRandomValuesUint64T();
	}
}