#include "stdafx.h"

#include "PhysicsBehaviour.h"

#include "Component.h"

namespace DentyEngine
{
	PhysicsBehaviour::PhysicsBehaviour() :
		Behaviour()
	{
		_inheritedType = InheritedType::PhysicsBehaviour;
	}

	PhysicsBehaviour::PhysicsBehaviour(const PhysicsBehaviour& source) :
		Behaviour(source)
	{
	}

	void PhysicsBehaviour::Awake()
	{
		Behaviour::Awake();
	}

	void PhysicsBehaviour::Start()
	{
		Behaviour::Start();
	}

	void PhysicsBehaviour::OnUpdate([[maybe_unused]] float deltaTime)
	{
	}

	void PhysicsBehaviour::OnFixedUpdate()
	{
		Behaviour::OnFixedUpdate();
	}

	void PhysicsBehaviour::OnGui()
	{
		Behaviour::OnGui();
	}

	void PhysicsBehaviour::OnPrefabGui()
	{
		Behaviour::OnPrefabGui();
	}

	void PhysicsBehaviour::OnEvent(Event* e)
	{
		Behaviour::OnEvent(e);
	}

	void PhysicsBehaviour::OnDestroy()
	{
		Behaviour::OnDestroy();
	}

	void PhysicsBehaviour::Destroy()
	{
		Behaviour::Destroy();
	}

	void PhysicsBehaviour::OnSerialized() const
	{
		Behaviour::OnSerialized();
	}

	void PhysicsBehaviour::OnDeserialized()
	{
		Behaviour::OnDeserialized();
	}

	void PhysicsBehaviour::OnGameObjectDeserialized()
	{
		Behaviour::OnGameObjectDeserialized();
	}

	Ref<Object> PhysicsBehaviour::MakeCopy() const
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return nullptr;
	}
}