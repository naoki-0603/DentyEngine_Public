#pragma once

#include "ICommand.h"

#include "System/Core/ID/EntityID.h"

namespace DentyEngine
{
	class GameObject;

	class ChangePositionCommand final : public ICommand
	{
	public:
		ChangePositionCommand() = delete;
		ChangePositionCommand(GameObject* owner, const Vector3& oldPosition, const Vector3& newPosition);
		~ChangePositionCommand() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ChangePositionCommand)

		void Redo() override;
		void Undo() override;

		[[nodiscard]]
		const char* GetName() const override { return "ChangePositionCommand"; }
	private:
		GameObject* _owner;

		Vector3 _oldPosition;
		Vector3 _newPosition;
	};
}
