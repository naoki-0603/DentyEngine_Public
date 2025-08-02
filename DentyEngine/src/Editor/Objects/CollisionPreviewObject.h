#pragma once

#include "System/Core/Object.h"

#include "System/Physics/PhysicsData.h"

namespace DentyEngine
{
	class CollisionMesh;

	class CollisionPreviewObject final : public Object
	{
	public:
		CollisionPreviewObject();
		~CollisionPreviewObject() override = default;

		void Create(Ref<CollisionMesh> collisionMesh, std::string_view name, uint32_t index, uint32_t maxVerticesCount);

		void Update();

		// Return selected index.
		[[nodiscard]]
		uint32_t OnGui();

		void OnInspectorGui();

		//
		// Getter
		//
		[[nodiscard]]
		const Vector3& GetPositionInWorld() const { return _positionInWorld; }

		[[nodiscard]]
		uint32_t GetVerticesCount() const;

		[[nodiscard]]
		uint32_t GetIndex() const { return _collisionMeshIndex; }

		[[nodiscard]]
		ShapeType GetShapeType() const { return _shapeType; }

		[[nodiscard]]
		ApproximationCollisionMeshBulletObjectType GetBulletObjectType() const { return _bulletObjectType; }

		[[nodiscard]]
		bool HasCollider() const { return (_shapeType != ShapeType::None); }
	private:
		Ref<CollisionMesh> _collisionMesh;

		uint32_t _collisionMeshIndex;

		// Bullet physics.
		Ref<btGhostObject> _ghostObject;
		MeshCollisionShapes _collisionShapes;

		Vector3 _positionInWorld;

		ShapeType _prevShapeType;
		ShapeType _shapeType;

		ApproximationCollisionMeshBulletObjectType _bulletObjectType;
	};
}