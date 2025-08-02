#include "stdafx.h"

#include "DepthBuffer.h"

namespace DentyEngine
{
    DepthBuffer::DepthBuffer() :
        _cachedRenderTargetView(), _depthStencilView(), _cachedDepthStencilView(),
        _shaderResourceView(), _size(), _format()
    {
    }

    void DepthBuffer::OnResize(ID3D11Device* device, const Vector2& size)
    {
        Create(device, size, _format);
    }

    ID3D11ShaderResourceView* const* DepthBuffer::GetResource() const
    {
        return _shaderResourceView.GetAddressOf();
    }

    bool DepthBuffer::IsValid() const
    {
        return (_shaderResourceView != nullptr);
    }

    void DepthBuffer::Create(ID3D11Device* device, const Vector2& size, const DXGI_FORMAT format, bool makeSrv)
    {
        HRESULT hr = S_OK;

        ComRef<ID3D11Texture2D> buffer = {};

        // Create buffer
        {
            D3D11_TEXTURE2D_DESC texture2dDesc = {};
            texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
            texture2dDesc.Width = static_cast<UINT>(size.x);
            texture2dDesc.Height = static_cast<UINT>(size.y);
            texture2dDesc.Format = format;
            texture2dDesc.BindFlags = (makeSrv ? (D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE) : D3D11_BIND_DEPTH_STENCIL);
            texture2dDesc.MipLevels = 1;
            texture2dDesc.ArraySize = 1;
            texture2dDesc.SampleDesc.Count = 1;
            texture2dDesc.SampleDesc.Quality = 0;

            hr = device->CreateTexture2D(
                &texture2dDesc, nullptr, buffer.ReleaseAndGetAddressOf()
            );
            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        // Create view
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
            viewDesc.Format = (format == DXGI_FORMAT_R32_TYPELESS ? DXGI_FORMAT_D32_FLOAT : DXGI_FORMAT_R24G8_TYPELESS);
            viewDesc.Flags = 0;
            viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

            hr = device->CreateDepthStencilView(
                buffer.Get(), &viewDesc, _depthStencilView.ReleaseAndGetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        // Create shader resource view
        if (makeSrv)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
            shaderResourceViewDesc.Format = (format == DXGI_FORMAT_R32_TYPELESS ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
            shaderResourceViewDesc.Texture2D.MipLevels = 1;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

            hr = device->CreateShaderResourceView(
                buffer.Get(), &shaderResourceViewDesc, _shaderResourceView.ReleaseAndGetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        _format = format;
        _size = size;
    }
};