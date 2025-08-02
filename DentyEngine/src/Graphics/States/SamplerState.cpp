#include "stdafx.h"

#include "SamplerState.h"

namespace DentyEngine
{
    std::vector<ComRef<ID3D11SamplerState>> SamplerState::_buildInSamplerStates = { };
    bool SamplerState::_created = false;

    SamplerState::SamplerState(ID3D11Device* device, const D3D11_SAMPLER_DESC& desc)
    {
        // Create state
        {
            HRESULT hr = S_OK;

            hr = device->CreateSamplerState(
                &desc, _state.GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), L"Failed to create sampler state (in SamplerState::SamplerState)");
        }
    }

    void SamplerState::CreateBuildInSamplerStates(ID3D11Device* device)
    {
        DENTY_ASSERT(!_created, L"Already initialize (in SamplerState::CreateDefaultSamplerStates)");

        _buildInSamplerStates.resize(static_cast<size_t>(Type::Max));

        // Create default sampler states
        {
            HRESULT hr = S_OK;

            // Point wrap
            D3D11_SAMPLER_DESC samplerDesc = {};
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.BorderColor[0] = 1.0f;
            samplerDesc.BorderColor[1] = 1.0f;
            samplerDesc.BorderColor[2] = 1.0f;
            samplerDesc.BorderColor[3] = 1.0f;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            hr = device->CreateSamplerState(
                &samplerDesc, _buildInSamplerStates[static_cast<int>(Type::PointWrap)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // Linear wrap
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

            hr = device->CreateSamplerState(
                &samplerDesc, _buildInSamplerStates[static_cast<int>(Type::LinearWrap)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // Anisotropic wrap
            samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
            samplerDesc.MaxAnisotropy = 16;

            hr = device->CreateSamplerState(
                &samplerDesc, _buildInSamplerStates[static_cast<int>(Type::AnisotropicWrap)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            // Comparison linear border
            samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL; //D3D11_COMPARISON_LESS_EQUAL
            samplerDesc.BorderColor[0] = 1;
            samplerDesc.BorderColor[1] = 1;
            samplerDesc.BorderColor[2] = 1;
            samplerDesc.BorderColor[3] = 1;

            hr = device->CreateSamplerState(
                &samplerDesc, _buildInSamplerStates[static_cast<int>(Type::ComparisonLinearBorder)].GetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }
    }

    ID3D11SamplerState* SamplerState::GetBuildInSamplerStatesAt(Type type)
    {
        return _buildInSamplerStates.at(static_cast<int>(type)).Get();
    }
};