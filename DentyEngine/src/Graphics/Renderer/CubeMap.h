#pragma once

#include "Texture.h"

namespace DentyEngine
{
	class CubeMap final : public Texture, public std::enable_shared_from_this<CubeMap>
	{
	public:
		CubeMap();
		CubeMap(const CubeMap&) = default;
		~CubeMap() override = default;

		CubeMap& operator=(const CubeMap& source);

		void Load(ID3D11Device* device, const std::filesystem::path& filePath);

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile) override;
		void Clone(const Ref<Asset>& asset) override;

		void OnSerialized() const override;
		void OnDeserialized() override;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<Texture>(this)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<Texture>(this)
			);
		}
	};
};

CEREAL_CLASS_VERSION(DentyEngine::CubeMap, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::CubeMap)