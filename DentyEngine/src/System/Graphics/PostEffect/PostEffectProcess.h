#pragma once

namespace DentyEngine
{
	class PostEffect;

	class PostEffectProcess final
	{
		using PostEffects = std::vector<Ref<PostEffect>>;
	public:
		PostEffectProcess() = delete;
		PostEffectProcess(const PostEffects& postEffects, const Vector2& screenSize);
		~PostEffectProcess() = default;

		// Call each post effect "Process" function.
		void OnPostProcessRender();

		DELETE_COPY_MOVE_CONSTRUCTORS(PostEffectProcess)
	private:
		PostEffects _postEffects;

		Vector2 _screenSize;
	};
}