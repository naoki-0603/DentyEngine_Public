#include "stdafx.h"

#include "Behaviour.h"

namespace DentyEngine
{
	Behaviour::Behaviour() :
		Component()
	{
		_inheritedType = InheritedType::Behaviour;
	}

	Behaviour::Behaviour(const Behaviour& source) :
		Component(source)
	{
	}

	void Behaviour::Awake()
	{
	}

	void Behaviour::Start()
	{
	}

	void Behaviour::OnGui()
	{
	}

	void Behaviour::OnPrefabGui()
	{
	}

	void Behaviour::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void Behaviour::OnSerialized() const
	{
		Component::OnSerialized();
	}

	void Behaviour::OnDeserialized()
	{
		Component::OnDeserialized();
	}

	void Behaviour::OnDestroy()
	{
		Component::OnDestroy();
	}

	void Behaviour::OnGameObjectDeserialized()
	{
	}

	Ref<Object> Behaviour::MakeCopy() const
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return nullptr;
	}
}
