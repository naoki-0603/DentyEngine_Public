#include "stdafx.h"

#include "DXVertexBuffer.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
	DXVertexBuffer::DXVertexBuffer() :
		IDirectXBuffer(), _info()
	{
	}

	DXVertexBuffer::DXVertexBuffer(const DXVertexBuffer& source) :
		_info(source._info)
	{
		using VertexType = VertexBufferInfo::VertexType;

		switch (_info.vertexType)
		{
		case VertexType::Vertex:
			_info.data = std::make_shared<Vertex[]>(_info.count);
			break;
		case VertexType::MeshVertex:
			_info.data = std::make_shared<MeshVertex[]>(_info.count);
			break;
		case VertexType::LineVertex:
			_info.data = std::make_shared<LineVertex[]>(_info.count);
			break;
		}
		_info.AssignData(source._info.data.get());

		// Create buffer.
		{
			const D3D11_BUFFER_DESC bufferDesc =
			{
				.ByteWidth = _info.size,
				.Usage = static_cast<D3D11_USAGE>(_info.usage),
				.BindFlags = D3D11_BIND_VERTEX_BUFFER,
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

	DXVertexBuffer& DXVertexBuffer::operator=(const DXVertexBuffer& source)
	{
		using VertexType = VertexBufferInfo::VertexType;

		_info = source._info;

		switch (_info.vertexType)
		{
		case VertexType::Vertex:
			_info.data = std::make_shared<Vertex[]>(_info.count);
			break;
		case VertexType::MeshVertex:
			_info.data = std::make_shared<MeshVertex[]>(_info.count);
			break;
		case VertexType::LineVertex:
			_info.data = std::make_shared<LineVertex[]>(_info.count);
			break;
		}
		_info.AssignData(source._info.data.get());

		// Create buffer.
		{
			const D3D11_BUFFER_DESC bufferDesc =
			{
				.ByteWidth = _info.size,
				.Usage = static_cast<D3D11_USAGE>(_info.usage),
				.BindFlags = D3D11_BIND_VERTEX_BUFFER,
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

	void DXVertexBuffer::Create(const VertexBufferInfo& info)
	{
		// Create buffer.
		{
			D3D11_BUFFER_DESC bufferDesc = { };
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = static_cast<D3D11_CPU_ACCESS_FLAG>(info.accessFlag);
			bufferDesc.ByteWidth = info.size;
			bufferDesc.StructureByteStride = 0;
			bufferDesc.Usage = static_cast<D3D11_USAGE>(info.usage);

			// Create sub resource data.
			D3D11_SUBRESOURCE_DATA subresourceData = { };
			subresourceData.SysMemPitch = 0;
			subresourceData.SysMemSlicePitch = 0;
			subresourceData.pSysMem = info.data.get();

			// Create buffer.
			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			const auto hr = device->CreateBuffer(
				&bufferDesc, &subresourceData, _info.buffer.ReleaseAndGetAddressOf()
			);
			DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
		}

		// Store info.
		_info = info;
	}

	void* DXVertexBuffer::Map()
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};

		context->Map(
			_info.buffer.Get(),
			0,
			static_cast<D3D11_MAP>(_info.mapType),
			0,
			&mappedSubresource
		);

		if (mappedSubresource.pData)
		{
			return mappedSubresource.pData;
		}

		return nullptr;
	}

	void DXVertexBuffer::UnMap() const
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		context->Unmap(_info.buffer.Get(), 0);
	}

	void DXVertexBuffer::Bind() const
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		context->IASetVertexBuffers(0, 1, _info.buffer.GetAddressOf(), &_info.stride, &_info.offset);
	}

	void DXVertexBuffer::Clone(const Ref<IDirectXBuffer>& buffer) const
	{
		using VertexType = VertexBufferInfo::VertexType;

		auto&& target = std::static_pointer_cast<DXVertexBuffer>(buffer);

		target->_info = _info;

		switch (_info.vertexType)
		{
		case VertexType::Vertex:
			target->_info.data = std::make_shared<Vertex[]>(target->_info.count);
			break;
		case VertexType::MeshVertex:
			target->_info.data = std::make_shared<MeshVertex[]>(target->_info.count);
			break;
		case VertexType::LineVertex:
			target->_info.data = std::make_shared<LineVertex[]>(target->_info.count);
			break;
		}
		
		target->_info.AssignData(_info.data.get());

		// Create buffer.
		{
			const D3D11_BUFFER_DESC bufferDesc =
			{
				.ByteWidth = target->_info.size,
				.Usage = static_cast<D3D11_USAGE>(target->_info.usage),
				.BindFlags = D3D11_BIND_VERTEX_BUFFER,
				.CPUAccessFlags = static_cast<UINT>(target->_info.accessFlag),
				.MiscFlags = 0,
				.StructureByteStride = 0,
			};

			const D3D11_SUBRESOURCE_DATA subresourceData =
			{
				.pSysMem = target->_info.data.get(),
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
