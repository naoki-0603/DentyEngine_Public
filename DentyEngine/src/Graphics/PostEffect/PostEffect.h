#pragma once

#include "System/Core/Object.h"
#include "Graphics/States/BlendState.h"

namespace DentyEngine
{
	class SceneBuffer;
	class FrameBuffer;
	class FullScreenQuad;
	class Shader;

	// Base class for post effect.
	class PostEffect : public Object
	{
	public:
		enum class Type
		{
			None,
			GaussianBlur,
			Bloom,
			KawaseBloom,

			Max
		};
	public:
		PostEffect();
		PostEffect(const PostEffect& source) = default;
		virtual ~PostEffect() override = default;

		//
		// Virtual functions.
		//
		virtual void Create(Ref<SceneBuffer> sceneBuffer, const Vector2& screenSize, BlendState::Type blendMode = BlendState::Type::Alpha, bool useDefaultBlendMode = true);
		virtual void Process() { }
		virtual void OnGui() {}

		//
		// Setter
		//
		void SetBaseBuffer(Ref<SceneBuffer> baseBuffer) { _sceneBuffer = baseBuffer; }

		//
		// Getter
		//
		[[nodiscard]]
		const Ref<FrameBuffer>& GetResultBuffer() const { return _resultBuffer; }

		[[nodiscard]]
		virtual Type GetType() const { return Type::None; }

		[[nodiscard]]
		bool HasCreated() const { return _hasCreated; }
	protected:
		//
		// non-static variables.
		// 
		Ref<SceneBuffer> _sceneBuffer;
		Ref<FrameBuffer> _resultBuffer;
		Ref<FullScreenQuad> _fullScreenQuad;
		Ref<Shader> _fullScreenQuadShader;

		bool _hasCreated;

		//
		// static variables.
		//
		static constexpr Vector2 PROCESS_IMAGE_SIZE = { 128.0f, 128.0f };
	};
}
