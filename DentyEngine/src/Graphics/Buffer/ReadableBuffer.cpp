#include "stdafx.h"

#include "ReadableBuffer.h"

namespace DentyEngine
{
	int ReadableBuffer::ReadPixel(
		ID3D11Device* device, ID3D11DeviceContext* context,
		ID3D11ShaderResourceView* bufferResource,
		DXGI_FORMAT readBufferFormat,
		const Vector2& readBufferSize, const Vector2& fetchPosition
	) const
	{
		ComRef<ID3D11Texture2D> pixelData = {};
		D3D11_TEXTURE2D_DESC texture2dDesc = {};

		texture2dDesc.Width = 1;
		texture2dDesc.Height = 1;
		texture2dDesc.Format = readBufferFormat;
		texture2dDesc.ArraySize = 1;
		texture2dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		texture2dDesc.SampleDesc.Count = 1;
		texture2dDesc.SampleDesc.Quality = 0;
		texture2dDesc.Usage = D3D11_USAGE_STAGING;

		HRESULT hr = device->CreateTexture2D(
			&texture2dDesc, nullptr, pixelData.ReleaseAndGetAddressOf()
		);
		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

		if (fetchPosition.x < 0.0f || fetchPosition.x > readBufferSize.x)
		{
			return -1;
		}

		if (fetchPosition.y < 0.0f || fetchPosition.y > readBufferSize.y)
		{
			return -1;
		}

		const Vector2 mousePosition = fetchPosition;

		D3D11_BOX srcBox = {};
		srcBox.left = static_cast<UINT>(mousePosition.x);
		srcBox.right = srcBox.left + 1;
		srcBox.top = static_cast<UINT>(mousePosition.y);
		srcBox.bottom = srcBox.top + 1;
		srcBox.front = 0;
		srcBox.back = 1;

		ComRef<ID3D11Resource> resource;
		bufferResource->GetResource(resource.ReleaseAndGetAddressOf());

		context->CopySubresourceRegion(
			pixelData.Get(),
			0, 0, 0, 0,
			resource.Get(),
			0, &srcBox
		);

		// Get pixel data
		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
		hr = context->Map(
			pixelData.Get(), 0, D3D11_MAP_READ, 0, &mappedSubresource
		);
		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

		const auto pixel = static_cast<int*>(mappedSubresource.pData);
		if (!pixel)
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Failed to get pixel. (in ReadableBuffer::ReadPixels)";
			return -1;
		}

		context->Unmap(
			pixelData.Get(), 0
		);

		return (*pixel);
	}
}