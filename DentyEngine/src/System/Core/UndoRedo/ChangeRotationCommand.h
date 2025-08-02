#pragma once

#include "ICommand.h"

namespace DentyEngine
{
	class GameObject;

	class ChangeRotationCommand final : public ICommand
	{
	public:
		ChangeRotationCommand() = delete;
		ChangeRotationCommand(GameObject* owner, const Vector3& oldEulerAngles, const Vector3& newEulerAngles);
		~ChangeRotationCommand() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ChangeRotationCommand)

		void Redo() override;
		void Undo() override;

		[[nodiscard]]
		const char* GetName() const override { return "ChangeRotationCommand"; }
	private:
		GameObject* _owner;

		Vector3 _oldEulerAngles;
		Vector3 _newEulerAngles;
	};
}
