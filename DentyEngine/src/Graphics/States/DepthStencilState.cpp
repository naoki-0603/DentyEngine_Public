#include "stdafx.h"

#include "DepthStencilState.h"

namespace DentyEngine
{
    std::vector<ComRef<ID3D11DepthStencilState>> DepthStencilState::_buildInDepthStencilStates = {};
    bool DepthStencilState::_created = false;

    DepthStencilState::DepthStencilState(ID3D11Device* device, const D3D11_DEPTH_STENCIL_DESC& desc)
    {
        HRESULT hr = S_OK;

        hr = device->CreateDepthStencilState(
            &desc, _state.GetAddressOf()
        );

        DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
    }

    void DepthStencilState::CreateBuildInDepthStencilStates(ID3D11Device* device)
    {
        DENTY_ASSERT(!_created, "Already created.");

        _buildInDepthStencilStates.resize(static_cast<int>(Type::Max));

        // Create
        {
            HRESULT hr = S_OK;

            // depth on, stencil on
            D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { };
            depthStencilDesc.DepthEnable = TRUE;
            depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
            depthStencilDesc.StencilEnable = TRUE;
            depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
            depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
            depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
            depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;

            hr = device->CreateDepthStencilState(
                &depthStencilDesc, _buildInDepthStencilStates[static_cast<int>(Type::DepthOnStencilOn)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // depth on, stencil off 
            depthStencilDesc.DepthEnable = TRUE;
            depthStencilDesc.StencilEnable = FALSE;

            hr = device->CreateDepthStencilState(
                &depthStencilDesc, _buildInDepthStencilStates[static_cast<int>(Type::DepthOnStencilOff)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // depth off, stencil on
            depthStencilDesc.DepthEnable = FALSE;
            depthStencilDesc.StencilEnable = TRUE;

            hr = device->CreateDepthStencilState(
                &depthStencilDesc, _buildInDepthStencilStates[static_cast<int>(Type::DepthOffStencilOn)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // depth off, stencil off
            depthStencilDesc.DepthEnable = FALSE;
            depthStencilDesc.StencilEnable = FALSE;

            hr = device->CreateDepthStencilState(
                &depthStencilDesc, _buildInDepthStencilStates[static_cast<int>(Type::DepthOffStencilOff)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // depth on, write off, stencil off
            depthStencilDesc.DepthEnable = TRUE;
            depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

            hr = device->CreateDepthStencilState(
                &depthStencilDesc, _buildInDepthStencilStates[static_cast<int>(Type::DepthOnWriteOffStencilOff)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        _created = true;
    }

    ID3D11DepthStencilState* DepthStencilState::GetBuildInDepthStencilStatesAt(Type type)
    {
        return _buildInDepthStencilStates.at(static_cast<int>(type)).Get();
    }
};