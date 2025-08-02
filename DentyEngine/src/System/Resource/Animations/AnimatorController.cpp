#include "stdafx.h"

#include "AnimatorController.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

#include "System/Serializer/AnimatorControllerSerializer.h"

#include "Graphics/Renderer/Texture2D.h"

#include "Editor/Animation/States/EntryState.h"
#include "Editor/Animation/States/ExitState.h"
#include "Editor/Animation/States/AnyState.h"
#include "Editor/Animation/States/AnimationState.h"

namespace DentyEngine
{
	const String AnimatorController::ADD_FILENAME_AT_END = "_DentyAnimatorController";

	Ref<Texture2D> AnimatorController::_animatorControllerAssetTexture = nullptr;

	AnimatorController::AnimatorController() :
		Object(), Asset(), _animator(), _animationClips(), _parameters(),
		_reservedRemoveParameters(), _states(), _animatorControllerID(), _editParameterName(), _selectedState()
	{
	}

    void AnimatorController::Create(std::string_view name, const std::filesystem::path& filePath)
    {
		_name = name;
		_filePath = filePath;
		_filePathToSerialized = filePath;

		// Create entry, exit and any states.
		{
			const auto entry = std::make_shared<EntryState>(shared_from_this());
			const auto exit = std::make_shared<ExitState>(shared_from_this());
			const auto any = std::make_shared<AnyState>(shared_from_this());

			_states.try_emplace(entry->GetName(), entry);
			_states.try_emplace(exit->GetName(), exit);
			_states.try_emplace(any->GetName(), any);
		}

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Animator controller created! (name: " << name << ", filePath: " << filePath << ")";
    }

    void AnimatorController::Update()
    {
		// Remove reserved parameters.
		{
			for (const auto& reservedParameterName : _reservedRemoveParameters)
			{
				auto&& it = _parameters.find(reservedParameterName);
				if (it != _parameters.end())
				{
					_parameters.erase(it);
				}
			}
			_reservedRemoveParameters.clear();
		}

		// Remove reserved states.
		{
			for (const auto& reservedStateName : _reservedRemoveStates)
			{
				auto&& it = _states.find(reservedStateName);

				// It should be animation state.
				{
					const auto animationState = std::static_pointer_cast<AnimationState>(it->second);
					auto&& clipIt = _animationClips.find(animationState->GetAnimationClip()->GetName());
					if (clipIt != _animationClips.end())
					{
						_animationClips.erase(clipIt);
					}
				}

				if (it != _states.end())
				{
					_states.erase(it);
				}

				
			}
			_reservedRemoveStates.clear();
		}

		// Update states.
		for (auto&& state : _states | std::views::values)
		{
		    state->OnUpdate();
		}
    }

    void AnimatorController::OnEvent(Event* e)
	{
		Object::OnEvent(e);
	}

	void AnimatorController::Destroy()
	{
		Object::Destroy();
	}

	void AnimatorController::OnDestroy()
	{
		Object::OnDestroy();
	}

	void AnimatorController::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void AnimatorController::OnDeserialized()
	{
		Object::OnDeserialized();
	}

    void AnimatorController::OnLoaded()
    {
		for (const auto& state : _states | std::views::values)
		{
		    state->OnAnimatorControllerLoaded();
		}

		for (const auto& state : _states)
		{
		    if (std::string(state.second->GetStateType()) != "AnimationState")
				continue;

			const auto animationState = std::static_pointer_cast<AnimationState>(state.second);

			_animationClips.try_emplace(animationState->GetName(), animationState->GetAnimationClip());
		}
    }

    void AnimatorController::Save(bool isUpdate) const
	{
		// If failed.
		if (!AnimatorControllerSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " animator controller!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " animator controller!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " animator controller!";

		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " animator controller!";
		}
	}

	void AnimatorController::Load(const std::filesystem::path& filePath)
	{
		auto animatorController = std::make_shared<AnimatorController>();

		// If failed
		if (!AnimatorControllerSerializer::DeserializeFromJson(filePath, animatorController))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " animator controller!";

			return;
		}

		animatorController->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " animator controller!";
	}

	void AnimatorController::Rename(std::string_view newName, [[maybe_unused]] bool deleteOldFile)
	{
		_name = newName;

		/*std::filesystem::remove(_filePathToSerialized);

		DENTY_REPLACE_FILENAME(_filePath, newName);
		DENTY_REPLACE_FILENAME(_filePathToSerialized, newName);*/
	}

	void AnimatorController::Clone(const Ref<Asset>& asset)
	{
		DENTY_ASSERT(asset->GetAssetType() == AssetType::Controller, "Asset type must be controller! (in AnimatorController::Clone)");

		auto&& target = std::static_pointer_cast<AnimatorController>(asset);

		// Object
		{
		    target->_name = _name;
			target->_uuid = _uuid;
		}

		// Asset
		{
			target->_previewImage = _previewImage;
			target->_previewImageFilePath = _previewImageFilePath;
		    target->_filePath = _filePath;
			target->_filePathToSerialized = _filePathToSerialized;
		}

		// This class
		{
			target->_animator = _animator;
			target->_states = _states;
			target->_parameters = _parameters;
			target->_animatorControllerID = _animatorControllerID;
		}
	}

	void AnimatorController::OnReload()
	{
	}

	void AnimatorController::OnParameterGui()
	{
		for (auto&& parameter : _parameters)
		{
			const auto& key = parameter.first;
			const auto& label = "##" + key + "_ValueType";
			auto& second = parameter.second;

			ImGui::Text(key.c_str());
			const auto offsetFromStart = ImGui::CalcTextSize(key.c_str()).x * 2.0f;

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				_editParameterName.oldName = key;
				_editParameterName.newName = _editParameterName.oldName;
				_editParameterName.isEditing = true;
			}

			ImGui::SameLine(offsetFromStart);

			switch (second.value.valueType)
			{
			case AnimatorValue::Type::Int:
				ImGui::InputInt(label.c_str(), &second.value.valueInt);
				break;
			case AnimatorValue::Type::Float:
				ImGui::InputFloat(label.c_str(), &second.value.valueFloat);
				break;
			case AnimatorValue::Type::Bool:
				ImGui::Checkbox(label.c_str(), &second.value.valueBool);
				break;
			case AnimatorValue::Type::Trigger:
				ImGui::RadioButton(label.c_str(), second.value.valueTrigger);
				break;
			default:
				DENTY_ASSERT(false, "AnimatorController::OnParameterGui");
				break;
			}

			// Remove button
			{
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 48.0f);

				const std::string removeButtonLabel = "Remove##Parameter_" + parameter.first;
				if (ImGui::Button(removeButtonLabel.c_str()))
				{
					if (!IsAlreadyAddedToReserveRemoveParameter(parameter.first))
					{
						_reservedRemoveParameters.emplace_back(parameter.first);
					}
				}
			}
		}

		if (_editParameterName.isEditing)
		{
			if (ImGui::BeginPopupContextWindow("##EditingParameterNamePopup", ImGuiPopupFlags_MouseButtonRight))
			{
				String::InputTextGui("##EditingParameterName", &_editParameterName.newName);

				const bool isMouseClickedOnWindow = (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemClicked());
				if (isMouseClickedOnWindow)
				{
					_editParameterName.Reset();
				}
				else if (ImGui::IsKeyDown(ImGuiKey_Enter))
				{
					OnParameterNameChanged();

					_editParameterName.Reset();
				}

				ImGui::EndPopup();
			}
		}
	}

	void AnimatorController::OnSequenceGui()
	{
		for (const auto& state : _states)
		{
			if (ImGui::Button(state.second->GetName().c_str()))
			{
				_selectedState = state.second;
			}

			if (!state.second->IsBuildInState())
			{
				// Remove button.
				{
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 48.0f);

					const auto removeButtonText = std::string("Remove") + "##" + state.second->GetName() + "_State";
					if (ImGui::Button(removeButtonText.c_str()))
					{
						if (!IsAlreadyAddedToReserveRemoveState(state.first))
						{
							_reservedRemoveStates.emplace_back(state.first);
						}
					}
				}
			}
		}
	}

	void AnimatorController::OnInspectorGui()
	{
		if (!_selectedState)
			return;

		_selectedState->OnGui();
	}

	std::optional<std::string> AnimatorController::OnStatesGui(bool& addStateButtonPressed) const
	{
		for (const auto& state : _states | std::views::values)
		{
			if (!state->IsAllowedSetAsTransitionDestination())
				continue;

			const auto label = state->GetName() + "##AnimatorController";
			if (ImGui::Selectable(label.c_str()))
			{
				addStateButtonPressed = false;
			    return state->GetName();
			}
		}

		return std::nullopt;
	}

	void AnimatorController::Add(const Ref<MeshResource::AnimationClip>& clip)
	{
		if (PathAlreadyExists(clip->GetFilePathToSerialized()))
		{
			DENTY_WARN_CONSOLE_LOG("Animation clip %s already added!", clip->GetName().c_str());

			return;
		}

		_animationClips.try_emplace(clip->GetName(), clip);

		// Create animation state.
		{
			const auto animationState = std::make_shared<AnimationState>(clip->GetName(), shared_from_this());
			animationState->SetAnimationClip(clip);

			_states.try_emplace(animationState->GetName(), animationState);
		}
	}

	void AnimatorController::AddParameter(const AnimatorParameter& parameter)
	{
		_parameters.try_emplace(parameter.name, parameter);
	}

	Ref<MeshResource::AnimationClip> AnimatorController::Find(std::string_view name) const
	{
		const auto& it = _animationClips.find(std::string(name));
		if (it != _animationClips.end())
		{
			return it->second;
		}

		return nullptr;
	}

    Ref<BaseState> AnimatorController::FindStateByName(std::string_view name) const
    {
		for (const auto& state : _states)
		{
		    if (state.second->GetName() != name)
				continue;

			return state.second;
		}

		return nullptr;
    }

    std::string AnimatorController::ObtainProceedName(std::string_view name, const uint32_t maxTrialCount) const
	{
		auto proceedName = String(name);

		for (uint32_t trialCount = 1; trialCount <= maxTrialCount; ++trialCount)
		{
			const bool duplicated = IsParameterNameDuplicated(proceedName.Get());

			if (!duplicated)
				break;

			{
				const uint32_t index = proceedName.FindLastOf("_");
				if (index != UINT32_MAX)
				{
					proceedName = proceedName.ClipUntil(index).value();
				}

				proceedName.Append("_" + std::to_string(trialCount));
			}
		}

		return proceedName.Get();
	}

    std::vector<std::string> AnimatorController::ObtainParameterNames() const
    {
		std::vector<std::string> names = { };

		for (const auto& parameter : _parameters | std::views::values)
		{
		    names.emplace_back(parameter.name);
		}

		return names;
    }

    std::vector<AnimatorValue> AnimatorController::ObtainAnimatorValues() const
    {
		std::vector<AnimatorValue> values = { };

		for (const auto& parameter : _parameters | std::views::values)
		{
		    values.emplace_back(parameter.value);
		}

		return values;
    }

    std::vector<std::string> AnimatorController::ObtainAnimatorValueTypesAsString() const
    {
	    std::vector<std::string> typeNames = { };

		for (const auto& value : ObtainAnimatorValues())
		{
		    typeNames.emplace_back(value.GetValueTypeAsString());
		}

		return typeNames;
    }

    std::vector<std::string> AnimatorController::ObtainAnimatorValueAsString() const
    {
		std::vector<std::string> valueNames = { };

		for (const auto& value : ObtainAnimatorValues())
		{
			const auto str = value.GetValueAsString();

			if (str.has_value())
			{
		        valueNames.emplace_back(str.value());
			}
		}

		return valueNames;
    }

    std::optional<AnimatorValue::Type> AnimatorController::FindParameterType(std::string_view parameterName) const
    {
		for (const auto& parameter : _parameters)
        {
            if (parameter.second.name != parameterName)
                continue;

            return parameter.second.value.valueType;
        }

        return std::nullopt;
    }

    void AnimatorController::LoadAssetTexture()
    {
		DENTY_ASSERT(not _animatorControllerAssetTexture, "Animator controller asset texture already loaded!");

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();
		_animatorControllerAssetTexture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::ANIMATOR_CONTROLLER_DEFAULT_THUMBNAIL_FILE_PATH).value();

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Animator controller asset texture successfully loaded!";
    }

    void AnimatorController::OnPlay([[maybe_unused]] float playSpeed)
    {
    }

    void AnimatorController::OnPause()
    {
    }

    void AnimatorController::OnStop()
    {
    }

    void AnimatorController::OnParameterNameChanged()
	{
		const auto& it = _parameters.find(_editParameterName.oldName);
		
		AnimatorParameter parameter = {};
		parameter.name = _editParameterName.newName.empty() ? ObtainProceedName("NoName") : _editParameterName.newName;
		parameter.value = it->second.value;

		if (it != _parameters.end())
		{
			_parameters.erase(it);
		}

		_parameters.try_emplace(parameter.name, parameter);
	}

    bool AnimatorController::PathAlreadyExists(const FilePath& filePath) const
    {
		for (const auto& clip : _animationClips | std::views::values)
		{
			if (clip->GetFilePathToSerialized() != filePath)
				continue;

			return true;
		}

		return false;
    }

    bool AnimatorController::IsParameterNameDuplicated(std::string_view name) const
    {
		for (const auto& parameterName : _parameters | std::views::keys)
		{
			if (parameterName != name)
				continue;

			return true;
		}

		return false;
    }

    bool AnimatorController::IsAlreadyAddedToReserveRemoveParameter(std::string_view parameterName) const
    {
		for (const auto& reservedParameter : _reservedRemoveParameters)
		{
			if (reservedParameter != parameterName)
				continue;

			return true;
		}

		return false;
    }

	bool AnimatorController::IsAlreadyAddedToReserveRemoveState(std::string_view stateName) const
	{
		for (const auto& reservedState : _reservedRemoveStates)
		{
			if (reservedState != stateName)
				continue;

			return true;
		}

		return false;
	}
}
