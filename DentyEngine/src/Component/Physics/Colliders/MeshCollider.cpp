#include "stdafx.h"

#include "MeshCollider.h"

#include "Component/Graphics/Renderer/SkinnedMeshRenderer.h"

#include "Scene/GameObject.h"

#include "System/Core/Tag.h"

#include "System/Editor/DragAndDrop/DragAndDropTarget.h"

#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	MeshCollider::MeshCollider() :
		Collider(), _shape()
	{
		_type = Type::Mesh;
		_name = "MeshCollider";
	}

	MeshCollider::MeshCollider(const MeshCollider& source) :
		Collider(source)
	{
		if (source._shape)
		{
			_shape = std::make_shared<StaticMeshShape>(*source._shape);
		}
	}

	void MeshCollider::Awake()
	{
		Collider::Awake();

		_shape = std::make_shared<StaticMeshShape>();
	}

	void MeshCollider::Start()
	{
		Collider::Start();
	}

	void MeshCollider::OnUpdate(float deltaTime)
	{
		if (_shape)
		{
			Collider::OnUpdate(deltaTime);

			ApplyScaling();
		}
	}

	void MeshCollider::OnFixedUpdate()
	{
		if (_shape)
		{
			Collider::OnFixedUpdate();
		}
	}

	void MeshCollider::OnEvent(Event* e)
	{
		Collider::OnEvent(e);
	}

	void MeshCollider::OnGui()
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

	void MeshCollider::OnPrefabGui()
	{
		Collider::OnPrefabGui();
	}

	void MeshCollider::OnSerialized() const
	{
	}

	void MeshCollider::OnDeserialized()
	{
		Collider::OnDeserialized();
	}

	void MeshCollider::OnDestroy()
	{
		Collider::OnDestroy();
	}

	void MeshCollider::OnGameObjectDeserialized()
	{
		Collider::OnGameObjectDeserialized();

		if (_shape)
		{
			ApplyScaling();

			_ghostObject->setCollisionShape(_shape->GetBtCollisionShape().get());
		}
	}

	void MeshCollider::OnSceneActivated()
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

		const bool hasCreated = std::static_pointer_cast<StaticMeshShape>(GetCollisionShape())->HasCreated();
		if (hasCreated)
		{
			SetGhostObjectCollisionShape(GetCollisionShape()->GetBtCollisionShape());

			ApplyScaling();

			PhysicsEngine::GetInstance()->AddCollisionObject(shared_from_this());
		}
	}

	void MeshCollider::OnSceneDeActivated()
	{
		//PhysicsEngine::GetInstance()->RemoveCollisionObject(shared_from_this());
	}

	void MeshCollider::OnGameObjectTagChanged()
	{
		Collider::OnGameObjectTagChanged();
	}

	Ref<Object> MeshCollider::MakeCopy() const
	{
		return std::make_shared<MeshCollider>(*std::static_pointer_cast<const MeshCollider>(shared_from_this()));
	}

	void MeshCollider::ApplyScaling()
	{
		const Transform& transform = GetParent()->transform;

		if (_shape)
		{
			if (GetParent()->HasParent())
			{
				_shape->Update(transform.scale);
			}
			else
			{
				_shape->Update(transform.localScale);
			}
		}
	}

	void MeshCollider::CollisionMeshAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData)
	{
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