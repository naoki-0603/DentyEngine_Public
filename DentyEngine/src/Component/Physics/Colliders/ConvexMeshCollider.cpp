#include "stdafx.h"

#include "ConvexMeshCollider.h"

#include "Scene/GameObject.h"

#include "Component/Transform.h"

#include "System/Core/Tag.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	ConvexMeshCollider::ConvexMeshCollider() :
		Collider(), _shape()
	{
		_type = Type::ConvexMesh;
		_name = "ConvexMeshCollider";
	}

	ConvexMeshCollider::ConvexMeshCollider(const ConvexMeshCollider& source) :
		Collider(source), _shape(source._shape)
	{
	}

	void ConvexMeshCollider::Awake()
	{
		Collider::Awake();

		_shape = std::make_shared<ConvexMeshShape>();
	}

	void ConvexMeshCollider::Start()
	{
		Collider::Start();
	}

	void ConvexMeshCollider::OnUpdate(float deltaTime)
	{
		Collider::OnUpdate(deltaTime);

		if (_shape)
		{
			ApplyScaling();
		}
	}

	void ConvexMeshCollider::OnFixedUpdate()
	{
		Collider::OnFixedUpdate();
	}

	void ConvexMeshCollider::OnEvent(Event* e)
	{
		Collider::OnEvent(e);
	}

	void ConvexMeshCollider::OnGui()
	{
		Collider::OnGui();

		if (_shape)
		{
			_shape->OnGui();
		}

		if (ImGui::CollapsingHeader("CollisionMesh##MeshCollider", ImGuiTreeNodeFlags_DefaultOpen))
		{
			const auto regionSize = ImGui::GetContentRegionAvail();

			ImGui::Text("DragAndDropHere!");
			ImGui::InvisibleButton("DragAndDropHere##MeshCollider", ImVec2(regionSize.x, 32.0f));

			// Target
			{
				const DragAndDropSpecification specification =
				{
					"CollisionMeshItem", sizeof(Utils::DragAndDropCollisionMeshData),
					nullptr, DragAndDropDataType::CollisionMesh,
					DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(CollisionMeshAssetDragAndDropTargetCallback),
					DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
				};
				DragAndDropTarget dragAndDropTarget(specification);
			}
		}
	}

	void ConvexMeshCollider::OnPrefabGui()
	{
		Collider::OnPrefabGui();
	}

	void ConvexMeshCollider::OnSerialized() const
	{
		Collider::OnSerialized();
	}

	void ConvexMeshCollider::OnDeserialized()
	{
		Collider::OnDeserialized();

		if (_shape)
		{
			_ghostObject->setCollisionShape(_shape->GetBtCollisionShape().get());
		}
	}

	void ConvexMeshCollider::OnDestroy()
	{
		Collider::OnDestroy();
	}

	void ConvexMeshCollider::OnGameObjectDeserialized()
	{
		Collider::OnGameObjectDeserialized();
	}

	void ConvexMeshCollider::OnSceneActivated()
	{
		Collider::OnSceneActivated();


		std::optional<uint32_t> tagNumber = Tag::GetTagNumberFrom(GetParent()->tag());
		if (tagNumber.has_value())
		{
			_ghostObject->setUserIndex(static_cast<int32_t>(tagNumber.value()));
		}

		// Restore flags.
		{
			if (_isTrigger)
			{
				_collisionFlag = _ghostObject->getCollisionFlags();
				_ghostObject->setCollisionFlags(_collisionFlag | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			}
			else
			{
				_collisionFlag = _ghostObject->getCollisionFlags();
				_ghostObject->setCollisionFlags(_collisionFlag & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
			}
		}

		const bool hasCreated = std::static_pointer_cast<ConvexMeshShape>(GetCollisionShape())->HasCreated();
		if (hasCreated)
		{
			SetGhostObjectCollisionShape(GetCollisionShape()->GetBtCollisionShape());

			ApplyScaling();

			PhysicsEngine::GetInstance()->AddCollisionObject(shared_from_this());
		}
	}

	void ConvexMeshCollider::OnSceneDeActivated()
	{
		//PhysicsEngine::GetInstance()->RemoveCollisionObject(shared_from_this());
	}

	void ConvexMeshCollider::OnGameObjectTagChanged()
	{
		Collider::OnGameObjectTagChanged();
	}

	Ref<Object> ConvexMeshCollider::MakeCopy() const
	{
		return std::make_shared<ConvexMeshCollider>(*std::static_pointer_cast<const ConvexMeshCollider>(shared_from_this()));
	}

	void ConvexMeshCollider::ApplyScaling()
	{
		const Transform& transform = GetParent()->transform;

		if (GetParent()->HasParent())
		{
			_shape->Update(transform.scale);
		}
		else
		{
			_shape->Update(transform.localScale);
		}
	}

	void ConvexMeshCollider::CollisionMeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData)
	{
		DENTY_ASSERT(dataType == DragAndDropDataType::CollisionMesh, "Data type must be collision mesh!");

		if (_shape->HasCreated())
		{
			PhysicsEngine::GetInstance()->RemoveCollisionObject(shared_from_this());
		}

		_shape->Reset();
		_shape->OnMeshAssetReceived(data, dataType, userData);

		_ghostObject->setCollisionShape(_shape->GetBtCollisionShape().get());

		AddWorld();
	}
}
