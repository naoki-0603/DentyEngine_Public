#include "stdafx.h"

#include "RasterizerState.h"

namespace DentyEngine
{
    std::vector<ComRef<ID3D11RasterizerState>> RasterizerState::_buildInRasterizerStates = {};
    bool RasterizerState::_created = false;

    RasterizerState::RasterizerState(ID3D11Device* device, const D3D11_RASTERIZER_DESC& desc)
    {
        // Create
        {
            HRESULT hr = S_OK;

            hr = device->CreateRasterizerState(
                &desc, _state.GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }
    }

    void RasterizerState::CreateBuildInRasterizerStates(ID3D11Device* device)
    {
        _buildInRasterizerStates.resize(static_cast<int>(Type::Max));

        // Create
        {
            HRESULT hr = S_OK;

            // FILL_SOLID, CULL_BACK
            D3D11_RASTERIZER_DESC rasterizerDesc = { };
            rasterizerDesc.FillMode = D3D11_FILL_SOLID;
            rasterizerDesc.CullMode = D3D11_CULL_BACK;
            rasterizerDesc.FrontCounterClockwise = FALSE;
            rasterizerDesc.ScissorEnable = FALSE;
            rasterizerDesc.MultisampleEnable = FALSE;
            rasterizerDesc.AntialiasedLineEnable = FALSE;
            rasterizerDesc.DepthClipEnable = TRUE;
            rasterizerDesc.DepthBias = 0;
            rasterizerDesc.DepthBiasClamp = 0.0f;
            rasterizerDesc.SlopeScaledDepthBias = 0.0f;

            hr = device->CreateRasterizerState(
                &rasterizerDesc, _buildInRasterizerStates[static_cast<int>(Type::FillCullBack)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));


            // WIREFRAME, CULL_BACK
            rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
            rasterizerDesc.CullMode = D3D11_CULL_BACK;

            hr = device->CreateRasterizerState(
                &rasterizerDesc, _buildInRasterizerStates[static_cast<int>(Type::WireframeCullBack)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // WIREFRAME, NO_CULL
            rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
            rasterizerDesc.CullMode = D3D11_CULL_NONE;

            hr = device->CreateRasterizerState(
                &rasterizerDesc, _buildInRasterizerStates[static_cast<int>(Type::WireframeNoCull)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // FILL_SOLID, CULL_BACK, CLOCKWISE
            rasterizerDesc.FillMode = D3D11_FILL_SOLID;
            rasterizerDesc.CullMode = D3D11_CULL_BACK;
            rasterizerDesc.FrontCounterClockwise = TRUE;

            hr = device->CreateRasterizerState(
                &rasterizerDesc, _buildInRasterizerStates[static_cast<int>(Type::FillCullBackClockwise)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // FILL_SOLID, NO_CULL
            rasterizerDesc.FillMode = D3D11_FILL_SOLID;
            rasterizerDesc.CullMode = D3D11_CULL_NONE;
            rasterizerDesc.FrontCounterClockwise = FALSE;

            hr = device->CreateRasterizerState(
                &rasterizerDesc, _buildInRasterizerStates[static_cast<int>(Type::FillNoCull)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        _created = true;
    }

    ID3D11RasterizerState* RasterizerState::GetBuildInRasterizerStatesAt(Type type)
    {
        return _buildInRasterizerStates.at(static_cast<int>(type)).Get();
    }
};