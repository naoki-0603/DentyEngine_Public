#pragma once

namespace DentyEngine
{
	class Texture2D;
	class EditorHelper final
	{
	public:
		EditorHelper() noexcept = default;
		~EditorHelper() noexcept = default;

		[[nodiscard]]
		static std::optional<Ref<Texture2D>> SelectTexture();
	private:
		static const Vector2 TEXTURE_THUMBNAIL_SIZE;
	};
}
