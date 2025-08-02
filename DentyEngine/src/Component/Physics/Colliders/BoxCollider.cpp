#include "stdafx.h"

#include "BoxCollider.h"

#include "Component/Transform.h"

#include "Scene/GameObject.h"

#include "System/Physics/PhysicsEngine.h"

namespace DentyEngine
{
	BoxCollider::BoxCollider() : Collider(), _boxShape(), _center()
	{
		_type = Type::Box;
		_name = "BoxCollider";
	}

	BoxCollider::BoxCollider(const BoxCollider& source) :
		Collider(source), _boxShape(), _center(source._center)
	{
		if (source._boxShape)
		{
			_boxShape = std::make_shared<BoxShape>(*source._boxShape);
		}
	}

	void BoxCollider::Awake()
	{
		Collider::Awake();

		// Create box shape.
		{
			if (not _boxShape)
			{
			    _boxShape = std::make_shared<BoxShape>();
			    _boxShape->Create();
			}

			_ghostObject->setCollisionShape(_boxShape->GetBtCollisionShape().get());
		}

		//  Add to physics engine.
		{
			PhysicsEngine::GetInstance()->AddCollisionObject(shared_from_this());
		}
	}

	void BoxCollider::Start()
	{
		Collider::Start();

		{
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
	}

	void BoxCollider::OnUpdate(float deltaTime)
	{
		Collider::OnUpdate(deltaTime);

		ApplyScaling();
	}

	void BoxCollider::OnFixedUpdate()
	{
		Collider::OnFixedUpdate();
	}

	void BoxCollider::OnGui()
	{
		Collider::OnGui();

		if (not _boxShape)
			return;

		_boxShape->OnGui();
	}

	void BoxCollider::OnPrefabGui()
	{
		Collider::OnPrefabGui();

		if (not _boxShape)
			return;

		_boxShape->OnGui();
	}

	void BoxCollider::OnEvent(Event* e)
	{
		Collider::OnEvent(e);
	}

	void BoxCollider::OnSerialized() const
	{
		Collider::OnSerialized();
	}

	void BoxCollider::OnDeserialized()
	{
		Collider::OnDeserialized();

		if (not _ghostObject || not _boxShape)
			return;

		_ghostObject->setCollisionShape(_boxShape->GetBtCollisionShape().get());
	}

	void BoxCollider::OnDestroy()
	{
		Collider::OnDestroy();
	}

	void BoxCollider::OnGameObjectDeserialized()
	{
		Collider::OnGameObjectDeserialized();

		ApplyScaling();
	}

	void BoxCollider::ApplyScaling()
	{
		if (not _boxShape)
			return;

		const Transform& transform = GetParent()->transform;

		const Vector3& size = _boxShape->GetSize();
		const bool hasParent = GetParent()->HasParent();

		if (hasParent)
		{
			_boxShape->Update(transform.scale + size);
		}
		else
		{
			_boxShape->Update(transform.localScale + size);
		}
	}

	Ref<Object> BoxCollider::MakeCopy() const
	{
		return std::make_shared<BoxCollider>(*std::static_pointer_cast<const BoxCollider>(shared_from_this()));
	}

	void BoxCollider::OnSceneActivated()
	{
		Collider::OnSceneActivated();

		if (not _ghostObject)
			return;

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

	void BoxCollider::OnSceneDeActivated()
	{
		//PhysicsEngine::GetInstance()->RemoveCollisionObject(shared_from_this());
	}

	void BoxCollider::OnGameObjectTagChanged()
	{
		Collider::OnGameObjectTagChanged();
	}
}
