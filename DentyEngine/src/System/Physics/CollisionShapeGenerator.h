#pragma once

#include "Graphics/Vertex.h"

namespace DentyEngine
{
	class CollisionMesh;

	class CollisionShapeGenerator final
	{
	public:
		CollisionShapeGenerator();
		~CollisionShapeGenerator() = default;

		//
		// Use collision mesh.
		//
		[[nodiscard]]
		static bool GenerateBvhCollisionShape(
			const Ref<CollisionMesh>& collisionMesh, 
			Ref<btCompoundShape>& compoundShape, 
			Ref<btTriangleMesh>& triangleMesh, 
			Ref<btBvhTriangleMeshShape>& bvhTriangleMeshShape, bool subMesh = true, bool createCompoundShape = true
		);

		[[nodiscard]]
		static bool GenerateConvexCollisionShape(
			const Ref<CollisionMesh>& collisionMesh,
			Ref<btCompoundShape>& compoundShape,
			Ref<btTriangleMesh>& triangleMesh,
			Ref<btConvexHullShape>& convexHullShape, bool subMesh = true, bool createCompoundShape = true
		);

		//
		// Without compound shape.
		//

		// Use sub meshes at 0
		static void GenerateBvhCollisionShape(
			const std::vector<MeshResource::CollisionMeshVertex>& vertices,
			Ref<btTriangleMesh>& triangleMesh,
			Ref<btBvhTriangleMeshShape>& bvhTriangleMeshShape
		);

		// Use sub meshes at 0
		static void GenerateConvexCollisionShape(
			const std::vector<MeshResource::CollisionMeshVertex>& vertices,
			Ref<btTriangleMesh>& triangleMesh,
			Ref<btConvexHullShape>& convexHullShape
		);

		// Use sub meshes at 0
		static void GenerateBvhCollisionShape(
			const Ref<CollisionMesh>& collisionMesh,
			Ref<btTriangleMesh>& triangleMesh,
			Ref<btBvhTriangleMeshShape>& bvhTriangleMeshShape
		);

		// Use sub meshes at 0
		static void GenerateConvexCollisionShape(
			const Ref<CollisionMesh>& collisionMesh,
			Ref<btTriangleMesh>& triangleMesh,
			Ref<btConvexHullShape>& convexHullShape
		);

		//
		// Use vertices.
		//
		[[nodiscard]]
		static bool GenerateBvhCollisionShape(
			const std::vector<MeshResource::CollisionMeshVertex>& vertices,
			Ref<btCompoundShape>& compoundShape,
			Ref<btTriangleMesh>& triangleMesh,
			Ref<btBvhTriangleMeshShape>& bvhTriangleMeshShape
		);

		[[nodiscard]]
		static bool GenerateConvexCollisionShape(
			const std::vector<MeshResource::CollisionMeshVertex>& vertices,
			Ref<btCompoundShape>& compoundShape,
			Ref<btTriangleMesh>& triangleMesh,
			Ref<btConvexHullShape>& convexHullShape
		);
	private:
		//
		// Use collision mesh.
		//
		static void GenerateTriangleMesh(
			const Ref<CollisionMesh>& collisionMesh,
			Ref<btTriangleMesh>& triangleMesh
		);

		//
		// Use vertices.
		//
		static void GenerateTriangleMesh(
			const std::vector<MeshResource::CollisionMeshVertex>& vertices,
			Ref<btTriangleMesh>& triangleMesh
		);
	};
}
