#pragma once

#include "ICommand.h"

namespace DentyEngine
{
	class GameObject;

	class ChangeScaleCommand final : public ICommand
	{
	public:
		ChangeScaleCommand() = delete;
		ChangeScaleCommand(GameObject* owner, const Vector3& oldScale, const Vector3& newScale);
		~ChangeScaleCommand() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ChangeScaleCommand)

		void Redo() override;
		void Undo() override;

		[[nodiscard]]
		const char* GetName() const override { return "ChangeScaleCommand"; }
	private:
		GameObject* _owner;

		Vector3 _oldScale;
		Vector3 _newScale;
	};
}
