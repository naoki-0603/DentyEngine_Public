#pragma once

#include "System/Core/Singleton.h"

namespace DentyEngine
{
	class ICommand;

	// https://qiita.com/makotox/items/f284f7485106ea05c305
	class CommandRecorder final : public DesignPattern::Singleton<CommandRecorder>
	{
		friend class Singleton;
	public:
		~CommandRecorder() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(CommandRecorder)

		void OnUpdate(float deltaTime);

		void Add(const Ref<ICommand>& command);

		void OnGui();

		void Clear();
	private:
		CommandRecorder() : _commands(), _currentIndex() {}

		void Undo();
		void Redo();
	private:
		std::list<Ref<ICommand>> _commands;

		uint32_t _currentIndex;
	private:
		//
		// Constants
		//
		static const uint32_t CONTAIN_COMMAND_MAX;
	};
}