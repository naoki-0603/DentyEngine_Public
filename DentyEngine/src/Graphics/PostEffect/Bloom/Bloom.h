#pragma once

#include "../PostEffect.h"

#include "Graphics/States/BlendState.h"

namespace DentyEngine
{
	class GaussianBlur;
	class Shader;

	class Bloom final : public PostEffect
	{
	public:
		Bloom();
		Bloom(const Bloom&) = default;
		~Bloom() override = default;

		void Create(Ref<SceneBuffer> sceneBuffer, const Vector2& screenSize, BlendState::Type blendMode = BlendState::Type::Alpha, bool useDefaultBlendMode = true) override;
		void Process() override;
		void OnGui() override;

		[[nodiscard]]
		Type GetType() const override { return Type::Bloom; }
	private:
		Ref<SceneBuffer> _extractionLuminanceBuffer;
		Ref<GaussianBlur> _gaussianBlur;
		Ref<Shader> _luminanceExtractionShader;
	};
}
