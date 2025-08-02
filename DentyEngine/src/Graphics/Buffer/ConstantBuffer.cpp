#include "stdafx.h"

#include "ConstantBuffer.h"

namespace DentyEngine
{
    ConstantBuffer::ConstantBuffer(ID3D11Device* device, const size_t sizeOfType)
    {
        DENTY_ASSERT(sizeOfType % 16 == 0, "The constant buffer must be a multiple of 16.");

        // Create constant buffer
        {
            HRESULT hr = S_OK;

            D3D11_BUFFER_DESC bufferDesc = { };
            bufferDesc.ByteWidth = static_cast<UINT>(sizeOfType);
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = 0;

            hr = device->CreateBuffer(
                &bufferDesc, nullptr, _buffer.ReleaseAndGetAddressOf()
            );

            _ASSERT_EXPR(SUCCEEDED(hr), Utils::HRTrace(hr));
        }
    }

    ConstantBuffer& ConstantBuffer::operator=(const ConstantBuffer& source)
    {
        source._buffer.CopyTo(_buffer.ReleaseAndGetAddressOf());

        return *this;
    }

    void ConstantBuffer::UpdateSubresource(ID3D11DeviceContext* context, const void* constants)
    {
        context->UpdateSubresource(
            _buffer.Get(),
            0,
            nullptr,
            constants,
            0, 0
        );
    }

    void ConstantBuffer::BindVertexShader(ID3D11DeviceContext* context, UINT startSlot)
    {
        context->VSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());
    }

    void ConstantBuffer::BindPixelShader(ID3D11DeviceContext* context, UINT startSlot)
    {
        context->PSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());
    }

    void ConstantBuffer::BindHullShader(ID3D11DeviceContext* context, UINT startSlot)
    {
        context->HSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());

    }

    void ConstantBuffer::BindDomainShader(ID3D11DeviceContext* context, UINT startSlot)
    {
        context->DSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());

    }

    void ConstantBuffer::BindGeometryShader(ID3D11DeviceContext* context, UINT startSlot)
    {
        context->GSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());

    }

    void ConstantBuffer::BindComputeShader(ID3D11DeviceContext* context, UINT startSlot)
    {
        context->CSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());

    }

    void ConstantBuffer::BindAll(ID3D11DeviceContext* context, const UINT startSlot)
    {
        context->VSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());
        context->PSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());
        context->HSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());
        context->DSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());
        context->GSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());
        context->CSSetConstantBuffers(startSlot, 1, _buffer.GetAddressOf());
    }
}