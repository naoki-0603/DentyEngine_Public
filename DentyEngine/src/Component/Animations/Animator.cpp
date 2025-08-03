#include "stdafx.h"

#include "Animator.h"

#include "System/Core/ID/AnimatorControllerID.h"

namespace DentyEngine
{
	Animator::Animator() :
		Behavior(), _controller(), _speed(1.0f)
	{
		_name = "Animator";
	}

	Animator::Animator(const Animator& source) :
		Behavior(source),
		_speed(source._speed)
	{
	}

	void Animator::Awake()
	{
		Behavior::Awake();
	}

	void Animator::Start()
	{
		Behavior::Start();
	}

	void Animator::OnUpdate([[maybe_unused]] float deltaTime)
	{
		// Nothing to do...
	}

	void Animator::OnFixedUpdate()
	{
		Behavior::OnFixedUpdate();
	}

	void Animator::OnGui()
	{
		Behavior::OnGui();
	}

	void Animator::OnPrefabGui()
	{
		Behavior::OnGui();

		// Drag and drop
		{
			char buf[] = "Drag and drop here!";

			// drag and drop area.
			ImGui::InputText("Controller##Animator", buf, sizeof(buf), ImGuiInputTextFlags_ReadOnly);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AnimatorControllerItem"))
				{
					IM_ASSERT(payload->DataSize == sizeof(AnimatorControllerID));

					//// Cast to animator controller id.
					//const auto animatorControllerID = *static_cast<const AnimatorControllerID*>(payload->Data);

					//// Find from controller library.
					//const AnimatorControllerLibrary* controllerLibrary = LibraryManager::GetInstance().GetAnimatorControllerLibrary();

					//const auto controller = controllerLibrary->Find(animatorControllerID);

					//// Set mesh.
					//_controller = controller;
					//_filePathToSerialized = _controller->GetFilePathToSerialized();
				}
			}
		}
	}

	void Animator::OnEvent(Event* e)
	{
		Behavior::OnEvent(e);
	}

	void Animator::OnSerialized() const
	{
		Behavior::OnSerialized();
	}

	void Animator::OnDeserialized()
	{
		Behavior::OnDeserialized();
	}

	void Animator::Destroy()
	{
		Behavior::Destroy();
	}

	void Animator::OnDestroy()
	{
		Behavior::OnDestroy();
	}

	void Animator::OnGameObjectDeserialized()
	{
		Behavior::OnGameObjectDeserialized();
	}

	Ref<Object> Animator::MakeCopy() const
	{
		return std::make_shared<Animator>(*shared_from_this());
	}

    void Animator::Play()
    {
		if (HasController())
		{
		    _controller->OnPlay(_speed);
		}
    }

    void Animator::Pause()
    {
		if (HasController())
		{
		    _controller->OnPause();
		}
    }

    void Animator::Stop()
    {
		if (HasController())
		{
		    _controller->OnStop();
		}
    }

    void Animator::SetInt(std::string_view name, int32_t value)
	{
		if (!HasController())
		{
			DENTY_WARN_CONSOLE_LOG("Animator controller doesn't set!");
			return;
		}

		auto&& it = _controller->_parameters.find(std::string(name));
		if (it != _controller->_parameters.end())
		{
			if (it->second.value.valueType != AnimatorValue::Type::Int)
			{
				DENTY_WARN_CONSOLE_LOG("%s type is %s. (in Animator::SetInt)", std::string(name).c_str(), it->second.value.GetValueTypeAsString().c_str());
				return;
			}

			it->second.value.valueInt = value;
		}
	}

	void Animator::SetFloat(std::string_view name, float value)
	{
		if (!HasController())
		{
			DENTY_WARN_CONSOLE_LOG("Animator controller doesn't set!");
			return;
		}

		auto&& it = _controller->_parameters.find(std::string(name));
		if (it != _controller->_parameters.end())
		{
			if (it->second.value.valueType != AnimatorValue::Type::Float)
			{
				DENTY_WARN_CONSOLE_LOG("%s type is %s. (in Animator::SetFloat)", std::string(name).c_str(), it->second.value.GetValueTypeAsString().c_str());
				return;
			}

			it->second.value.valueFloat = value;
		}
	}

	void Animator::SetBool(std::string_view name, bool value)
	{
		if (!HasController())
		{
			DENTY_WARN_CONSOLE_LOG("Animator controller doesn't set!");
			return;
		}

		auto&& it = _controller->_parameters.find(std::string(name));
		if (it != _controller->_parameters.end())
		{
			if (it->second.value.valueType != AnimatorValue::Type::Bool)
			{
				DENTY_WARN_CONSOLE_LOG("%s type is %s. (in Animator::SetBool)", std::string(name).c_str(), it->second.value.GetValueTypeAsString().c_str());
				return;
			}

			it->second.value.valueBool = value;
		}
	}

	void Animator::SetTrigger(std::string_view name)
	{
		if (!HasController())
		{
			DENTY_WARN_CONSOLE_LOG("Animator controller doesn't set!");
			return;
		}

		auto&& it = _controller->_parameters.find(std::string(name));
		if (it != _controller->_parameters.end())
		{
			if (it->second.value.valueType != AnimatorValue::Type::Trigger)
			{
				DENTY_WARN_CONSOLE_LOG("%s type is %s. (in Animator::SetTrigger)", std::string(name).c_str(), it->second.value.GetValueTypeAsString().c_str());
				return;
			}

			it->second.value.valueTrigger = true;
		}
	}

    std::optional<int32_t> Animator::GetInt(std::string_view name) const
    {
		if (!HasController())
		{
			DENTY_WARN_CONSOLE_LOG("Animator controller doesn't set!");
			return std::nullopt;
		}

		auto&& it = _controller->_parameters.find(std::string(name));
		if (it != _controller->_parameters.end())
		{
			if (it->second.value.valueType != AnimatorValue::Type::Int)
			{
				DENTY_WARN_CONSOLE_LOG("%s type is %s. (in Animator::GetInt)", std::string(name).c_str(), it->second.value.GetValueTypeAsString().c_str());
				return std::nullopt;
			}

			return it->second.value.valueInt;
		}

		DENTY_ERR_CONSOLE_LOG("%s parameter not found!", std::string(name).c_str());

		return std::nullopt;
    }

    std::optional<float> Animator::GetFloat(std::string_view name) const
    {
		if (!HasController())
		{
			DENTY_WARN_CONSOLE_LOG("Animator controller doesn't set!");
			return std::nullopt;
		}

		auto&& it = _controller->_parameters.find(std::string(name));
		if (it != _controller->_parameters.end())
		{
			if (it->second.value.valueType != AnimatorValue::Type::Float)
			{
				DENTY_WARN_CONSOLE_LOG("%s type is %s. (in Animator::GetFloat)", std::string(name).c_str(), it->second.value.GetValueTypeAsString().c_str());
				return std::nullopt;
			}

			return it->second.value.valueFloat;
		}

		DENTY_ERR_CONSOLE_LOG("%s parameter not found!", std::string(name).c_str());

		return std::nullopt;
    }

    std::optional<bool> Animator::GetBool(std::string_view name) const
    {
		if (!HasController())
		{
			DENTY_WARN_CONSOLE_LOG("Animator controller doesn't set!");
			return std::nullopt;
		}

		auto&& it = _controller->_parameters.find(std::string(name));
		if (it != _controller->_parameters.end())
		{
			if (it->second.value.valueType != AnimatorValue::Type::Bool)
			{
				DENTY_WARN_CONSOLE_LOG("%s type is %s. (in Animator::GetBool)", std::string(name).c_str(), it->second.value.GetValueTypeAsString().c_str());
				return std::nullopt;
			}

			return it->second.value.valueBool;
		}

		DENTY_ERR_CONSOLE_LOG("%s parameter not found!", std::string(name).c_str());

		return std::nullopt;
    }
}
