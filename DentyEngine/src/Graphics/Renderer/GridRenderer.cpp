#include "stdafx.h"

#include "GridRenderer.h"

#include "Graphics/Renderer/Shader.h"

#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"

#include "Graphics/States/BlendState.h"
#include "Graphics/States/DepthStencilState.h"
#include "Graphics/States/RasterizerState.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

namespace DentyEngine
{
	GridRendererSpecification::GridRendererSpecification() :
		vertices(), vertexBuffer(), shader(),
		cellSize(), gridLineThickness(), lineLength(),
		rowCount(), columnCount()
	{
	}

	GridRendererSpecification GridRenderer::_specification = { };
	bool GridRenderer::_enable = true;

	GridRenderer::GridRenderer()
	{
	}

	void GridRenderer::Create(ID3D11Device* device, const Vector2& cellSize, float gridLineThickness, float lineLength)
	{
		const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();
		_specification.shader = shaderLibrary->Find(ShaderLibrary::GRID_SHADER_PATH).value();
		DENTY_CHECK_NULL(_specification.shader)

		_specification.cellSize = cellSize;
		_specification.lineLength = lineLength;
		_specification.gridLineThickness = gridLineThickness;
		_specification.rowCount = static_cast<int32_t>(_specification.lineLength * 4.0f);
		_specification.columnCount = static_cast<int32_t>(_specification.lineLength * 4.0f);

		CreateVerticesAndBuffer(device);
	}

	void GridRenderer::Render(ID3D11DeviceContext* context)
	{
		ComRef<ID3D11BlendState> cachedBlendState = nullptr;
		ComRef<ID3D11DepthStencilState> cachedDepthStencilState = nullptr;
		ComRef<ID3D11RasterizerState> cachedRasterizerState = nullptr;

		float cachedBlendFactor[4] = { };
		UINT cachedSamplerMask = 0;
		UINT cachedStencilRef = 0;

		// Get cached.
		{
			context->OMGetBlendState(cachedBlendState.ReleaseAndGetAddressOf(), cachedBlendFactor, &cachedSamplerMask);
			context->OMGetDepthStencilState(cachedDepthStencilState.ReleaseAndGetAddressOf(), &cachedStencilRef);
			context->RSGetState(cachedRasterizerState.ReleaseAndGetAddressOf());
		}

		// Set pipeline.
		{
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			_specification.vertexBuffer->Bind();

			ID3D11DepthStencilState* depthStencilState = DepthStencilState::GetBuildInDepthStencilStatesAt(DepthStencilState::Type::DepthOnStencilOff);
			context->OMSetDepthStencilState(depthStencilState, 0);

			constexpr float blendFactor[4] = { 1, 1, 1, 1 };
			constexpr UINT samplerMask = 0xFFFFFFFF;
			ID3D11BlendState* blendState = BlendState::GetBuildInBlendStatesAt(BlendState::Type::Alpha);
			context->OMSetBlendState(blendState, blendFactor, samplerMask);

			ID3D11RasterizerState* rasterizerState = RasterizerState::GetBuildInRasterizerStatesAt(RasterizerState::Type::FillNoCull);
			context->RSSetState(rasterizerState);

			_specification.shader->BindVertexShader(context);
			_specification.shader->BindPixelShader(context);

			context->Draw(static_cast<UINT>(_specification.vertices.size()), 0);
		}

		// Set cached.
		{
			context->OMSetBlendState(cachedBlendState.Get(), cachedBlendFactor, cachedSamplerMask);
			context->OMSetDepthStencilState(cachedDepthStencilState.Get(), cachedStencilRef);
			context->RSSetState(cachedRasterizerState.Get());
		}
	}

	void GridRenderer::CreateVerticesAndBuffer([[maybe_unused]] ID3D11Device* device)
	{
		// Create vertex.
		{
			const float startPositionZ = _specification.lineLength;

			// Row line first.
			for (int row = 0; row < _specification.rowCount; ++row)
			{
				const float lineThickness = _specification.gridLineThickness * 0.5f;
				const float positionZ = startPositionZ - _specification.cellSize.y * static_cast<float>(row);

				Vertex leftTopVertex = { };
				leftTopVertex.position = { -_specification.lineLength, 0.0f, positionZ + lineThickness, 1.0f };

				Vertex rightTopVertex = { };
				rightTopVertex.position = { +_specification.lineLength, 0.0f, positionZ + lineThickness, 1.0f };

				Vertex leftBottomVertex = { };
				leftBottomVertex.position = { -_specification.lineLength, 0.0f, positionZ - lineThickness, 1.0f };

				Vertex rightBottomVertex = { };
				rightBottomVertex.position = { +_specification.lineLength, 0.0f, positionZ - lineThickness, 1.0f };

				_specification.vertices.emplace_back(leftTopVertex);
				_specification.vertices.emplace_back(leftBottomVertex);
				_specification.vertices.emplace_back(rightTopVertex);

				_specification.vertices.emplace_back(leftBottomVertex);
				_specification.vertices.emplace_back(rightTopVertex);
				_specification.vertices.emplace_back(rightBottomVertex);
			}

			const float startPositionX = _specification.lineLength;

			// Column line.
			for (int column = 0; column < _specification.columnCount; ++column)
			{
				const float lineThickness = _specification.gridLineThickness * 0.5f;
				const float positionX = startPositionX - _specification.cellSize.x * static_cast<float>(column);

				Vertex leftTopVertex = { };
				leftTopVertex.position = { positionX + lineThickness , 0.0f, -_specification.lineLength, 1.0f };

				Vertex rightTopVertex = { };
				rightTopVertex.position = { positionX + lineThickness, 0.0f,  +_specification.lineLength, 1.0f };

				Vertex leftBottomVertex = { };
				leftBottomVertex.position = { positionX - lineThickness, 0.0f,  -_specification.lineLength, 1.0f };

				Vertex rightBottomVertex = { };
				rightBottomVertex.position = { positionX - lineThickness, 0.0f,  +_specification.lineLength, 1.0f };

				_specification.vertices.emplace_back(leftTopVertex);
				_specification.vertices.emplace_back(leftBottomVertex);
				_specification.vertices.emplace_back(rightTopVertex);

				_specification.vertices.emplace_back(leftBottomVertex);
				_specification.vertices.emplace_back(rightTopVertex);
				_specification.vertices.emplace_back(rightBottomVertex);
			}
		}

		// Create vertex buffer.
		{
			using VertexType = DXVertexBuffer::VertexBufferInfo::VertexType;
			using AccessFlag = IDirectXBuffer::CpuAccessFlag;
			using Usage = IDirectXBuffer::Usage;
			using MapType = IDirectXBuffer::MapType;

			DXVertexBuffer::VertexBufferInfo vertexBufferInfo = {};
			vertexBufferInfo.usage = Usage::Default;
			vertexBufferInfo.accessFlag = AccessFlag::NoAccessWriteAndRead;
			vertexBufferInfo.mapType = MapType::None;
			vertexBufferInfo.vertexType = VertexType::Vertex;
			vertexBufferInfo.stride = sizeof(Vertex);
			vertexBufferInfo.size = sizeof(Vertex) * static_cast<uint32_t>(_specification.vertices.size());
			vertexBufferInfo.count = static_cast<uint32_t>(_specification.vertices.size());
			vertexBufferInfo.data = std::make_shared<Vertex[]>(_specification.vertices.size());
			vertexBufferInfo.AssignData(_specification.vertices.data());

			_specification.vertexBuffer = std::make_shared<DXVertexBuffer>();
			_specification.vertexBuffer->Create(vertexBufferInfo);
		}
	}
}