#include "stdafx.h"

#include "SphereCollider.h"

#include "Component/Transform.h"

#include "Scene/GameObject.h"

#include "System/Core/Tag.h"

#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	SphereCollider::SphereCollider() : Collider(), _shape()
	{
		_type = Type::Sphere;
		_name = "SphereCollider";
	}

	SphereCollider::SphereCollider(const SphereCollider& source) :
		Collider(source), _shape(source._shape)
	{
	}

	void SphereCollider::Awake()
	{
		Collider::Awake();

		// Create collision shape.
		{
			_shape = std::make_shared<SphereShape>();
			_shape->Create();

			_ghostObject->setCollisionShape(_shape->GetBtCollisionShape().get());
		}

		// Add to physics world.
		{
			PhysicsEngine::GetInstance()->AddCollisionObject(shared_from_this());
		}
	}

	void SphereCollider::Start()
	{
		Collider::Start();
	}

	void SphereCollider::OnUpdate(float deltaTime)
	{
		Collider::OnUpdate(deltaTime);

		ApplyScaling();
	}

	void SphereCollider::OnFixedUpdate()
	{
		Collider::OnFixedUpdate();
	}

	void SphereCollider::OnGui()
	{
		Collider::OnGui();

		_shape->OnGui();
	}

	void SphereCollider::OnPrefabGui()
	{
		Collider::OnPrefabGui();

		_shape->OnGui();
	}

	void SphereCollider::OnEvent(Event* e)
	{
		Collider::OnEvent(e);
	}

	void SphereCollider::OnSerialized() const
	{
		Collider::OnSerialized();
	}

	void SphereCollider::OnDeserialized()
	{
		Collider::OnDeserialized();

		_ghostObject->setCollisionShape(_shape->GetBtCollisionShape().get());
	}

	void SphereCollider::OnDestroy()
	{
		Collider::OnDestroy();
	}

	void SphereCollider::OnGameObjectDeserialized()
	{
		Collider::OnGameObjectDeserialized();

		ApplyScaling();
	}

	void SphereCollider::OnSceneActivated()
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

	void SphereCollider::OnSceneDeActivated()
	{
		//PhysicsEngine::GetInstance()->RemoveCollisionObject(shared_from_this());
	}

	void SphereCollider::OnGameObjectTagChanged()
	{
		Collider::OnGameObjectTagChanged();
	}

	Ref<Object> SphereCollider::MakeCopy() const
	{
		return std::make_shared<SphereCollider>(*std::static_pointer_cast<const SphereCollider>(shared_from_this()));
	}

	void SphereCollider::ApplyScaling()
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
}
