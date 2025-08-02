#include "stdafx.h"

#include "DXIndexBuffer.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
	DXIndexBuffer::DXIndexBuffer() :
		IDirectXBuffer(), _info()
	{
	}

	DXIndexBuffer::DXIndexBuffer(const DXIndexBuffer& source) :
		_info(source._info)
	{
		_info.data = std::make_shared<uint32_t[]>(_info.count);
		_info.AssignData(source._info.data.get());

		// Create buffer.
		{
			const D3D11_BUFFER_DESC bufferDesc =
			{
				.ByteWidth = _info.size,
				.Usage = static_cast<D3D11_USAGE>(_info.usage),
				.BindFlags = D3D11_BIND_INDEX_BUFFER,
				.CPUAccessFlags = static_cast<UINT>(_info.accessFlag),
				.MiscFlags = 0,
				.StructureByteStride = 0,
			};

			const D3D11_SUBRESOURCE_DATA subresourceData =
			{
				.pSysMem = _info.data.get(),
				.SysMemPitch = 0,
				.SysMemSlicePitch = 0,
			};

			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			const auto hr = device->CreateBuffer(
				&bufferDesc, &subresourceData, _info.buffer.ReleaseAndGetAddressOf()
			);
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
		}
	}

	DXIndexBuffer& DXIndexBuffer::operator=(const DXIndexBuffer& source)
	{
		_info = source._info;

		_info.data = std::make_shared<uint32_t[]>(_info.count);
		_info.AssignData(source._info.data.get());

		// Create buffer.
		{
			const D3D11_BUFFER_DESC bufferDesc =
			{
				.ByteWidth = _info.size,
				.Usage = static_cast<D3D11_USAGE>(_info.usage),
				.BindFlags = D3D11_BIND_INDEX_BUFFER,
				.CPUAccessFlags = static_cast<UINT>(_info.accessFlag),
				.MiscFlags = 0,
				.StructureByteStride = 0,
			};

			const D3D11_SUBRESOURCE_DATA subresourceData =
			{
				.pSysMem = _info.data.get(),
				.SysMemPitch = 0,
				.SysMemSlicePitch = 0,
			};

			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			const auto hr = device->CreateBuffer(
				&bufferDesc, &subresourceData, _info.buffer.ReleaseAndGetAddressOf()
			);
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
		}

		return *this;
	}

	void DXIndexBuffer::Create(const IndexBufferInfo& info)
	{
		// Create buffer.
		D3D11_BUFFER_DESC bufferDesc = { };
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = static_cast<D3D11_CPU_ACCESS_FLAG>(info.accessFlag);
		bufferDesc.ByteWidth = info.size;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.Usage = static_cast<D3D11_USAGE>(info.usage);

		// Create sub resource data.
		D3D11_SUBRESOURCE_DATA subresourceData = { };
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;
		subresourceData.pSysMem = info.data.get();

		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		// Create buffer.
		const auto hr = device->CreateBuffer(
			&bufferDesc, &subresourceData, _info.buffer.ReleaseAndGetAddressOf()
		);
		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

		_info = info;
	}

	void* DXIndexBuffer::Map()
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
		context->Map(_info.buffer.Get(), 0, static_cast<D3D11_MAP>(_info.mapType), 0, &mappedSubresource);

		if (mappedSubresource.pData)
		{
			return mappedSubresource.pData;
		}

		return nullptr;
	}

	void DXIndexBuffer::UnMap() const
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		context->Unmap(_info.buffer.Get(), 0);
	}

	void DXIndexBuffer::Bind() const
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		context->IASetIndexBuffer(_info.buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DXIndexBuffer::Clone(const Ref<IDirectXBuffer>& buffer) const
	{
		auto&& target = std::static_pointer_cast<DXIndexBuffer>(buffer);

		target->_info = _info;

		target->_info.data = std::make_shared<uint32_t[]>(_info.count);
		target->_info.AssignData(_info.data.get());

		// Create buffer.
		{
			const D3D11_BUFFER_DESC bufferDesc =
			{
				.ByteWidth = _info.size,
				.Usage = static_cast<D3D11_USAGE>(_info.usage),
				.BindFlags = D3D11_BIND_INDEX_BUFFER,
				.CPUAccessFlags = static_cast<UINT>(_info.accessFlag),
				.MiscFlags = 0,
				.StructureByteStride = 0,
			};

			const D3D11_SUBRESOURCE_DATA subresourceData =
			{
				.pSysMem = _info.data.get(),
				.SysMemPitch = 0,
				.SysMemSlicePitch = 0,
			};

			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			const auto hr = device->CreateBuffer(
				&bufferDesc, &subresourceData, target->_info.buffer.ReleaseAndGetAddressOf()
			);
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
		}
	}
}
