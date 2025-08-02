#pragma once

namespace DentyEngine
{
	class ICommand
	{
	public:
		ICommand();
		virtual ~ICommand() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ICommand)

		virtual void Undo() = 0;
		virtual void Redo() = 0;

		[[nodiscard]]
		virtual const char* GetName() const = 0;
	};
}