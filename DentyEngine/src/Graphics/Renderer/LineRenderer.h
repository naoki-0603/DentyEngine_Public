#pragma once

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	class DXVertexBuffer;

	class Shader;

	class LineRenderer final
	{
	public:
		LineRenderer();
		~LineRenderer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(LineRenderer)

		//
		// Static functions.
		//
		static void Initialize();

		static void Flush(bool collisionEditorWindow = false);

		static void AddPoint(const Vector3& position, bool collisionEditorWindow = false);
		static void AddLine(const Vector3& from, const Vector3& to, bool collisionEditorWindow = false);
	private:
		static void UpdateBuffer(bool collisionEditorWindow);
		static void Render(bool collisionEditorWindow);

	private:
		static std::vector<LineVertex> _vertices;
		static Ref<DXVertexBuffer> _vertexBuffer;

		static std::vector<LineVertex> _verticesForCollisionEditorWindow;
		static Ref<DXVertexBuffer> _vertexBufferForCollisionEditorWindow;

		static Ref<Shader> _shader;
	private:
		static constexpr uint32_t MAX_LINE_COUNT = 32767u;

		static const Color LINE_COLOR;
	};
}