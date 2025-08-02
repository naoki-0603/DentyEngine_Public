#include "stdafx.h"

#include "CapsuleCollider.h"

#include "Component/Transform.h"

#include "Scene/GameObject.h"

#include "System/Core/Tag.h"

#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	CapsuleCollider::CapsuleCollider() :
		Collider(), _shape()
	{
		_type = Type::Capsule;
		_name = "CapsuleCollider";
	}

	CapsuleCollider::CapsuleCollider(const CapsuleCollider& source) :
		Collider(source), _shape(source._shape)
	{
	}

	void CapsuleCollider::Awake()
	{
		Collider::Awake();

		// Create capsule shape.
		{
			_shape = std::make_shared<CapsuleShape>();
			_shape->Create();

			_ghostObject->setCollisionShape(_shape->GetBtCollisionShape().get());
		}

		//  Add to physics engine.
		{
			PhysicsEngine::GetInstance()->AddCollisionObject(shared_from_this());
		}
	}

	void CapsuleCollider::Start()
	{
		Collider::Start();

		const Transform& transform = GetParent()->transform;
		if (GetParent()->HasParent())
		{
			_center = transform.position;
		}
		else
		{
			_center = transform.localPosition;
		}
	}

	void CapsuleCollider::OnUpdate(float deltaTime)
	{
		Collider::OnUpdate(deltaTime);

		ApplyScaling();
	}

	void CapsuleCollider::OnFixedUpdate()
	{
		Collider::OnFixedUpdate();
	}

	void CapsuleCollider::OnEvent(Event* e)
	{
		Collider::OnEvent(e);
	}

	void CapsuleCollider::OnGui()
	{
		Collider::OnGui();

		_shape->OnGui();
	}

	void CapsuleCollider::OnPrefabGui()
	{
		Collider::OnPrefabGui();

		_shape->OnGui();
	}

	void CapsuleCollider::OnSerialized() const
	{
		Collider::OnSerialized();
	}

	void CapsuleCollider::OnDeserialized()
	{
		Collider::OnDeserialized();

		_ghostObject->setCollisionShape(_shape->GetBtCollisionShape().get());
	}

	void CapsuleCollider::OnDestroy()
	{
		Collider::OnDestroy();
	}

	void CapsuleCollider::OnGameObjectDeserialized()
	{
		Collider::OnGameObjectDeserialized();

		ApplyScaling();
	}

	void CapsuleCollider::OnSceneActivated()
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

		if (GetCollisionShape())
		{
			SetGhostObjectCollisionShape(GetCollisionShape()->GetBtCollisionShape());

			ApplyScaling();

			PhysicsEngine::GetInstance()->AddCollisionObject(shared_from_this());
		}
	}

	void CapsuleCollider::OnSceneDeActivated()
	{
		//PhysicsEngine::GetInstance()->RemoveCollisionObject(shared_from_this());
	}

	void CapsuleCollider::OnGameObjectTagChanged()
	{
		Collider::OnGameObjectTagChanged();
	}

	Ref<Object> CapsuleCollider::MakeCopy() const
	{
		return std::make_shared<CapsuleCollider>(*std::static_pointer_cast<const CapsuleCollider>(shared_from_this()));
	}

	void CapsuleCollider::ApplyScaling()
	{
		const Transform& transform = GetParent()->transform;

		if (GetParent()->HasParent())
		{
			Vector3 size = transform.scale;
			size.y += _shape->GetHeight();

			_shape->Update(size);
		}
		else
		{
			Vector3 size = transform.localScale;
			size.y += _shape->GetHeight();

			_shape->Update(size);
		}
	}
}
