#include "stdafx.h"

#include "ConvexMeshShape.h"

#include "Physics/CollisionMesh.h"

#include "System/Physics/CollisionShapeGenerator.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"

namespace DentyEngine
{
	ConvexMeshShape::ConvexMeshShape() :
		IShape(), _collisionMesh(), _collisionMeshFilePath(),
		_shape(), _triangleMesh(), _size(), _created()
	{
	}

	ConvexMeshShape::ConvexMeshShape(const ConvexMeshShape& source) :
		_collisionMesh(source._collisionMesh),
		_collisionMeshFilePath(source._collisionMeshFilePath),
		_size(source._size), _created(source._created)
	{
		if (_collisionMesh)
		{
			Create(_collisionMesh);
		}
	}

	void ConvexMeshShape::Create(Ref<CollisionMesh> collisionMesh)
	{
		_collisionMesh = collisionMesh;
		_collisionMeshFilePath = _collisionMesh->GetFilePathToSerialized();

		{
			CollisionShapeGenerator::GenerateConvexCollisionShape(
				_collisionMesh, _triangleMesh, _shape
			);
		}

		_created = true;
	}

	void ConvexMeshShape::Update(const Vector3& size)
	{
		_size = size;

		if (_shape)
		{
			_shape->setLocalScaling(size.ToBtVector3());
		}
	}

	void ConvexMeshShape::Reset()
	{
		_shape.reset();
		_collisionMesh = nullptr;
		_collisionMeshFilePath = FilePath();
		_triangleMesh.reset();

		_created = false;
	}

	void ConvexMeshShape::OnGui()
	{
		ImGui::Separator();

		if (_collisionMesh)
		{
			ImGui::Text("CollisionMeshName: %s", _collisionMesh->GetName().c_str());
		}
		else
		{
			ImGui::Text("CollisionMeshName: None");
		}

		ImGui::PushStyleColor(ImGuiCol_Text, Color::RED.ToImVec4());
		ImGui::Text("Size: %f, %f, %f", _size.x, _size.y, _size.z);
		ImGui::PopStyleColor();

		ImGui::Separator();
	}

	void ConvexMeshShape::OnSerialized() const
	{
	}

	void ConvexMeshShape::OnDeserialized()
	{
		if (_collisionMeshFilePath.IsValid())
		{
			const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

			const std::optional<Ref<CollisionMesh>> collisionMesh = meshLibrary->Find<CollisionMesh>(_collisionMeshFilePath);
			if (!collisionMesh.has_value())
			{
				DENTY_ASSERT(false, "CollisionMesh not found! (in StaticMeshShape::OnDeserialized)");
			}

			_collisionMesh = collisionMesh.value();

			Create(_collisionMesh);
		}
	}

	void ConvexMeshShape::OnMeshAssetReceived(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::CollisionMesh, "Data type must be collision mesh!");

		const auto collisionMeshData = *static_cast<Utils::DragAndDropCollisionMeshData*>(data);
		const auto collisionMesh = collisionMeshData.Get();

		if (!collisionMesh)
		{
			DENTY_ASSERT(false, "Collision mesh doens't exists! (in ConvexMeshShape::OnMeshAssetReceived)");
		}

		_collisionMesh = collisionMesh;

		Create(_collisionMesh);
	}
}
