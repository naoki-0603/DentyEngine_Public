#pragma once

namespace DentyEngine
{
	class PostEffect;

	class PostEffectGenerator final
	{
		using PostEffects = std::unordered_map<std::string, Ref<PostEffect>>;
	public:
		PostEffectGenerator();
		~PostEffectGenerator() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(PostEffectGenerator)

		static void Initialize();
		static void ShowList(_Out_ std::string& postEffectName, _Out_ bool& pressedAddPostEffectButton);

		// If name is doesn't exists, return nullptr.
		static Ref<PostEffect> Generate(std::string_view name);
	private:
		static bool Exists(std::string_view name);
	private:
		static PostEffects _postEffects;
	};
}