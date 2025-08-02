#pragma once

#include "PostEffect.h"

#include "Graphics/Buffer/FrameBuffer.h"
#include "Graphics/States/BlendState.h"

namespace DentyEngine
{
	class GaussianBlur final : public PostEffect
	{
	public:
		struct Constants
		{
			Constants() : weights() {}

			std::array<float, 8> weights;
		};

		struct HorizontalDownSampling
		{
		public:
			HorizontalDownSampling() : buffer(), shader() {}

			void Create(const Vector2& screenSize);
		public:
			Scope<FrameBuffer> buffer;
			Ref<Shader> shader;
		};

		struct VerticalDownSampling
		{
		public:
			VerticalDownSampling() : buffer(), shader() {}

			void Create(const Vector2& screenSize);
		public:
			Scope<FrameBuffer> buffer;
			Ref<Shader> shader;
		};
	public:
		GaussianBlur();
		GaussianBlur(const GaussianBlur& source) = default;
		~GaussianBlur() override = default;

		//
		// Inherited functions.
		//
		void Create(Ref<SceneBuffer> sceneBuffer, const Vector2& screenSize, BlendState::Type blendMode = BlendState::Type::Alpha, bool useDefaultBlendMode = true) override;
		void Process() override;
		void OnGui() override;

		//
		// Getter
		//
		[[nodiscard]]
		Type GetType() const override { return Type::GaussianBlur; }
	private:
		//
		// Static functions.
		//

		// Reference -> HLSLÇÃñÇì±èë (10.3 ÉuÉâÅ[)
		static void CalculateWeightsTableFromGaussian(std::array<float, 8>& weights, float bokehRate);

		//
		// private variables.
		//
	private:
		HorizontalDownSampling _horizontalDownSampling;
		VerticalDownSampling _verticalDownSampling;

		Constants _constants;

		float _bokehRate;
	};
}
