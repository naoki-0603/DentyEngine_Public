#include "stdafx.h"

#include "FrameBuffer.h"

#include "ColorBuffer.h"
#include "DepthBuffer.h"

#include "Graphics/States/BlendState.h"
#include "Graphics/States/DepthStencilState.h"
#include "Graphics/States/RasterizerState.h"

namespace DentyEngine
{
    FrameBuffer::FrameBuffer() :
	    _colorBuffer(), _depthBuffer(), _viewport(), _cachedViewport(), _blendMode(), _usage()
    {
    }

    void FrameBuffer::Create(ID3D11Device* device, const Vector2& size, [[maybe_unused]] BlendState::Type blendMode,
                             DXGI_FORMAT colorBufferFormat, DXGI_FORMAT depthBufferFormat, Usage usage, bool makeRtvSrv, bool makeDsvSrv)
    {
		// Create buffers
        switch (usage)
		{
		case Usage::UseRenderTargetView:
		    {
		        CreateColorBuffer(device, size, colorBufferFormat, makeRtvSrv);
		    }
			break;
		case Usage::UseDepthStencilView:
		    {
		        CreateDepthBuffer(device, size, depthBufferFormat, makeDsvSrv);
		    }
			break;
		case Usage::UseBoth:
			CreateColorBuffer(device, size, colorBufferFormat, makeRtvSrv);
			CreateDepthBuffer(device, size, depthBufferFormat, makeDsvSrv);
			break;
		}

		// Create viewport
		_viewport.TopLeftX = 0.0f;
		_viewport.TopLeftY = 0.0f;
		_viewport.Width = size.x;
		_viewport.Height = size.y;
		_viewport.MinDepth = 0.0f;
		_viewport.MaxDepth = 1.0f;

		_usage = usage;
    }

	void FrameBuffer::Clear(ID3D11DeviceContext* context, const Color& clearColor)
	{
		if (_usage == Usage::UseBoth || _usage == Usage::UseRenderTargetView)
		{
			context->ClearRenderTargetView(_colorBuffer->_renderTargetView.Get(), &clearColor.color.x);
		}

		if (_usage == Usage::UseBoth || _usage == Usage::UseDepthStencilView)
		{
			context->ClearDepthStencilView(_depthBuffer->_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, static_cast<UINT8>(0));
		}
	}

	void FrameBuffer::Activate(ID3D11DeviceContext* context)
	{
		// Get cached viewport.
		{
			UINT numViewport = 1;
			context->RSGetViewports(&numViewport, &_cachedViewport);
		}

		// Get cached rtv and dsv.
		{
			switch (_usage)
			{
			case Usage::UseRenderTargetView:
				context->OMGetRenderTargets(
				    1, _colorBuffer->_cachedRenderTargetView.ReleaseAndGetAddressOf(), nullptr
				);
				break;
			case Usage::UseDepthStencilView:
				context->OMGetRenderTargets(
				    1, _depthBuffer->_cachedRenderTargetView.ReleaseAndGetAddressOf(),
				    _depthBuffer->_cachedDepthStencilView.ReleaseAndGetAddressOf()
				);
				break;
			case Usage::UseBoth:
				context->OMGetRenderTargets(
				1, _colorBuffer->_cachedRenderTargetView.ReleaseAndGetAddressOf(), _depthBuffer->_cachedDepthStencilView.ReleaseAndGetAddressOf()
			    );
				break;
			}
		}

		// Set blend.
		{
			constexpr FLOAT blendFactor[4] = { 1, 1, 1, 1 };

			ID3D11BlendState* blendState = BlendState::GetBuildInBlendStatesAt(_blendMode);
			context->OMSetBlendState(blendState, blendFactor, 0xFFFFFFFF);
		}

		// Set depth stencil state.
		{
			ID3D11DepthStencilState* depthStencilState = DepthStencilState::GetBuildInDepthStencilStatesAt(DepthStencilState::Type::DepthOnStencilOn);
			context->OMSetDepthStencilState(depthStencilState, 0);
		}

		// Set rasterizer state.
		{
			ID3D11RasterizerState* rasterizerState = RasterizerState::GetBuildInRasterizerStatesAt(RasterizerState::Type::FillCullBack);
			context->RSSetState(rasterizerState);
		}

		// Set rtv and dsv.
		{
			switch (_usage)
			{
			case Usage::UseRenderTargetView:
				context->OMSetRenderTargets(
					1, _colorBuffer->_renderTargetView.GetAddressOf(), nullptr
				);
				break;
			case Usage::UseDepthStencilView:
				context->OMSetRenderTargets(
				    0, nullptr, _depthBuffer->_depthStencilView.Get()
				);
				break;
			case Usage::UseBoth:
				context->OMSetRenderTargets(
				    1, _colorBuffer->_renderTargetView.GetAddressOf(), _depthBuffer->_depthStencilView.Get()
				);
				break;
			}
		}

		// Set viewport.
		{
			context->RSSetViewports(
				1, &_viewport
			);
		}
	}

	void FrameBuffer::DeActivate(ID3D11DeviceContext* context)
	{
		constexpr UINT numViews = 1;
		constexpr UINT numViewport = 1;

		{
			switch (_usage)
			{
			case Usage::UseRenderTargetView:
				context->OMSetRenderTargets(
				    numViews, _colorBuffer->_cachedRenderTargetView.GetAddressOf(), nullptr
				);
				break;
			case Usage::UseDepthStencilView:
				context->OMSetRenderTargets(
				    numViews, _depthBuffer->_cachedRenderTargetView.GetAddressOf(), _depthBuffer->_cachedDepthStencilView.Get()
				);
				break;
			case Usage::UseBoth:
				context->OMSetRenderTargets(
				    numViews,
				_colorBuffer->_cachedRenderTargetView.GetAddressOf(),
				_depthBuffer->_cachedDepthStencilView.Get()
			    );
				break;
			}
		}
		
		context->RSSetViewports(
			numViewport, &_cachedViewport
		);
	}

	void FrameBuffer::OnResize(ID3D11Device* device, const Vector2& size)
	{
		if (_usage == Usage::UseRenderTargetView || _usage == Usage::UseBoth)
		{
			_colorBuffer->OnResize(device, size);
		}

		if (_usage == Usage::UseDepthStencilView || _usage == Usage::UseBoth)
		{
			_depthBuffer->OnResize(device, size);
		}

		_viewport.Width = size.x;
		_viewport.Height = size.y;
	}

	int FrameBuffer::ReadPixel(ID3D11Device* device, ID3D11DeviceContext* context,
		ID3D11ShaderResourceView* bufferResource, DXGI_FORMAT readBufferFormat, const Vector2& readBufferSize,
		const Vector2& fetchPosition) const
	{
		return ReadableBuffer::ReadPixel(device, context, bufferResource, readBufferFormat, readBufferSize, fetchPosition);
	}

    void FrameBuffer::CreateColorBuffer(ID3D11Device* device, const Vector2& size, DXGI_FORMAT format, bool makeSrv)
    {
		_colorBuffer = std::make_shared<ColorBuffer>();
		_colorBuffer->Create(device, size, format, makeSrv);
    }

    void FrameBuffer::CreateDepthBuffer(ID3D11Device* device, const Vector2& size, DXGI_FORMAT format, bool makeSrv)
    {
		_depthBuffer = std::make_shared<DepthBuffer>();
		_depthBuffer->Create(device, size, format, makeSrv);
    }
};