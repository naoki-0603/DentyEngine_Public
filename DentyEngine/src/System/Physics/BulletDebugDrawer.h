#pragma once

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	class DXVertexBuffer;
	class Shader;

	struct BulletDebugDrawerLineDataConstants
	{
		BulletDebugDrawerLineDataConstants() : viewProjection() {}

		Matrix viewProjection;
	};

	struct BulletDebugDrawerLineData
	{
	public:
		BulletDebugDrawerLineData();
		~BulletDebugDrawerLineData() = default;

		void Initialize();
		void Finalize();
		void Clear();
		void Update(ID3D11DeviceContext* context, const Matrix& viewProjection);
		void Render();

		void AddVertex(const LineVertex& vertex);

		// 
		// Getter
		// 
		[[nodiscard]]
		size_t GetVerticesCount() const { return _vertices.size(); }
	private:
		void UpdateVertexData(ID3D11DeviceContext* context);

	private:
		Ref<DXVertexBuffer> _vertexBuffer;

		Ref<Shader> _shader;

		std::vector<LineVertex> _vertices;

		BulletDebugDrawerLineDataConstants _constants;
	};

	class BulletDebugDrawer final : public btIDebugDraw
	{
	public:
		BulletDebugDrawer();
		~BulletDebugDrawer() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(BulletDebugDrawer)

		void Initialize();
		void Finalize();
		void Update(ID3D11DeviceContext* context, const Matrix& viewProjection);

		void ClearLines();

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override;

		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;

		// ç°ÇÃÇ∆Ç±ÇÎÇÕãÛé¿ëïÇ…ÇµÇƒÇ®Ç≠ÅB
		void reportErrorWarning([[maybe_unused]] const char* warningString) override {}
		void draw3dText([[maybe_unused]] const btVector3& location, [[maybe_unused]] const char* textString) override {}

		void clearLines() override;
		void flushLines() override;

		//
		// Setter
		//
		void setDebugMode(int debugMode) override;

		//
		// Getter
		//

		[[nodiscard]]
		int32_t getDebugMode() const override;
	public:
		static const Color DEFAULT_LINE_COLOR;
		static constexpr size_t MAX_LINES = USHORT_MAX;
	private:
		BulletDebugDrawerLineData _lines;
		DebugDrawModes _debugMode;
	};
}