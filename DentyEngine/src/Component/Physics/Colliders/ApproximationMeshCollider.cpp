#include "stdafx.h"

#include "ApproximationMeshCollider.h"

#include "System/Core/Tag.h"

#include "Physics/ApproximationCollisionMesh.h"

#include "Scene/GameObject.h"
#include "System/Core/Assertion.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"
#include "System/Events/EventScene.h"

#include "System/Physics/PhysicsEngine.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"

namespace DentyEngine
{
	ApproximationMeshCollider::ApproximationMeshCollider() :
		Collider(), _approximationCollisionMesh(), _approximationCollisionMeshFilePath(),
        _offset(), _rotation(), _size(1, 1, 1)
	{
		_type = Type::ApproximationMesh;
		_name = "ApproximationMeshCollider";

		_attachToChildAllowed = false;
	}

	ApproximationMeshCollider::ApproximationMeshCollider(const ApproximationMeshCollider& source) :
		Collider(source), _approximationCollisionMesh(source._approximationCollisionMesh),
		_approximationCollisionMeshFilePath(source._approximationCollisionMeshFilePath),
		_offset(source._offset), _rotation(source._rotation), _size(source._size)
	{
	}

	void ApproximationMeshCollider::Awake()
	{
		Collider::Awake();
	}

	void ApproximationMeshCollider::Start()
	{
		Collider::Start();
	}

	void ApproximationMeshCollider::OnUpdate(float deltaTime)
	{
		Collider::OnUpdate(deltaTime);
	}

	void ApproximationMeshCollider::OnFixedUpdate()
	{
		Collider::OnFixedUpdate();
	}

	void ApproximationMeshCollider::OnGui()
	{
		Collider::OnGui();

		// Transform
		{
            bool isChanged = false;
            if (ImGui::SliderAngle("RotationX##ApproximationMeshCollider", &_rotation.x))
            {
                isChanged = true;
            }

            if (ImGui::SliderAngle("RotationY##ApproximationMeshCollider", &_rotation.y))
            {
                isChanged = true;
            }

            if (ImGui::SliderAngle("RotationZ##ApproximationMeshCollider", &_rotation.z))
            {
                isChanged = true;
            }

            if (ImGui::DragFloat3("Offset##ApproximationMeshCollider", &_offset.x, 0.1f, 50.0f))
            {
                isChanged = true;
            }

            if (ImGui::DragFloat3("Scale##ApproximationMeshCollider", &_size.x, 0.1f, 50.0f))
            {
                isChanged = true;
            }

            if (isChanged)
            {
                UpdateBulletWorldTransform();
            }
		}

		// Apply or remove trigger to/from children.
		{
			ImGui::TextWrapped("IsTrigger");
			ImGui::SameLine();

		    if (ImGui::Checkbox("##IsTriggerApproximationMeshCollider", &_isTrigger))
		    {
		        if (_isTrigger)
		        {
		            ApplyTriggerCollisionStateToChildren();
		        }
				else
				{
				    RemoveTriggerCollisionStateFromChildren();
				}
		    }
		}

		// Drag and drop.
		{
			if (ImGui::CollapsingHeader("ApproximationCollisionMesh##ApproximationMeshCollider", ImGuiTreeNodeFlags_DefaultOpen))
			{
				const auto regionSize = ImGui::GetContentRegionAvail();

				if (_approximationCollisionMesh)
				{
					_approximationCollisionMesh->OnGui();
				}
				else
				{
					ImGui::Text("DragAndDropHere!");
					ImGui::InvisibleButton("DragAndDropHere##ApproximationMeshCollider", ImVec2(regionSize.x, 32.0f));
				}

				// Target
				{
					const DragAndDropSpecification specification =
					{
						"ApproximationCollisionMeshItem", sizeof(Utils::DragAndDropApproximationCollisionMeshData),
						nullptr, DragAndDropDataType::ApproximationCollisionMesh,
						DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(ApproximationCollisionMeshAssetDragAndDropTargetCallback),
						DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
					};
					DragAndDropTarget dragAndDropTarget(specification);
				}
			}
		}
	}

	void ApproximationMeshCollider::OnPrefabGui()
	{
		Collider::OnPrefabGui();

		ImGui::InputFloat3("Scale##ApproximationMeshCollider", &_size.x, "%.3f");
	}

	void ApproximationMeshCollider::OnEvent(Event* e)
	{
		Collider::OnEvent(e);
	}

	void ApproximationMeshCollider::OnSerialized() const
	{
		Collider::OnSerialized();
	}

	void ApproximationMeshCollider::OnDeserialized()
	{
		Collider::OnDeserialized();

		if (_approximationCollisionMeshFilePath.IsValid())
		{
			const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();

			const std::optional<Ref<ApproximationCollisionMesh>> collisionMesh = meshLibrary->Find<ApproximationCollisionMesh>(_approximationCollisionMeshFilePath);
			if (!collisionMesh.has_value())
			{
				DENTY_ASSERT(false, "Approximation collision mesh not found! (in ApproximationMeshCollider::OnDeserialized)");
			}

			_approximationCollisionMesh = collisionMesh.value();
		}
	}

	void ApproximationMeshCollider::OnGameObjectDeserialized()
	{
		Collider::OnGameObjectDeserialized();
	}

	void ApproximationMeshCollider::UpdateState(const Vector3& position, const Quaternion& orientation)
	{
		Collider::UpdateState(position, orientation);
	}

	void ApproximationMeshCollider::ApplyScaling()
	{
	}

	void ApproximationMeshCollider::AddCollisionObjects(btDiscreteDynamicsWorld* world)
	{
		if (not _approximationCollisionMesh)
			return;

		for (auto&& subMesh : _approximationCollisionMesh->GetSubMeshes())
		{
			auto&& collisionMesh = std::static_pointer_cast<ApproximationCollisionMesh>(subMesh);

			btCollisionObject* collisionObject = collisionMesh->GetBulletObject();
			const Ref<btCollisionShape> collisionShape = collisionMesh->GetCollisionShape();

			collisionObject->setCollisionShape(collisionShape.get());

			//world->addCollisionObject(ghostObject.get(), group, mask);
			world->addCollisionObject(collisionObject);
		}
	}

	void ApproximationMeshCollider::RemoveCollisionObjects(btDiscreteDynamicsWorld* world)
	{
		if (not _approximationCollisionMesh)
			return;

		for (auto&& subMesh : _approximationCollisionMesh->GetSubMeshes())
		{
			auto&& collisionMesh = std::static_pointer_cast<ApproximationCollisionMesh>(subMesh);

			btCollisionObject* collisionObject = collisionMesh->GetBulletObject();

			world->removeCollisionObject(collisionObject);
		}
	}

	Ref<Object> ApproximationMeshCollider::MakeCopy() const
	{
		return std::make_shared<ApproximationMeshCollider>(*std::static_pointer_cast<const ApproximationMeshCollider>(shared_from_this()));
	}

	void ApproximationMeshCollider::OnSceneActivated()
	{
		if (HasApproximationMesh())
		{
			SetChildrenUserIndex();

			UpdateBulletWorldTransform();

			if (_isTrigger)
			{
			    ApplyTriggerCollisionStateToChildren();
			}

			PhysicsEngine::GetInstance()->AddCollisionObject(shared_from_this());
		}
	}

	void ApproximationMeshCollider::OnSceneDeActivated()
	{
		PhysicsEngine::GetInstance()->RemoveCollisionObject(shared_from_this());
	}

	void ApproximationMeshCollider::OnGameObjectTagChanged()
	{
		SetChildrenUserIndex();
	}

	void ApproximationMeshCollider::SetChildrenUserIndex()
	{
        const std::optional<uint32_t> tagNumber = Tag::GetTagNumberFrom(GetParent()->tag());

		for (auto&& subMesh : _approximationCollisionMesh->GetSubMeshes())
		{
			std::static_pointer_cast<ApproximationCollisionMesh>(subMesh)->GetBulletObject()->setUserIndex((tagNumber.has_value() ? static_cast<int32_t>(tagNumber.value()) : -1));
		}
	}

    void ApproximationMeshCollider::ApplyTriggerCollisionStateToChildren()
    {
		for (auto&& subMesh : _approximationCollisionMesh->GetSubMeshes())
		{
		    auto&& approximationCollisionMesh = std::static_pointer_cast<ApproximationCollisionMesh>(subMesh);

		    const int32_t collisionFlags = approximationCollisionMesh->GetBulletObject()->getCollisionFlags();
			approximationCollisionMesh->GetBulletObject()->setCollisionFlags(collisionFlags | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
		}
    }

    void ApproximationMeshCollider::RemoveTriggerCollisionStateFromChildren()
    {
		for (auto&& subMesh : _approximationCollisionMesh->GetSubMeshes())
		{
		    auto&& approximationCollisionMesh = std::static_pointer_cast<ApproximationCollisionMesh>(subMesh);

		    const int32_t collisionFlags = approximationCollisionMesh->GetBulletObject()->getCollisionFlags();
			approximationCollisionMesh->GetBulletObject()->setCollisionFlags(collisionFlags & ~btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
		}
    }

    void ApproximationMeshCollider::UpdateBulletWorldTransform()
    {
		for (auto&& subMesh : _approximationCollisionMesh->GetSubMeshes())
		{
		    auto&& approximationCollisionMesh = std::static_pointer_cast<ApproximationCollisionMesh>(subMesh);
			approximationCollisionMesh->UpdateBulletTransform(_offset, _rotation, _size);
		}
    }

    void ApproximationMeshCollider::ApproximationCollisionMeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, [[maybe_unused]] const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::ApproximationCollisionMesh, "Data type must be approximation collision mesh!");

		const auto meshData = *static_cast<Utils::DragAndDropApproximationCollisionMeshData*>(data);
		const auto approximationCollisionMesh = meshData.Get();

		if (!approximationCollisionMesh)
		{
			DENTY_ASSERT(false, 
				"Approximation collision mesh doens't exists! (in ApproximationMeshCollider::ApproximationCollisionMeshAssetDragAndDropTargetCallback)"
			);
		}

		_approximationCollisionMesh = approximationCollisionMesh;
		_approximationCollisionMeshFilePath = _approximationCollisionMesh->GetFilePathToSerialized();

		AddWorld();
	}
}
