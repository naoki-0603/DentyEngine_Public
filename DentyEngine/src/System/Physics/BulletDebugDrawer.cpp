#include "stdafx.h"

#include "BulletDebugDrawer.h"

#include "Graphics/Renderer/Shader.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"

#include "Graphics/States/BlendState.h"
#include "Graphics/States/DepthStencilState.h"
#include "Graphics/States/RasterizerState.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

#include "Utilities/MathUtilities.h"

namespace DentyEngine
{
	BulletDebugDrawerLineData::BulletDebugDrawerLineData() :
		_vertexBuffer(), _shader(), _vertices(), _constants()
	{
	}

	void BulletDebugDrawerLineData::Initialize()
	{
		// Create vertex buffer.
		{
			DXVertexBuffer::VertexBufferInfo info = {};
			info.stride = sizeof(LineVertex);
			info.offset = 0u;
			info.usage = DXVertexBuffer::Usage::Dynamic;
			info.accessFlag = DXVertexBuffer::CpuAccessFlag::AccessWrite;
			info.mapType = DXVertexBuffer::MapType::WriteDiscard;
			info.vertexType = DXVertexBuffer::VertexBufferInfo::VertexType::LineVertex;
			info.count = BulletDebugDrawer::MAX_LINES;
			info.size = info.stride * info.count;
			info.data = std::make_shared<LineVertex[]>(info.count);

			_vertexBuffer = std::make_shared<DXVertexBuffer>();
			_vertexBuffer->Create(info);
		}

		// Create shader.
		{
			const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

			const std::optional<Ref<Shader>> shader = shaderLibrary->Find(ShaderLibrary::DEBUG_DRAWER_PATH);
			if (!shader.has_value())
			{
				DENTY_ASSERT(false, "Shader not found! (in BulletDebugDrawerLineData::Initialize)");
			}

			_shader = shader.value();
		}
	}

	void BulletDebugDrawerLineData::Finalize()
	{
		Clear();
	}

	void BulletDebugDrawerLineData::Clear()
	{
		_vertices.clear();
	}

	void BulletDebugDrawerLineData::Update(ID3D11DeviceContext* context, const Matrix& viewProjection)
	{
		_constants.viewProjection = viewProjection;

		UpdateVertexData(context);
	}

	void BulletDebugDrawerLineData::Render()
	{
		if (_vertices.empty())
			return;

		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		// Cached blend state variables.
		ComRef<ID3D11BlendState> cachedBlendState;
		float* cachedBlendFactor = nullptr;
		UINT cachedSampleMask = 0u;

		// Cached depth stencil state variabls.
		ComRef<ID3D11DepthStencilState> cachedDepthStencilState;
		UINT cachedStencilRef = 0u;

		// Cached rasterizer state.
		ComRef<ID3D11RasterizerState> cachedRasterizerState;

		// Get cached.
		{
			context->OMGetBlendState(cachedBlendState.ReleaseAndGetAddressOf(), cachedBlendFactor, &cachedSampleMask);
			context->OMGetDepthStencilState(cachedDepthStencilState.ReleaseAndGetAddressOf(), &cachedStencilRef);
			context->RSGetState(cachedRasterizerState.ReleaseAndGetAddressOf());
		}

		// Set pipeline states.
		{
			const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			_vertexBuffer->Bind();

			_shader->BindVertexShader(context);
			_shader->BindPixelShader(context);

			_shader->constantBuffer->UpdateSubresource(context, &_constants);
			_shader->constantBuffer->BindAll(context, 0);

			ID3D11BlendState* blendState = BlendState::GetBuildInBlendStatesAt(BlendState::Type::Alpha);
			context->OMSetBlendState(blendState, blendFactor, 0xFFFFFFFF);

			ID3D11DepthStencilState* depthStencilState = DepthStencilState::GetBuildInDepthStencilStatesAt(DepthStencilState::Type::DepthOnStencilOn);
			context->OMSetDepthStencilState(depthStencilState, 0);

			ID3D11RasterizerState* rasterizerState = RasterizerState::GetBuildInRasterizerStatesAt(RasterizerState::Type::FillNoCull);
			context->RSSetState(rasterizerState);

			context->Draw(static_cast<UINT>(_vertices.size()), 0);
		}

		{
			context->OMSetBlendState(cachedBlendState.Get(), cachedBlendFactor, cachedSampleMask);
			context->OMSetDepthStencilState(cachedDepthStencilState.Get(), cachedStencilRef);
			context->RSSetState(cachedRasterizerState.Get());
		}
	}

	void BulletDebugDrawerLineData::AddVertex(const LineVertex& vertex)
	{
		_vertices.emplace_back(vertex);
	}

	void BulletDebugDrawerLineData::UpdateVertexData([[maybe_unused]] ID3D11DeviceContext* context)
	{
		if (_vertices.empty()) 
			return;

		void* data = _vertexBuffer->Map();
		{
			if (data)
			{
				const auto vertices = static_cast<LineVertex*>(data);
				memcpy_s(vertices, sizeof(LineVertex) * BulletDebugDrawer::MAX_LINES, _vertices.data(), sizeof(LineVertex) * _vertices.size());
			}
		}
		_vertexBuffer->UnMap();
	}

	const Color BulletDebugDrawer::DEFAULT_LINE_COLOR = Color::GREEN;

	BulletDebugDrawer::BulletDebugDrawer() :
		btIDebugDraw(), _lines(), _debugMode()
	{
	}

	void BulletDebugDrawer::Initialize()
	{
		_lines.Initialize();
	}

	void BulletDebugDrawer::Finalize()
	{
		_lines.Finalize();
	}

	void BulletDebugDrawer::Update(ID3D11DeviceContext* context, const Matrix& viewProjection)
	{
		_lines.Update(context, viewProjection);
	}

	void BulletDebugDrawer::ClearLines()
	{
		_lines.Clear();
	}

	void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		if (_lines.GetVerticesCount() + 2 >= MAX_LINES) 
			return;

		Color desiredColor = { color.getX(), color.getY(), color.getZ(), 1.0f };
		const Vector3 colorVec3 = desiredColor.color.GetAsVector3();
		if (MathUtils::ApproxEqual(colorVec3.x, 1.0f) && MathUtils::ApproxEqual(colorVec3.y, 1.0f) && MathUtils::ApproxEqual(colorVec3.z, 1.0f))
			desiredColor = DEFAULT_LINE_COLOR;

		const LineVertex fromVertex = { Vector4(from.x(), from.y(), from.z(), 1.0f), desiredColor };
		const LineVertex toVertex = { Vector4(to.x(), to.y(), to.z(), 1.0f), desiredColor };

		_lines.AddVertex(fromVertex);
		_lines.AddVertex(toVertex);
	}

	void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, [[maybe_unused]] const btVector3& fromColor,
		[[maybe_unused]] const btVector3& toColor)
	{
		if (_lines.GetVerticesCount() + 2 >= MAX_LINES)
			return;

		const LineVertex fromVertex = { Vector4(from.x(), from.y(), from.z(), 1.0f), DEFAULT_LINE_COLOR };
		const LineVertex toVertex = { Vector4(to.x(), to.y(), to.z(), 1.0f), DEFAULT_LINE_COLOR };

		_lines.AddVertex(fromVertex);
		_lines.AddVertex(toVertex);
	}

	void BulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
		[[maybe_unused]] int lifeTime, const btVector3& color)
	{
		drawLine(PointOnB, PointOnB + normalOnB * distance, color);
		btVector3 normalColor(0, 0, 0);
		drawLine(PointOnB, PointOnB + normalOnB * 0.01f, normalColor);
	}

	void BulletDebugDrawer::clearLines()
	{
		_lines.Clear();
	}

	void BulletDebugDrawer::flushLines()
	{
		_lines.Render();
	}

	void BulletDebugDrawer::setDebugMode(int debugMode)
	{
		_debugMode = static_cast<DebugDrawModes>(debugMode);
	}

	int BulletDebugDrawer::getDebugMode() const
	{
		return static_cast<int32_t>(_debugMode);
	}
}
