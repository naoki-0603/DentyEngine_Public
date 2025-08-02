#pragma once

#include "System/Core/ID/EntityID.h"

namespace DentyEngine
{
	enum class CollisionType : int32_t
	{
		None = -1,
		Collision,
		Trigger,

		Max
	};

	enum class ShapeType : int32_t
	{
		None = -1,
		Box = 0,
		Sphere,
		Capsule,
		Mesh,
		ConvexMesh,

		Max
	};

	// This collision state
	enum class CollisionState : int32_t
	{
		None = -1,
		Enter,
		Stay,
		Exit,

		Max
	};

	enum class ApproximationCollisionMeshBulletObjectType : int32_t
	{
		None = -1,
		GhostObject = 0,
		RigidBody,

		Max
	};

	struct ApproximationMeshCollisionShapes
	{
		ApproximationMeshCollisionShapes() :
			convexHullShape(), triangleMesh(), bvhTriangleMeshShape() {}

		// For convex shape.
		Ref<btConvexHullShape> convexHullShape;

		// For bvh triangle mesh shape.
		Ref<btTriangleMesh> triangleMesh;
		Ref<btBvhTriangleMeshShape> bvhTriangleMeshShape;
	};

	struct MeshCollisionShapes
	{
		MeshCollisionShapes() : convexHullShape(), triangleMesh(), bvhTriangleMeshShape() {}
		MeshCollisionShapes(const MeshCollisionShapes&) = delete;
		MeshCollisionShapes& operator=(const MeshCollisionShapes&) noexcept = delete;

		void Reset()
		{
			compoundShape.reset();
			convexHullShape.reset();
			bvhTriangleMeshShape.reset();
			triangleMesh.reset();
		}
	public:
		Ref<btCompoundShape> compoundShape;

		// For convex shape.
		Ref<btConvexHullShape> convexHullShape;

		// For bvh triangle mesh shape.
		Ref<btTriangleMesh> triangleMesh;
		Ref<btBvhTriangleMeshShape> bvhTriangleMeshShape;
	};


    // This struct used to identify.

    //
	// Unused struct. (2024/01/09)
	//
	struct CollisionData
	{
		CollisionData() : currentCollisionDataUMap(), objectIDs(), collisionState(), collisionType(), currentFrameIsReceived() {}
		CollisionData(EntityID ownerID) :
			currentCollisionDataUMap(), objectIDs(),
		ownerID(ownerID), collisionState(CollisionState::None), collisionType(CollisionType::None), currentFrameIsReceived() {}

		void Update();
		void AddCollisionData(CollisionData& collisionData);
	public:
		// Doesn't serialize
		std::unordered_map<EntityID, CollisionData> currentCollisionDataUMap;

		// Store IDs of object counties that have already called OnXXXStay
		// Do not make multiple hits on the same object in one frame.
		std::vector<EntityID> objectIDs;

		EntityID ownerID;

		// Doesn't serialize
		CollisionState collisionState;

		CollisionType collisionType;

		bool currentFrameIsReceived;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive) const
		{
			archive(
				CEREAL_NVP(ownerID),
				CEREAL_NVP(collisionType)
			);
		}

		template <class Archive>
		void load(Archive& archive)
		{
			archive(
				CEREAL_NVP(ownerID),
				CEREAL_NVP(collisionType)
			);
		}
	};
}