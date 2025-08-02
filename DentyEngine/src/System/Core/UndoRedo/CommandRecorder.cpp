#include "stdafx.h"

#include "CommandRecorder.h"

#include "ICommand.h"

namespace DentyEngine
{
	const uint32_t CommandRecorder::CONTAIN_COMMAND_MAX = 4096u;

	void CommandRecorder::OnUpdate([[maybe_unused]] float deltaTime)
	{
		const bool ctrl = Input::IsKeyPressed(KeyCode::LCtrl);

		if (ctrl)
		{
			// Redo
			if (Input::IsKeyTriggered(KeyCode::Y))
			{
				Redo();
			}
			// Undo
			else if (Input::IsKeyTriggered(KeyCode::Z))
			{
				Undo();
			}
		}
	}

	void CommandRecorder::Add(const Ref<ICommand>& command)
	{
		const uint32_t size = static_cast<uint32_t>(_commands.size());

		if (size >= CONTAIN_COMMAND_MAX)
		{
			_commands.pop_front();

			--_currentIndex;
		}

		if (_currentIndex == _commands.size())
		{
			_commands.emplace_back(command);
		}
		else
		{
			const auto it = std::next(_commands.begin(), _currentIndex);

			_commands.insert(it, command);
		}

		++_currentIndex;
	}

	void CommandRecorder::OnGui()
	{
		if (ImGui::CollapsingHeader("CommandRecorder##CoreLayer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Clear##CommandRecorder"))
			{
				Clear();
			}

			ImGui::Text("CurrentIndex: %u", _currentIndex);
			ImGui::Text("CurrentSize: %u", _commands.size());

			if (ImGui::TreeNodeEx("History##CommandRecorder", ImGuiTreeNodeFlags_Framed))
			{
				for (const auto& command : _commands)
				{
					ImGui::Text("%s", command->GetName());
				}

				ImGui::TreePop();
			}
		}
		
	}

	void CommandRecorder::Clear()
	{
		_commands.clear();
		_currentIndex = 0u;
	}

	void CommandRecorder::Undo()
	{
		if (_currentIndex <= 0u)
			return;

		--_currentIndex;

		uint32_t index = 0u;
		for (const auto& command : _commands)
		{
			if (index != _currentIndex)
			{
				++index;
				continue;
			}

			command->Undo();
			break;
		}
	}

	void CommandRecorder::Redo()
	{
		if (_currentIndex >= CONTAIN_COMMAND_MAX)
			return;

		uint32_t index = 0u;
		for (const auto& command : _commands)
		{
			if (index != _currentIndex)
			{
				++index;
				continue;
			}

			command->Redo();

			++_currentIndex;
			break;
		}
	}
}
