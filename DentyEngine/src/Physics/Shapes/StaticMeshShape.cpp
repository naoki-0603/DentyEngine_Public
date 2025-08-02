#include "stdafx.h"

#include "StaticMeshShape.h"

#include "Physics/CollisionMesh.h"

#include "System/Physics/CollisionShapeGenerator.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"

namespace DentyEngine
{
	StaticMeshShape::StaticMeshShape() :
		IShape(), _collisionMesh(), _collisionMeshFilePath(),
		_shape(), _triangleMesh(), _size(), _hasCreated()
	{
	}

	StaticMeshShape::StaticMeshShape(const StaticMeshShape& source) :
		_collisionMesh(source._collisionMesh),
		_collisionMeshFilePath(source._collisionMeshFilePath),
		_size(source._size), _hasCreated(source._hasCreated)
	{
		if (_collisionMesh)
		{
			Create(_collisionMesh);
		}
	}

	void StaticMeshShape::Create(Ref<CollisionMesh> collisionMesh)
	{
		_collisionMesh = collisionMesh;
		_collisionMeshFilePath = collisionMesh->GetFilePathToSerialized();

		// Create shape.
		{
			CollisionShapeGenerator::GenerateBvhCollisionShape(
				_collisionMesh, _triangleMesh, _shape
			);
		}

		_hasCreated = true;
	}

	void StaticMeshShape::Reset()
	{
		_collisionMesh = nullptr;
		_collisionMeshFilePath = FilePath();
		_shape.reset();
		_triangleMesh.reset();

		_hasCreated = false;
	}

	void StaticMeshShape::Update(const Vector3& size)
	{
		_size = size;

		if (_shape)
		{
			_shape->setLocalScaling(size.ToBtVector3());
		}
	}

	void StaticMeshShape::OnGui()
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

	void StaticMeshShape::OnSerialized() const
	{
	}

	void StaticMeshShape::OnDeserialized()
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

	void StaticMeshShape::OnMeshAssetReceived(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::CollisionMesh, "Data type must be collision mesh!");

		const auto collisionMeshData = *static_cast<Utils::DragAndDropCollisionMeshData*>(data);
		const auto collisionMesh = collisionMeshData.Get();

		if (!collisionMesh)
		{
			DENTY_ASSERT(false, "Collision mesh doens't exists! (in StaticMeshShape::OnMeshAssetReceived)");
		}

		Create(collisionMesh);
	}
}
