#include "stdafx.h"

#include "CollisionPreviewObject.h"

#include "Physics/CollisionMesh.h"

#include "Graphics/Renderer/LineRenderer.h"

#include "System/Physics/CollisionShapeGenerator.h"

namespace DentyEngine
{
	CollisionPreviewObject::CollisionPreviewObject() :
		Object(), _collisionMesh(),  _collisionMeshIndex(),
		_ghostObject(), _collisionShapes(), _positionInWorld(), _prevShapeType(ShapeType::None), _shapeType(ShapeType::None), _bulletObjectType(ApproximationCollisionMeshBulletObjectType::RigidBody)
	{
		_name = "CollisionPreviewObject";
	}

	void CollisionPreviewObject::Create(Ref<CollisionMesh> collisionMesh, std::string_view name, uint32_t index, uint32_t maxVerticesCount)
	{
		{
			_collisionMesh = collisionMesh;
			_collisionMeshIndex = index;

			_ghostObject = std::make_shared<btGhostObject>();

			_name = name;
		}

		{
			// Create mesh collider.
			if (GetVerticesCount() <= maxVerticesCount)
			{
				const bool succeed = CollisionShapeGenerator::GenerateBvhCollisionShape(
					_collisionMesh, _collisionShapes.compoundShape, _collisionShapes.triangleMesh,
					_collisionShapes.bvhTriangleMeshShape, false
				);

				if (!succeed)
					DENTY_ASSERT(false, "Failed to generate bvh collision shape! (in CollsionPreviewObject::Create)");

				_prevShapeType = ShapeType::Mesh;
				_shapeType = ShapeType::Mesh;
			}
		}

		// Compute position in world.
		{
			const Vector3 centroid = _collisionMesh->GetCentroid();
			const Vector3 halfSize = _collisionMesh->GetBoundingBox().GetHalfSize();

			_positionInWorld = centroid - (halfSize);
		}	
	}

	void CollisionPreviewObject::Update()
	{
		switch (_shapeType)
		{
		case ShapeType::Mesh:
			{
			for (uint32_t index = 0u; index < _collisionMesh->GetVertices().size(); ++index)
			{
				const MeshResource::CollisionMeshVertex& vertex = _collisionMesh->GetVertices().at(index);

				LineRenderer::AddPoint(vertex.position.GetAsVector3(), true);
			}
			}
			break;
		case ShapeType::ConvexMesh:
			{
				const int32_t numVertices = _collisionShapes.convexHullShape->getNumVertices();

				for (int32_t index = 0; index < numVertices; ++index)
				{
					btVector3 position;
					_collisionShapes.convexHullShape->getVertex(index, position);

					LineRenderer::AddPoint(Vector3::ToVector3(position), true);
				}
			}
			break;
		}
	}

	uint32_t CollisionPreviewObject::OnGui()
	{
		const String label = String(std::to_string(_collisionMeshIndex)) + "_" + _name + "##CollisionPreviewObject";

		if (ImGui::Selectable(label.CStr()))
		{
			return _collisionMeshIndex;
		}

		return UINT32_MAX;
	}

	void CollisionPreviewObject::OnInspectorGui()
	{
		ImGui::Text("Name: %s", _name.CStr());
		ImGui::Text("ShapeType: %s", String(NAMEOF_ENUM(_shapeType)).CStr());

		if (ImGui::CollapsingHeader("CollisionShapes##CollisionPreviewObject", ImGuiTreeNodeFlags_DefaultOpen))
		{
			_prevShapeType = _shapeType;

			ImGui::Indent();

			/*if (ImGui::Selectable("Box##CollisionShape"))
			{
				_colliderType = ColliderType::Box;
			}

			if (ImGui::Selectable("Sphere##CollisionShape"))
			{
				_colliderType = ColliderType::Sphere;
			}

			if (ImGui::Selectable("Capsule##CollisionShape"))
			{
				_colliderType = ColliderType::Capsule;
			}*/

			if (ImGui::Selectable("Mesh##CollisionShape"))
			{
				_shapeType = ShapeType::Mesh;
			}

			if (ImGui::Selectable("ConvexMesh##CollisionShape"))
			{
				_shapeType = ShapeType::ConvexMesh;
			}

			// Changed shape type.
			if (_prevShapeType != _shapeType)
			{
				_collisionShapes.Reset();

				switch (_shapeType)
				{
				case ShapeType::Mesh:
					{
						const bool succeed = CollisionShapeGenerator::GenerateBvhCollisionShape(
							_collisionMesh, _collisionShapes.compoundShape, _collisionShapes.triangleMesh,
							_collisionShapes.bvhTriangleMeshShape, false
						);

						if (!succeed)
							DENTY_ASSERT(false, "Failed to generate bvh collision shape! (in CollsionPreviewObject::OnInspectorGui)");
					}
					
					break;
				case ShapeType::ConvexMesh:
					{
						const bool succeed = CollisionShapeGenerator::GenerateConvexCollisionShape(
							_collisionMesh, _collisionShapes.compoundShape, _collisionShapes.triangleMesh,
							_collisionShapes.convexHullShape, false
						);

						if (!succeed)
							DENTY_ASSERT(false, "Failed to generate convex collision shape! (in CollsionPreviewObject::OnInspectorGui)");
					}
					break;
				}
			}

			ImGui::Unindent();
		}

		ImGui::Separator();
	}

	uint32_t CollisionPreviewObject::GetVerticesCount() const
	{
		if (!_collisionMesh)
			DENTY_ASSERT(false, "Collision mesh doesn't exists! (in CollisionPreviewObject::GetVerticesCount)");

		return static_cast<uint32_t>(_collisionMesh->GetVertices().size());
	}
}
