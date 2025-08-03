#include "stdafx.h"

#include "Collider.h"

#include "Component/Transform.h"

#include "Scene/GameObject.h"

#include "System/Core/Tag.h"

#include "System/Core/Setting/GlobalSettingsManager.h"

#include "System/Physics/PhysicsEngine.h"

#include "System/Layer/ApplicationLayer.h"

namespace DentyEngine
{
	Collider::Collider() : PhysicsBehavior(),
		_ghostObject(), _collisionInfo(), _type(), _collisionFlag(), _collisionFilter(), _isTrigger()
	{
		_multipleAllowed = false;
	}

	Collider::Collider(const Collider& source) :
		PhysicsBehavior(source), _type(source._type),
		_collisionFlag(source._collisionFlag),
		_collisionFilter(source._collisionFilter),
		_isTrigger(source._isTrigger)
	{
		_ghostObject = std::make_shared<btGhostObject>();

		_collisionInfo = std::make_shared<CollisionInfo>();
		_collisionInfo->Create(GetParent()->GetEntityID());
	}

	void Collider::Awake()
	{
		_ghostObject = std::make_shared<btGhostObject>();

		_collisionInfo = std::make_shared<CollisionInfo>();
		_collisionInfo->Create(GetParent()->GetEntityID());

		// Set flags
		{
		    if (_isTrigger)
		    {
				const int32_t collisionFlag = _ghostObject->getCollisionFlags();

		        _ghostObject->setCollisionFlags(
					collisionFlag | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE
				);
		    }
		}

		// Set tag number
		{
			const std::optional<uint32_t> tagNumber = Tag::GetTagNumberFrom(GetParent()->tag());
			if (tagNumber.has_value())
			{
		        _ghostObject->setUserIndex(static_cast<int32_t>(tagNumber.value()));
			}
			else
			{
			    _ghostObject->setUserIndex(-1);
			}
		}

		_ghostObject->setUserPointer(_collisionInfo.get());
	}

	void Collider::Start()
	{
	}

	void Collider::OnUpdate([[maybe_unused]] float deltaTime)
	{
		UpdateTransform();

		if (ApplicationLayer::IsPlay())
		{
		    _collisionInfo->Update();
		}
	}

	void Collider::OnFixedUpdate()
	{
		PhysicsBehavior::OnFixedUpdate();

		// Update transform.
		{
			UpdateTransform();
		}
	}

	void Collider::OnGui()
	{
		if (ImGui::CollapsingHeader("CollisionState##Collider", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// Trigger 
			{
				if (ImGui::Checkbox("IsTrigger##Collider", &_isTrigger))
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
			}

			// Collision filtering.
		    {
		        const CollisionFilterSetting& filterSetting = GlobalSettingsManager::GetCollisionFilterSetting();

				// TODO Do implementation collision filtering.
				if (ImGui::CollapsingHeader("CollisionFiltering##Collider", ImGuiTreeNodeFlags_DefaultOpen))
				{
					//const std::string groupName = DENTY_RETURN_VALUE_IF_OPTIONAL_HAS(filterSetting.GetNameAt(_collisionFilter.group), std::string);

					//ImGui::Text("CollisionGroup: %s", groupName.c_str());

					//ImGui::Separator();

					////filterSetting.SelectCollisionFilterGui(&_collisionFilter.group, &_collisionFilter.mask);
				}
			}
		}

		//ImGui::DragFloat3("Center##Collider", &_center.x, 0.01f);

		ImGui::Separator();

		_collisionInfo->OnGui();
	}

	void Collider::OnPrefabGui()
	{
		PhysicsBehavior::OnPrefabGui();

		// Get physics settings.
		//const auto physicsSettings = SettingsManager::GetInstance().GetPhysicsSettings();

		if (ImGui::CollapsingHeader("CollisionState##Collider", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// Trigger 
			{
				ImGui::Checkbox("IsTrigger##Collider", &_isTrigger);
			}

			//// Collision filtering.
			//{
			//	ImGui::InputInt("CollisionFilter##Collider", &_collisionFilter);

			//	// Clamp
			//	_collisionFilter = std::clamp(_collisionFilter, 0, PhysicsSettings::COLLISION_GROUP_AND_MASK_MAX - 1);

			//	// Get collision filter.
			//	const auto& collisionFilter = physicsSettings->GetCollisionFilterAt(_collisionFilter);
			//	ImGui::Text(collisionFilter.name.c_str());
			//}
		}

		ImGui::DragFloat3("Center##Collider", &_center.x, 0.01f);
	}

	void Collider::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void Collider::OnSerialized() const
	{
	}

	void Collider::OnDeserialized()
	{
		_ghostObject = std::make_shared<btGhostObject>();

		if (_isTrigger)
		{
			_ghostObject->setCollisionFlags(_collisionFlag | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
	}

	void Collider::OnDestroy()
	{
		PhysicsBehavior::OnDestroy();

		PhysicsEngine::GetInstance()->RemoveCollisionObject(shared_from_this());
	}

	void Collider::OnGameObjectDeserialized()
	{
		_collisionInfo = std::make_shared<CollisionInfo>();
		_collisionInfo->Create(GetParent()->GetEntityID());

		// Set collision shape transform.
		{
			btTransform btTransform = { };

			const Transform& transform = GetParent()->transform;
			Matrix orientation = {};
			Vector3 origin = {};

			if (GetParent()->HasParent())
			{
				orientation = transform.rotation.ConvertToMatrix();
				origin = transform.position + _center;
			}
			else
			{
				orientation = transform.localRotation.ConvertToMatrix();
				origin = transform.localPosition + _center;
			}

			btTransform.setBasis(orientation.ToBtTransform().getBasis());
			btTransform.setOrigin(origin.ToBtVector3());

			_ghostObject->setWorldTransform(btTransform);

			// Set user values.
			{
				const std::optional<uint32_t> number = Tag::GetTagNumberFrom(GetParent()->tag());
				if (number.has_value())
				{
			        _ghostObject->setUserIndex(static_cast<int32_t>(number.value()));
				}
				else
				{
				    _ghostObject->setUserIndex(-1);
				}
			    _ghostObject->setUserPointer(_collisionInfo.get());
			}
		}

		AddWorldReserved();
	}

	void Collider::UpdateState(const Vector3& position, const Quaternion& orientation)
	{
		Transform& transform = _parent.lock()->transform;

		transform.rotation = orientation;
		transform.position = position;
	}

	void Collider::OnSceneActivated()
	{
		_collisionInfo = std::make_shared<CollisionInfo>();
		_collisionInfo->Create(GetParent()->GetEntityID());

		if (_ghostObject)
		{
			_ghostObject->setUserPointer(_collisionInfo.get());
		}
	}

	void Collider::OnSceneDeActivated()
	{
	}

	void Collider::OnRuntimeSceneChanged()
	{
		UpdateTransform();

		_collisionInfo->Reset();
	}

	void Collider::OnGameObjectTagChanged()
	{
		if (not _ghostObject)
			return;

		const std::string& tag = GetParent()->tag();

		const std::optional<uint32_t> number = Tag::GetTagNumberFrom(tag);
		if (number.has_value())
		{
			_ghostObject->setUserIndex(static_cast<int32_t>(number.value()));
		}
		else
		{
			_ghostObject->setUserIndex(-1);
		}
	}

	void Collider::SetCollisionFilter([[maybe_unused]] int collisionFilter)
	{
		//_collisionFilter = std::clamp(collisionFilter, 0, PhysicsSettings::COLLISION_GROUP_AND_MASK_MAX - 1);
	}

	void Collider::AddWorld()
	{
		PhysicsEngine::GetInstance()->AddCollisionObject(shared_from_this());
	}

	void Collider::AddWorldReserved()
	{
		PhysicsEngine::GetInstance()->AddReservedCollisionObject(shared_from_this());
	}

	void Collider::UpdateTransform()
	{
		if (not _ghostObject)
			return;

		const Transform& transform = GetParent()->transform;

		Matrix orientation = Matrix::Identity();
		btTransform t = { };

		if (GetParent()->HasParent())
		{
			orientation = transform.world.ObtainWithoutScaling().Transpose();

			const btVector3 right = orientation.GetRightVec3().ToBtVector3();
			const btVector3 up = orientation.GetUpVec3().ToBtVector3();
			const btVector3 front =orientation.GetFrontVec3().ToBtVector3();

			btMatrix3x3 basis = { right, up, front };

			t = btTransform(basis, Vector3(transform.world.GetOffsetVec3() + _center).ToBtVector3());
		}
		else
		{
			orientation = transform.local.ObtainWithoutScaling().Transpose();

			const btVector3 right = orientation.GetRightVec3().ToBtVector3();
			const btVector3 up = orientation.GetUpVec3().ToBtVector3();
			const btVector3 front = orientation.GetFrontVec3().ToBtVector3();

			btMatrix3x3 basis = { right, up, front };

			t = btTransform(basis, Vector3(transform.local.GetOffsetVec3() + _center).ToBtVector3());
		}

		_ghostObject->setWorldTransform(t);
	}

   
}
