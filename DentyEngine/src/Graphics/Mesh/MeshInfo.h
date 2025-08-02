#pragma once

#include "System/Core/ID/MaterialID.h"

namespace DentyEngine
{
	enum class ScaleFactor
	{
		// centimeter
		Cm,

		// meter
		M,

		Max
	};

	enum class CoordinateSystem
	{
		RhsYUp,
		LhsYUp,
		RhsZUp,
		LhsZUp,

		Max
	};

	enum class MeshType
	{
		None = -1,
		Dynamic = 0,
		StaticBatching,

		Max
	};

	//
	// Strings
	//
	const extern std::array<std::string, 2> SCALE_FACTOR_STRINGS;
	const extern std::array<std::string, 4> COORDINATE_SYSTEM_STRINGS;

	const extern std::array<Matrix, 4> COORDINATE_SYSTEM_TRANSFORM;

	struct Subset
	{
		Subset() : indexStart(), indexCount(), materialIDInFile(0u), materialID(0u), materialName() {}

		uint32_t indexStart;
		uint32_t indexCount;
		uint64_t materialIDInFile;

		MaterialID materialID;

		std::string materialName;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
					CEREAL_NVP(indexStart),
					CEREAL_NVP(indexCount),
					CEREAL_NVP(materialIDInFile),
					CEREAL_NVP(materialID),
					CEREAL_NVP(materialName)
			    );
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
					CEREAL_NVP(indexStart),
					CEREAL_NVP(indexCount),
					CEREAL_NVP(materialIDInFile),
					CEREAL_NVP(materialID),
					CEREAL_NVP(materialName)
			    );
			}
		}
	};

	// Constants
	static constexpr uint32_t MAX_BONE_INFLUENCES = 4;
};

CEREAL_CLASS_VERSION(DentyEngine::Subset, 1u)