#include "stdafx.h"

#include "MeshInfo.h"

namespace DentyEngine
{
	const extern std::array<std::string, 2> SCALE_FACTOR_STRINGS =
	{
		"Cm", "M"
	};

	const extern std::array<std::string, 4> COORDINATE_SYSTEM_STRINGS =
	{
		"RhsYUp", "LhsYUp", "RhsZUp", "LhsZUp"
	};

	const std::array<Matrix, 4> COORDINATE_SYSTEM_TRANSFORM =
	{
		{
			// Rhs-y-up
			{ -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },

			// Lhs-y-up
			{  1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },

			// Rhs-z-up
			{ -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },

			// Lhs-z-up
			{  1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 }
		}
	};
}