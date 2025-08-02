#pragma once

#include "ReadableBuffer.h"

#include "Graphics/States/BlendState.h"

#include "Graphics/Color.h"

namespace DentyEngine
{
	class BlendState;
	class ColorBuffer;
    class DepthBuffer;

    class FrameBuffer final : public ReadableBuffer
    {
    public:
		enum class Usage
		{
			UseRenderTargetView = 1,
			UseDepthStencilView = 2,
			UseBoth = (UseRenderTargetView | UseDepthStencilView),

			Max = 3
		};
	public:
		FrameBuffer();
		~FrameBuffer() override = default;

		void Create(ID3D11Device* device, const Vector2& size,
			BlendState::Type blendMode = BlendState::Type::Alpha,
			DXGI_FORMAT colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_R32_TYPELESS, Usage usage = Usage::UseBoth, bool makeRtvSrv = true, bool makeDsvSrv = true);
		
		void Clear(ID3D11DeviceContext* context, const Color& clearColor);
		void Activate(ID3D11DeviceContext* context);
		void DeActivate(ID3D11DeviceContext* context);

		void OnResize(ID3D11Device* device, const Vector2& size);

		int ReadPixel(ID3D11Device* device, ID3D11DeviceContext* context,
			ID3D11ShaderResourceView* bufferResource,
			DXGI_FORMAT readBufferFormat,
			const Vector2& readBufferSize, const Vector2& fetchPosition) const override;

		[[nodiscard]]
    	Ref<ColorBuffer> GetColorBuffer() const { return _colorBuffer; }

		[[nodiscard]]
    	Ref<DepthBuffer> GetDepthBuffer() const { return _depthBuffer; }

		[[nodiscard]]
		const D3D11_VIEWPORT& GetViewport() const { return _viewport; }
    private:
		void CreateColorBuffer(ID3D11Device* device, const Vector2& size, DXGI_FORMAT format, bool makeSrv);
		void CreateDepthBuffer(ID3D11Device* device, const Vector2& size, DXGI_FORMAT format, bool makeSrv);
	private:
		Ref<ColorBuffer> _colorBuffer;
		Ref<DepthBuffer> _depthBuffer;

		D3D11_VIEWPORT _viewport;
		D3D11_VIEWPORT _cachedViewport;

		BlendState::Type _blendMode;

		Usage _usage;
    };

	DEFINE_ENUM_CLASS_BIT_OPERATORS(FrameBuffer::Usage)
};