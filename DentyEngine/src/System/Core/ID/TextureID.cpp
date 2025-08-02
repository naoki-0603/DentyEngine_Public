#include "stdafx.h"

#include "TextureID.h"

#include "Utilities/Random.h"

namespace DentyEngine
{
	TextureID::TextureID() :
		_textureID(Random::GetRandomValuesUint64T())
	{
	}

	TextureID::TextureID(const uint64_t textureID) :
		_textureID(textureID)
	{
	}

	void TextureID::ReCalculate()
	{
		_textureID = Random::GetRandomValuesUint64T();
	}
}