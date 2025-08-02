#include "stdafx.h"

#include "Component.h"

namespace DentyEngine
{
	Component::Component() :
		_parent(), _requiredComponentNames(), _componentID(),
		_inheritedType(InheritedType::Component), _enabled(true), _multipleAllowed(), _attachToChildAllowed(true)
	{
	}

	Component::Component(const Component& source) :
		Object(source), _parent(source._parent),
		_requiredComponentNames(source._requiredComponentNames),
		_inheritedType(source._inheritedType),
		_enabled(source._enabled),
		_multipleAllowed(source._multipleAllowed),
		_attachToChildAllowed(source._attachToChildAllowed)
	{
		// Recalculate id.
		_componentID.ReCalculate();
	}

	void Component::OnSerialized() const
	{
	}

	void Component::OnDeserialized()
	{
	}

	void Component::OnDestroy()
	{
		Object::OnDestroy();
	}

	Ref<Object> Component::MakeCopy() const
	{
		DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

		return nullptr;
	}

	void Component::SetParent(const Ref<GameObject>& parent)
	{
		_parent = parent;
	}

	GameObject* Component::GetParent() const
	{
		if (!_parent.expired())
		{
			return _parent.lock().get();
		}

		return nullptr;
	}
};