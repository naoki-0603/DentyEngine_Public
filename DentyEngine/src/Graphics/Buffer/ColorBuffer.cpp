#include "stdafx.h"

#include "ColorBuffer.h"

namespace DentyEngine
{
    ColorBuffer::ColorBuffer() :
        _buffer(), _renderTargetView(), _cachedRenderTargetView(),
        _shaderResourceView(), _size(), _format()
    {
    }

    void ColorBuffer::CopyTo(ID3D11Resource** resource) const
    {
        _shaderResourceView->GetResource(resource);
    }

    void ColorBuffer::OnResize(ID3D11Device* device, const Vector2& size)
    {
        Create(device, size, _format);
    }

    ID3D11ShaderResourceView* const* ColorBuffer::GetResource() const
    {
        return _shaderResourceView.GetAddressOf();
    }

    bool ColorBuffer::IsValid() const
    {
        return (_shaderResourceView != nullptr);
    }

    void ColorBuffer::Create(ID3D11Device* device, const Vector2& size, const DXGI_FORMAT format, bool makeSrv)
    {
        HRESULT hr = S_OK;

        // Create buffer
        {
            D3D11_TEXTURE2D_DESC texture2dDesc = {};
            texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
            texture2dDesc.Width = static_cast<UINT>(size.x);
            texture2dDesc.Height = static_cast<UINT>(size.y);
            texture2dDesc.Format = format;
            texture2dDesc.BindFlags = (makeSrv ? (D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE) : D3D11_BIND_RENDER_TARGET);
            texture2dDesc.MipLevels = 1;
            texture2dDesc.ArraySize = 1;
            texture2dDesc.SampleDesc.Count = 1;
            texture2dDesc.SampleDesc.Quality = 0;

            hr = device->CreateTexture2D(
                &texture2dDesc, nullptr, _buffer.ReleaseAndGetAddressOf()
            );
            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        // Create view
        {
            D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
            viewDesc.Format = format;
            viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

            hr = device->CreateRenderTargetView(
                _buffer.Get(), &viewDesc, _renderTargetView.ReleaseAndGetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        // Create shader resource view
        if (makeSrv)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
            shaderResourceViewDesc.Format = format;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

            hr = device->CreateShaderResourceView(
                _buffer.Get(), &shaderResourceViewDesc, _shaderResourceView.ReleaseAndGetAddressOf()
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }


        _size = size;
        _format = format;
    }
};