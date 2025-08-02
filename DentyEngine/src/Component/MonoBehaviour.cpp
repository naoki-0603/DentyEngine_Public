#include "stdafx.h"

#include "MonoBehaviour.h"

#include "Scene/GameObject.h"

namespace DentyEngine
{
	MonoBehaviour::MonoBehaviour() :
		Behaviour()
	{
		_inheritedType = InheritedType::MonoBehaviour;
	}

	MonoBehaviour::MonoBehaviour(const MonoBehaviour& source) :
		Behaviour(source)
	{
	}

	void MonoBehaviour::Awake()
	{
		
	}

	void MonoBehaviour::Start()
	{
		
	}

	void MonoBehaviour::OnUpdate([[maybe_unused]] float deltaTime)
	{

	}

	void MonoBehaviour::OnFixedUpdate()
	{

	}

	void MonoBehaviour::OnLateUpdate([[maybe_unused]] float deltaTime)
	{
	}

	void MonoBehaviour::OnEnabled()
	{
	}

	void MonoBehaviour::OnDisable()
	{
	}

	void MonoBehaviour::OnGui()
	{
	}

	void MonoBehaviour::OnPrefabGui()
	{
		Behaviour::OnPrefabGui();
	}

	void MonoBehaviour::OnSerialized() const
	{
		Behaviour::OnSerialized();
	}

	void MonoBehaviour::OnDeserialized()
	{
		Behaviour::OnDeserialized();
	}

	void MonoBehaviour::OnDestroy()
	{
		Behaviour::OnDestroy();
	}

	void MonoBehaviour::OnGameObjectDeserialized()
	{
		Behaviour::OnGameObjectDeserialized();
	}

	Ref<Object> MonoBehaviour::MakeCopy() const
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return nullptr;
	}
}
