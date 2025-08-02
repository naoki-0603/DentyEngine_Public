#include "stdafx.h"

#include "SceneBuffer.h"

#include "ColorBuffer.h"
#include "DepthBuffer.h"

#include "Graphics/States/BlendState.h"
#include "Graphics/States/DepthStencilState.h"
#include "Graphics/States/RasterizerState.h"

namespace DentyEngine
{
	SceneBuffer::SceneBuffer() :
		ReadableBuffer(),
		_colorBuffer(), _entityIDBuffer(),
		_depthBuffer(), _blendState(), _viewport(), _cachedViewport()
	{
	}

    void SceneBuffer::Create(ID3D11Device* device, const Vector2& size)
    {
		// Create buffers
		_colorBuffer = Utils::CreateRef<ColorBuffer>();
		_colorBuffer->Create(device, size, DXGI_FORMAT_R16G16B16A16_FLOAT);

		_entityIDBuffer = Utils::CreateRef<ColorBuffer>();
		_entityIDBuffer->Create(device, size, DXGI_FORMAT_R32_UINT);

		_depthBuffer = Utils::CreateRef<DepthBuffer>();
		_depthBuffer->Create(device, size, DXGI_FORMAT_R32_TYPELESS);

		// Create viewport
		_viewport.TopLeftX = 0.0f;
		_viewport.TopLeftY = 0.0f;
		_viewport.Width = size.x;
		_viewport.Height = size.y;
		_viewport.MinDepth = 0.0f;
		_viewport.MaxDepth = 1.0f;

		// Create blend state
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.IndependentBlendEnable = TRUE;
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[1].BlendEnable = FALSE;
		blendDesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		_blendState = Utils::CreateRef<BlendState>(device, blendDesc);
    }

	void SceneBuffer::Clear(ID3D11DeviceContext* context, const Color& clearColor)
	{
		context->ClearRenderTargetView(_colorBuffer->_renderTargetView.Get(), &clearColor.color.x);
		context->ClearRenderTargetView(_entityIDBuffer->_renderTargetView.Get(), &clearColor.color.x);
		context->ClearDepthStencilView(_depthBuffer->_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, static_cast<UINT8>(0));
	}

	void SceneBuffer::Activate(ID3D11DeviceContext* context)
	{
		UINT numViewport = 1;

		context->RSGetViewports(&numViewport, &_cachedViewport);
		context->OMGetRenderTargets(
			1, _colorBuffer->_cachedRenderTargetView.ReleaseAndGetAddressOf(), _depthBuffer->_cachedDepthStencilView.ReleaseAndGetAddressOf()
		);

		constexpr FLOAT blendFactor[4] = { 1, 1, 1, 1 };
		context->OMSetBlendState(_blendState->GetState(), blendFactor, 0xFFFFFFFF);

		ID3D11DepthStencilState* depthStencilState = DepthStencilState::GetBuildInDepthStencilStatesAt(DepthStencilState::Type::DepthOnStencilOn);
		context->OMSetDepthStencilState(depthStencilState, 0);

		ID3D11RasterizerState* rasterizerState = RasterizerState::GetBuildInRasterizerStatesAt(RasterizerState::Type::FillCullBack);
		context->RSSetState(rasterizerState);

		ID3D11RenderTargetView* renderTargetViews[] =
		{
			_colorBuffer->_renderTargetView.Get(),
			_entityIDBuffer->_renderTargetView.Get()
		};

		context->OMSetRenderTargets(
			_countof(renderTargetViews), renderTargetViews, _depthBuffer->_depthStencilView.Get()
		);

		context->RSSetViewports(
			1, &_viewport
		);
	}

	void SceneBuffer::DeActivate(ID3D11DeviceContext* context)
	{
		constexpr UINT numViews = 1;
		constexpr UINT numViewport = 1;

		context->OMSetRenderTargets(
			numViews,
			_colorBuffer->_cachedRenderTargetView.GetAddressOf(),
			_depthBuffer->_cachedDepthStencilView.Get()
		);

		context->RSSetViewports(
			numViewport, &_cachedViewport
		);
	}

	void SceneBuffer::OnResize(ID3D11Device* device, const Vector2& size)
	{
		_colorBuffer->OnResize(device, size);
		_entityIDBuffer->OnResize(device, size);
		_depthBuffer->OnResize(device, size);

		_viewport.Width = size.x;
		_viewport.Height = size.y;
	}

    int SceneBuffer::ReadPixel(ID3D11Device* device, ID3D11DeviceContext* context,
                               ID3D11ShaderResourceView* bufferResource, DXGI_FORMAT readBufferFormat,
                               const Vector2& readBufferSize, const Vector2& fetchPosition) const
	{
		return ReadableBuffer::ReadPixel(device, context, bufferResource, readBufferFormat, readBufferSize, fetchPosition);
	}
};