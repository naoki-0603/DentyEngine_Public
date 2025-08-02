#pragma once

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	class Shader;
	class DXVertexBuffer;

	struct GridRendererSpecification
	{
	public:
		GridRendererSpecification();
		~GridRendererSpecification() = default;

	public:
		std::vector<Vertex> vertices;

		Ref<DXVertexBuffer> vertexBuffer;
		Ref<Shader> shader;

		Vector2 cellSize;

		float gridLineThickness;
		float lineLength;

		int32_t rowCount;
		int32_t columnCount;
	};

	class GridRenderer final
	{
	public:
		GridRenderer();
		~GridRenderer() = default;

		static void Create(ID3D11Device* device, const Vector2& cellSize, float gridLineThickness, float lineLength);

		static void Render(ID3D11DeviceContext* context);

		static void Enable() { _enable = true; }
		static void Disable() { _enable = false; }

		[[nodiscard]]
		static bool IsEnabled() { return _enable; }
	private:
		static void CreateVerticesAndBuffer(ID3D11Device* device);

	private:
		static GridRendererSpecification _specification;
		static bool _enable;
	};
}
