#pragma once

#include "ReadableBuffer.h"

#include "../Color.h"

namespace DentyEngine
{
	class ColorBuffer;
	class DepthBuffer;
	class FrameBuffer;

	class BlendState;

	class SceneBuffer final : public ReadableBuffer
	{
	public:
		SceneBuffer();
		~SceneBuffer() override = default;

		void Create(ID3D11Device* device, const Vector2& size);

		void Clear(ID3D11DeviceContext* context, const Color& clearColor);
		void Activate(ID3D11DeviceContext* context);
		void DeActivate(ID3D11DeviceContext* context);

		void OnResize(ID3D11Device* device, const Vector2& size);

		[[nodiscard]]
		int32_t ReadPixel(ID3D11Device* device, ID3D11DeviceContext* context,
			ID3D11ShaderResourceView* bufferResource,
			DXGI_FORMAT readBufferFormat,
			const Vector2& readBufferSize, const Vector2& fetchPosition) const override;

		//
		// Getter
		//

		[[nodiscard]]
		ColorBuffer* GetColorBuffer() const { return _colorBuffer.get(); }

		[[nodiscard]]
		ColorBuffer* GetEntityIDBuffer() const { return _entityIDBuffer.get(); }

		[[nodiscard]]
		DepthBuffer* GetDepthBuffer() const { return _depthBuffer.get(); }

		[[nodiscard]]
		const D3D11_VIEWPORT& GetViewport() const { return _viewport; }
	private:
		Ref<ColorBuffer> _colorBuffer;
		Ref<ColorBuffer> _entityIDBuffer;
		Ref<DepthBuffer> _depthBuffer;
		Ref<BlendState> _blendState;

		D3D11_VIEWPORT _viewport;
		D3D11_VIEWPORT _cachedViewport;
	};
};