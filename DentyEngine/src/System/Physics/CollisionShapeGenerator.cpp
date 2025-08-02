#include "stdafx.h"

#include <bullet/BulletCollision/CollisionShapes/btShapeHull.h>

#include "CollisionShapeGenerator.h"

#include "Physics/CollisionMesh.h"


namespace DentyEngine
{
	CollisionShapeGenerator::CollisionShapeGenerator()
	{
	}

	bool CollisionShapeGenerator::GenerateBvhCollisionShape(
		const Ref<CollisionMesh>& collisionMesh,
		Ref<btCompoundShape>& compoundShape, 
		Ref<btTriangleMesh>& triangleMesh,
		Ref<btBvhTriangleMeshShape>& bvhTriangleMeshShape, bool subMesh, bool createCompoundShape)
	{
		Ref<CollisionMesh> collision = nullptr;
		if (subMesh)
		{
			collision = std::static_pointer_cast<CollisionMesh>(
				collisionMesh->GetSubMeshes().at(0)
			);
		}
		else
		{
			collision = collisionMesh;
		}
		

		GenerateTriangleMesh(collision, triangleMesh);

		bvhTriangleMeshShape = std::make_shared<btBvhTriangleMeshShape>(triangleMesh.get(), true);

		if (createCompoundShape)
		{
			compoundShape = std::make_shared<btCompoundShape>();
			compoundShape->addChildShape(btTransform::getIdentity(), bvhTriangleMeshShape.get());
		}

		return true;
	}

	bool CollisionShapeGenerator::GenerateConvexCollisionShape(
		const Ref<CollisionMesh>& collisionMesh,
		Ref<btCompoundShape>& compoundShape,
		Ref<btTriangleMesh>& triangleMesh,
		Ref<btConvexHullShape>& convexHullShape, bool subMesh, bool createCompoundShape)
	{
		Ref<CollisionMesh> collision = nullptr;
		if (subMesh)
		{
			collision = std::static_pointer_cast<CollisionMesh>(
				collisionMesh->GetSubMeshes().at(0)
			);
		}
		else
		{
			collision = collisionMesh;
		}

		GenerateTriangleMesh(collision, triangleMesh);

		auto tempMeshShape = new btConvexTriangleMeshShape(triangleMesh.get());

		// Build hull.
		auto hull = new btShapeHull(tempMeshShape);
		hull->buildHull(tempMeshShape->getMargin());

		// Create hull shape.
		convexHullShape = std::make_shared<btConvexHullShape>();
		for (int index = 0; index < hull->numVertices(); ++index)
		{
			convexHullShape->addPoint(hull->getVertexPointer()[index]);
		}

		DENTY_SAFE_DELETE(tempMeshShape);
		DENTY_SAFE_DELETE(hull);

		if (createCompoundShape)
		{
			compoundShape = std::make_shared<btCompoundShape>();
			compoundShape->addChildShape(btTransform::getIdentity(), convexHullShape.get());
		}

		return true;
	}

	void CollisionShapeGenerator::GenerateBvhCollisionShape(
		const std::vector<MeshResource::CollisionMeshVertex>& vertices,
		Ref<btTriangleMesh>& triangleMesh,
		Ref<btBvhTriangleMeshShape>& bvhTriangleMeshShape
	)
	{
		GenerateTriangleMesh(vertices, triangleMesh);

		bvhTriangleMeshShape = std::make_shared<btBvhTriangleMeshShape>(triangleMesh.get(), true);
	}

	void CollisionShapeGenerator::GenerateConvexCollisionShape(
		const std::vector<MeshResource::CollisionMeshVertex>& vertices,
		Ref<btTriangleMesh>& triangleMesh,
		Ref<btConvexHullShape>& convexHullShape
	)
	{
		GenerateTriangleMesh(vertices, triangleMesh);

		auto tempMeshShape = new btConvexTriangleMeshShape(triangleMesh.get());

		// Build hull.
		auto hull = new btShapeHull(tempMeshShape);
		hull->buildHull(tempMeshShape->getMargin());

		// Create hull shape.
		convexHullShape = std::make_shared<btConvexHullShape>();
		for (int index = 0; index < hull->numVertices(); ++index)
		{
			convexHullShape->addPoint(hull->getVertexPointer()[index]);
		}

		DENTY_SAFE_DELETE(tempMeshShape);
		DENTY_SAFE_DELETE(hull);
	}

	void CollisionShapeGenerator::GenerateBvhCollisionShape(
		const Ref<CollisionMesh>& collisionMesh,
		Ref<btTriangleMesh>& triangleMesh, 
		Ref<btBvhTriangleMeshShape>& bvhTriangleMeshShape
	)
	{
		const auto subMesh = std::static_pointer_cast<CollisionMesh>(collisionMesh->GetSubMeshes().at(0));

		GenerateTriangleMesh(subMesh, triangleMesh);

		bvhTriangleMeshShape = std::make_shared<btBvhTriangleMeshShape>(triangleMesh.get(), true);
	}

	void CollisionShapeGenerator::GenerateConvexCollisionShape(
		const Ref<CollisionMesh>& collisionMesh,
		Ref<btTriangleMesh>& triangleMesh, 
		Ref<btConvexHullShape>& convexHullShape
	)
	{
		const auto subMesh = std::static_pointer_cast<CollisionMesh>(collisionMesh->GetSubMeshes().at(0));

		GenerateTriangleMesh(subMesh, triangleMesh);

		auto tempMeshShape = new btConvexTriangleMeshShape(triangleMesh.get());

		// Build hull.
		auto hull = new btShapeHull(tempMeshShape);
		hull->buildHull(tempMeshShape->getMargin());

		// Create hull shape.
		convexHullShape = std::make_shared<btConvexHullShape>();
		for (int index = 0; index < hull->numVertices(); ++index)
		{
			convexHullShape->addPoint(hull->getVertexPointer()[index]);
		}

		DENTY_SAFE_DELETE(tempMeshShape);
		DENTY_SAFE_DELETE(hull);
	}

	bool CollisionShapeGenerator::GenerateBvhCollisionShape(
		const std::vector<MeshResource::CollisionMeshVertex>& vertices,
		Ref<btCompoundShape>& compoundShape,
		Ref<btTriangleMesh>& triangleMesh,
		Ref<btBvhTriangleMeshShape>& bvhTriangleMeshShape
	)
	{
		GenerateTriangleMesh(vertices, triangleMesh);

		bvhTriangleMeshShape = std::make_shared<btBvhTriangleMeshShape>(triangleMesh.get(), true);

		compoundShape = std::make_shared<btCompoundShape>();
		compoundShape->addChildShape(btTransform::getIdentity(), bvhTriangleMeshShape.get());

		return true;
	}

	bool CollisionShapeGenerator::GenerateConvexCollisionShape(
		const std::vector<MeshResource::CollisionMeshVertex>& vertices,
		Ref<btCompoundShape>& compoundShape,
		Ref<btTriangleMesh>& triangleMesh, 
		Ref<btConvexHullShape>& convexHullShape
	)
	{
		GenerateTriangleMesh(vertices, triangleMesh);

		auto tempMeshShape = new btConvexTriangleMeshShape(triangleMesh.get());

		// Build hull.
		auto hull = new btShapeHull(tempMeshShape);
		hull->buildHull(tempMeshShape->getMargin());

		// Create hull shape.
		convexHullShape = std::make_shared<btConvexHullShape>();
		for (int index = 0; index < hull->numVertices(); ++index)
		{
			convexHullShape->addPoint(hull->getVertexPointer()[index]);
		}

		DENTY_SAFE_DELETE(tempMeshShape);
		DENTY_SAFE_DELETE(hull);

		compoundShape = std::make_shared<btCompoundShape>();
		compoundShape->addChildShape(btTransform::getIdentity(), convexHullShape.get());

		return true;
	}

	void CollisionShapeGenerator::GenerateTriangleMesh(
		const Ref<CollisionMesh>& collisionMesh,
		Ref<btTriangleMesh>& triangleMesh)
	{
		triangleMesh = std::make_shared<btTriangleMesh>();

		for (uint32_t vertexIndex = 0u; vertexIndex < collisionMesh->GetVertices().size(); vertexIndex += 3u)
		{
			const auto& vertex0 = collisionMesh->GetVertices().at(vertexIndex).position.GetAsVector3();
			const auto& vertex1 = collisionMesh->GetVertices().at(vertexIndex + 1u).position.GetAsVector3();
			const auto& vertex2 = collisionMesh->GetVertices().at(vertexIndex + 2u).position.GetAsVector3();

			triangleMesh->addTriangle(vertex0.ToBtVector3(), vertex1.ToBtVector3(), vertex2.ToBtVector3(), true);
		}
	}

	void CollisionShapeGenerator::GenerateTriangleMesh(
		const std::vector<MeshResource::CollisionMeshVertex>& vertices,
		Ref<btTriangleMesh>& triangleMesh
	)
	{
		triangleMesh = std::make_shared<btTriangleMesh>();

		for (uint32_t vertexIndex = 0u; vertexIndex < vertices.size(); vertexIndex += 3u)
		{
			const auto& vertex0 = vertices.at(vertexIndex).position.GetAsVector3();
			const auto& vertex1 = vertices.at(vertexIndex + 1u).position.GetAsVector3();
			const auto& vertex2 = vertices.at(vertexIndex + 2u).position.GetAsVector3();

			triangleMesh->addTriangle(vertex0.ToBtVector3(), vertex1.ToBtVector3(), vertex2.ToBtVector3(), true);
		}
	}
}
