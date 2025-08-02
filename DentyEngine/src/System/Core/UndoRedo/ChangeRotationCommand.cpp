#include "stdafx.h"

#include "ChangeRotationCommand.h"

#include "Scene/GameObject.h"

namespace DentyEngine
{
	ChangeRotationCommand::ChangeRotationCommand(GameObject* owner, const Vector3& oldEulerAngles, const Vector3& newEulerAngles) :
		ICommand(), _owner(owner), _oldEulerAngles(oldEulerAngles), _newEulerAngles(newEulerAngles)
	{
	}

	void ChangeRotationCommand::Redo()
	{
		_owner->transform.localEulerAngles = _newEulerAngles;
	}

	void ChangeRotationCommand::Undo()
	{
		_owner->transform.localEulerAngles = _oldEulerAngles;
	}
}
