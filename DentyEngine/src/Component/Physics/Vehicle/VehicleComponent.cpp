#include "stdafx.h"

#include "VehicleComponent.h"

#include "Scene/GameObject.h"

#include "System/Core/Tag.h"

#include "System/Layer/ApplicationLayer.h"

#include "System/Physics/PhysicsEngine.h"

#include "System/Events/EventEditor.h"

namespace DentyEngine
{
	VehicleComponent::VehicleComponent() :
		PhysicsBehavior(), _car()
	{
		_name = "VehicleComponent";
	}

	VehicleComponent::VehicleComponent(const VehicleComponent& source) :
		PhysicsBehavior(source), _car(source._car)
	{
	}

	void VehicleComponent::Awake()
	{
		PhysicsBehavior::Awake();

		const std::optional<uint32_t> tagNumber = Tag::GetTagNumberFrom(GetParent()->tag());
		if (tagNumber.has_value())
		{
			_car.Create(GetParent()->GetEntityID(), tagNumber.value(), GetParent()->transform.local);
		}
		else
		{
			_car.Create(GetParent()->GetEntityID(), 0u, GetParent()->transform.local);
		}
	}

	void VehicleComponent::Start()
	{
		PhysicsBehavior::Start();
	}

	void VehicleComponent::OnUpdate(float deltaTime)
	{
		PhysicsBehavior::OnUpdate(deltaTime);

		if (ApplicationLayer::IsPlay())
		{
			_car.Update();
		}

		if (ApplicationLayer::IsEdit())
		{
			UpdateRigidBodyTransform();
		}
	}

	void VehicleComponent::OnFixedUpdate()
	{
		PhysicsBehavior::OnFixedUpdate();

		UpdateTransform();
	}

	void VehicleComponent::OnGui()
	{
		PhysicsBehavior::OnGui();
	}

	void VehicleComponent::OnPrefabGui()
	{
		PhysicsBehavior::OnPrefabGui();
	}

	void VehicleComponent::OnEvent(Event* e)
	{
		PhysicsBehavior::OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventOnScenePlayButtonPressed>(BIND_EVENT_FUNCTION(OnScenePlayButtonPressed));
	}

	void VehicleComponent::OnSerialized() const
	{
		PhysicsBehavior::OnSerialized();
	}

	void VehicleComponent::OnDeserialized()
	{
		PhysicsBehavior::OnDeserialized();
	}

	void VehicleComponent::OnDestroy()
	{
		PhysicsBehavior::OnDestroy();
	}

	void VehicleComponent::OnGameObjectDeserialized()
	{
		PhysicsBehavior::OnGameObjectDeserialized();
	}

	void VehicleComponent::OnSceneActivated()
	{
		const std::optional<uint32_t> tagNumber = Tag::GetTagNumberFrom(GetParent()->tag());
		if (tagNumber.has_value())
		{
			_car.Create(GetParent()->GetEntityID(), tagNumber.value(), GetParent()->transform.local);
		}
		else
		{
			_car.Create(GetParent()->GetEntityID(), 0u, GetParent()->transform.local);
		}

		AddWorld();
	}

	void VehicleComponent::OnSceneDeActivated()
	{
		//RemoveWorld();
	}

	void VehicleComponent::OnRuntimeSceneChanged()
	{
		Transform& transform = GetParent()->transform;
		transform.SetUpdateDisabled();

		_car.ResetCollisionInfo();
	}

	void VehicleComponent::OnGameObjectTagChanged()
	{

	}

	Ref<Object> VehicleComponent::MakeCopy() const
	{
		return std::make_shared<VehicleComponent>(*shared_from_this());
	}

	void VehicleComponent::AddWorld()
	{
		PhysicsEngine::GetInstance()->AddVehicle(shared_from_this());
	}

	void VehicleComponent::RemoveWorld()
	{
		PhysicsEngine::GetInstance()->RemoveVehicle(shared_from_this());
	}

    void VehicleComponent::Recreate()
    {
		RemoveWorld();

		const std::optional<uint32_t> tagNumber = Tag::GetTagNumberFrom(GetParent()->tag());

		if (tagNumber.has_value())
		{
			_car.Recreate(GetParent()->GetEntityID(), tagNumber.value(), GetParent()->transform.local);
		}
		else
		{
			_car.Recreate(GetParent()->GetEntityID(), 0u, GetParent()->transform.local);
		}

		AddWorld();
    }

    void VehicleComponent::UpdateRigidBodyTransform()
    {
		const Transform& transform = GetParent()->transform;

		const Matrix world = transform.world;
		_car.GetComponents().rigidBody->setWorldTransform(world.ToBtTransformWithoutScaling());
    }

    void VehicleComponent::UpdateTransform()
	{
		Transform& transform = GetParent()->transform;
		{
			btTransform trans;
			_car.GetComponents().rigidBody->getMotionState()->getWorldTransform(trans);

			Matrix world = Matrix::ToMatrix(trans);
			
			{
				const Matrix scaling = DirectX::XMMatrixScaling(
					transform.localScale.x, transform.localScale.y, transform.localScale.z
				);

				transform.localPosition = world.GetOffsetVec3();
				transform.local = world * scaling;
			}
		}
	}

	void VehicleComponent::OnScenePlayButtonPressed([[maybe_unused]] EventOnScenePlayButtonPressed* e)
	{
		using AppState = ApplicationLayer::State;

		const AppState state = ApplicationLayer::GetCurrentState();

		switch (state)
		{
		case AppState::Edit:
			// Enable transform update.
			{
				GetParent()->transform.SetUpdateEnabled();

				for (auto& child : GetParent()->GetChildren())
				{
					child->transform.SetUpdateEnabled();
				}
			}
			break;
		case AppState::Play:
			// Disable transform update.
			{
				GetParent()->transform.SetUpdateDisabled();

				for (auto& child : GetParent()->GetChildren())
				{
					child->transform.SetUpdateDisabled();
				}
			}
			break;
		}
	}
}
