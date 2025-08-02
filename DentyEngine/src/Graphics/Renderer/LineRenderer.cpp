#include "stdafx.h"

#include "LineRenderer.h"

#include "Shader.h"
#include "System/Graphics/GraphicsManager.h"

#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

namespace DentyEngine
{
	std::vector<LineVertex> LineRenderer::_vertices = {};
	Ref<DXVertexBuffer> LineRenderer::_vertexBuffer = nullptr;

	// For collision editor window.
	std::vector<LineVertex> LineRenderer::_verticesForCollisionEditorWindow = {};
	Ref<DXVertexBuffer> LineRenderer::_vertexBufferForCollisionEditorWindow = nullptr;

	Ref<Shader> LineRenderer::_shader = nullptr;

	const Color LineRenderer::LINE_COLOR = Color::GREEN;

	LineRenderer::LineRenderer()
	{
	}

	void LineRenderer::Initialize()
	{
		const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		_shader = shaderLibrary->Find(ShaderLibrary::LINE_SHADER_PATH).value();

		// Create vertex buffer.
		{
			DXVertexBuffer::VertexBufferInfo info = {};
			info.stride = sizeof(LineVertex);
			info.offset = 0u;
			info.usage = DXVertexBuffer::Usage::Dynamic;
			info.accessFlag = DXVertexBuffer::CpuAccessFlag::AccessWrite;
			info.mapType = DXVertexBuffer::MapType::WriteDiscard;
			info.vertexType = DXVertexBuffer::VertexBufferInfo::VertexType::LineVertex;
			info.count = MAX_LINE_COUNT * 2u;
			info.size = info.stride * info.count;
			info.data = std::make_shared<LineVertex[]>(info.count);

			_vertexBuffer = std::make_shared<DXVertexBuffer>();
			_vertexBuffer->Create(info);
		}

		// Create vertex buffer for collision editor window.
		{
			DXVertexBuffer::VertexBufferInfo info = {};
			info.stride = sizeof(LineVertex);
			info.offset = 0u;
			info.usage = DXVertexBuffer::Usage::Dynamic;
			info.accessFlag = DXVertexBuffer::CpuAccessFlag::AccessWrite;
			info.mapType = DXVertexBuffer::MapType::WriteDiscard;
			info.vertexType = DXVertexBuffer::VertexBufferInfo::VertexType::LineVertex;
			info.count = MAX_LINE_COUNT * 2u;
			info.size = info.stride * info.count;
			info.data = std::make_shared<LineVertex[]>(info.count);

			_vertexBufferForCollisionEditorWindow = std::make_shared<DXVertexBuffer>();
			_vertexBufferForCollisionEditorWindow->Create(info);
		}
	}

	void LineRenderer::Flush(bool collisionEditorWindow)
	{
		UpdateBuffer(collisionEditorWindow);
		Render(collisionEditorWindow);

		if (collisionEditorWindow)
		{
			_verticesForCollisionEditorWindow.clear();
		}
		else
		{
			_vertices.clear();
		}
	}

	void LineRenderer::AddPoint(const Vector3& position, bool collisionEditorWindow)
	{
		if (collisionEditorWindow)
		{
			if (_verticesForCollisionEditorWindow.size() >= MAX_LINE_COUNT * 2u)
				return;

			LineVertex vertex = { Vector4(position, 1.0f), LINE_COLOR };

			_verticesForCollisionEditorWindow.emplace_back(vertex);
		}
		else
		{
			if (_vertices.size() >= MAX_LINE_COUNT * 2u)
				return;

			LineVertex vertex = { Vector4(position, 1.0f), LINE_COLOR };

			_vertices.emplace_back(vertex);
		}
	}

	void LineRenderer::AddLine(const Vector3& from, const Vector3& to, bool collisionEditorWindow)
	{
		if (collisionEditorWindow)
		{
			if (_verticesForCollisionEditorWindow.size() + 2u >= MAX_LINE_COUNT * 2u)
				return;

			LineVertex fromVertex = { Vector4(from, 1.0f), LINE_COLOR };
			LineVertex toVertex = { Vector4(to, 1.0f), LINE_COLOR };

			_verticesForCollisionEditorWindow.emplace_back(fromVertex);
			_verticesForCollisionEditorWindow.emplace_back(toVertex);
		}
		else
		{
			if (_vertices.size() + 2u >= MAX_LINE_COUNT * 2u)
				return;

			LineVertex fromVertex = { Vector4(from, 1.0f), LINE_COLOR };
			LineVertex toVertex = { Vector4(to, 1.0f), LINE_COLOR };

			_vertices.emplace_back(fromVertex);
			_vertices.emplace_back(toVertex);
		}

		
	}

	void LineRenderer::UpdateBuffer(bool collisionEditorWindow)
	{
		if (collisionEditorWindow)
		{
			if (_verticesForCollisionEditorWindow.empty())
				return;

			void* data = _vertexBufferForCollisionEditorWindow->Map();
			{
				if (data)
				{
					const auto vertices = static_cast<LineVertex*>(data);
					memcpy_s(vertices, sizeof(LineVertex) * (MAX_LINE_COUNT * 2u), _verticesForCollisionEditorWindow.data(), sizeof(LineVertex) * _verticesForCollisionEditorWindow.size());
				}
			}
			_vertexBufferForCollisionEditorWindow->UnMap();
		}
		else
		{
			if (_vertices.empty())
				return;

			void* data = _vertexBuffer->Map();
			{
				if (data)
				{
					const auto vertices = static_cast<LineVertex*>(data);
					memcpy_s(vertices, sizeof(LineVertex) * (MAX_LINE_COUNT * 2u), _vertices.data(), sizeof(LineVertex) * _vertices.size());
				}
			}
			_vertexBuffer->UnMap();
		}
		
	}

	void LineRenderer::Render(bool collisionEditorWindow)
	{
		if (collisionEditorWindow)
		{
			if (_verticesForCollisionEditorWindow.empty())
				return;

			ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

			// Set rendering pipeline.
			{
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

				_vertexBufferForCollisionEditorWindow->Bind();

				// Bind Shaders.
				_shader->BindVertexShader(context);
				_shader->BindPixelShader(context);

				context->Draw(static_cast<UINT>(_verticesForCollisionEditorWindow.size()), 0);
			}

		}
		else
		{
			if (_vertices.empty())
				return;

			ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

			// Set rendering pipeline.
			{
				context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

				_vertexBuffer->Bind();

				// Bind Shaders.
				_shader->BindVertexShader(context);
				_shader->BindPixelShader(context);

				context->Draw(static_cast<UINT>(_vertices.size()), 0);
			}
		}
	}
}
