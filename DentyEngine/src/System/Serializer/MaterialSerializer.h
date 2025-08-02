#pragma once

namespace DentyEngine
{
	class Material;
	class CubeMaterial;

	class MaterialSerializer final
	{
	public:
		MaterialSerializer();
		~MaterialSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(MaterialSerializer)

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const Material>&);

		[[nodiscard]]
		static bool CubeSerializeAsJson(const std::filesystem::path&, const Ref<const CubeMaterial>&);
		
		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<Material>&);

		[[nodiscard]]
		static bool CubeDeserializeFromJson(const std::filesystem::path&, Ref<CubeMaterial>&);
	};
}