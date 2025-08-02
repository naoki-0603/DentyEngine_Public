#pragma once

namespace DentyEngine
{
	namespace Utils
	{
		static constexpr uint8_t TEXTURE_ALBEDO = (1 << 0);
		static constexpr uint8_t TEXTURE_SPECULAR = (1 << 1);
	}

	class Material;

	class EditorAssetSetter final
	{
	public:
		EditorAssetSetter();
		~EditorAssetSetter() = default;

		[[nodiscard]]
		static bool SetTextureAssetFromDirectory(
			const Ref<Material>& material, uint8_t textureUsed
		);

	private:
		[[nodiscard]]
		static bool IsSupportedTexture(uint8_t textureUsed);
	};
}