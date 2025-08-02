#pragma once

namespace DentyEngine
{
	class Texture;
	class Texture2D;
	class CubeMap;

	class TextureSerializer final
	{
	public:
		TextureSerializer();
		~TextureSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(TextureSerializer)

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const Texture2D>&);

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const CubeMap>&);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<Texture>&);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<Texture2D>&);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<CubeMap>&);
	};
}