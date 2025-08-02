#include "stdafx.h"

#include "BlendState.h"

namespace DentyEngine
{
	std::vector<ComRef<ID3D11BlendState>> BlendState::_buildInBlendStates = {};
	bool BlendState::_created = false;

    BlendState::BlendState(ID3D11Device* device, const D3D11_BLEND_DESC& desc)
    {
		// Create state
		{
            HRESULT hr = S_OK;

            hr = device->CreateBlendState(
				&desc, _state.GetAddressOf()
			);

			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
		}
    }

    void BlendState::CreateBuildInBlendStates(ID3D11Device* device)
    {
		DENTY_ASSERT(!_created, L"Already created.");

		_buildInBlendStates.resize(static_cast<int>(Type::Max));

		// Create blend states
		{
			HRESULT hr = S_OK;

			// No blend
			D3D11_BLEND_DESC blendDesc = { };
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = FALSE;
			blendDesc.RenderTarget[0].BlendEnable = FALSE;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			hr = device->CreateBlendState(&blendDesc, _buildInBlendStates[static_cast<int>(Type::NoBlend)].GetAddressOf());
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

			// Alpha
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			hr = device->CreateBlendState(&blendDesc, _buildInBlendStates[static_cast<int>(Type::Alpha)].GetAddressOf());
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

			// Add
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			hr = device->CreateBlendState(&blendDesc, _buildInBlendStates[static_cast<int>(Type::Add)].GetAddressOf());
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

			// Subtract
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_SUBTRACT;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			hr = device->CreateBlendState(&blendDesc, _buildInBlendStates[static_cast<int>(Type::Subtract)].GetAddressOf());
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

			// Multiply
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_SUBTRACT;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			hr = device->CreateBlendState(&blendDesc, _buildInBlendStates[static_cast<int>(Type::Multiply)].GetAddressOf());
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

			// AlphaCoverage
			blendDesc.AlphaToCoverageEnable = TRUE;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			hr = device->CreateBlendState(&blendDesc, _buildInBlendStates[static_cast<int>(Type::AlphaCoverage)].GetAddressOf());
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
		}

		_created = true;
    }

    ID3D11BlendState* BlendState::GetBuildInBlendStatesAt(Type type)
    {
		return _buildInBlendStates.at(static_cast<int>(type)).Get();
    }
};