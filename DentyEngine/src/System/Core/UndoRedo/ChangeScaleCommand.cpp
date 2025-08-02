#include "stdafx.h"

#include "ChangeScaleCommand.h"

#include "Scene/GameObject.h"

namespace DentyEngine
{
	ChangeScaleCommand::ChangeScaleCommand(GameObject* owner, const Vector3& oldScale, const Vector3& newScale) :
		ICommand(), _owner(owner), _oldScale(oldScale), _newScale(newScale)
	{
	}

	void ChangeScaleCommand::Redo()
	{
		_owner->transform.localScale = _newScale;
	}

	void ChangeScaleCommand::Undo()
	{
		_owner->transform.localScale = _oldScale;
	}
}
