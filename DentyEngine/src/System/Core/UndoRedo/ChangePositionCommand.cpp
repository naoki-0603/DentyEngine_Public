#include "stdafx.h"

#include "ChangePositionCommand.h"

#include "Scene/GameObject.h"

namespace DentyEngine
{
	ChangePositionCommand::ChangePositionCommand(GameObject* owner, const Vector3& oldPosition, const Vector3& newPosition) :
		ICommand(), _owner(owner), _oldPosition(oldPosition), _newPosition(newPosition)
	{
	}

	void ChangePositionCommand::Redo()
	{
		_owner->transform.localPosition = _newPosition;
	}

	void ChangePositionCommand::Undo()
	{
		_owner->transform.localPosition = _oldPosition;
	}
}
