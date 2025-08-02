#pragma once

#include "System/Core/ID/AnimatorControllerID.h"

#include "System/Resource/Animations/AnimationClip.h"

#include "Editor/Animation/AnimatorData.h"

#include "Editor/Animation/States/BaseState.h"

namespace DentyEngine
{
	class Animator;

	class AnimatorController final : public Object, public Asset, public std::enable_shared_from_this<AnimatorController>
	{
		friend class Animator;

		using AnimationClips = std::unordered_map<std::string, Ref<MeshResource::AnimationClip>>;
		using AnimatorParameters = std::unordered_map<std::string, AnimatorParameter>;
		using States = std::unordered_map<std::string, Ref<BaseState>>;
	public:
		struct EditParameterName
		{
			EditParameterName() : oldName(), newName(), isEditing() {}

			inline void Reset()
			{
				oldName = {};
				newName = {};
				isEditing = false;
			}

			std::string oldName;
			std::string newName;

			bool isEditing;
		};
	public:
		AnimatorController();
		~AnimatorController() override = default;

		void Create(std::string_view name, const std::filesystem::path& filePath);

		// Call on EditorAnimatorWindow::OnGui function.
		void Update();

		void OnEvent(Event* e) override;

		void Destroy() override;
		void OnDestroy() override;

		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnLoaded();

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile) override;

		void Clone(const Ref<Asset>& asset) override;
		void OnReload() override;

		//
		// Gui functions.
		//
		void OnParameterGui();
		void OnSequenceGui();
		void OnInspectorGui();

		// Called on Transition::OnGui function.
		// Return selected state name.
		// If pressed set addStateButtonPressed false.
		std::optional<std::string> OnStatesGui(bool& addStateButtonPressed) const;

		//
		// Add functions.
		//
		void Add(const Ref<MeshResource::AnimationClip>& clip);
		void AddParameter(const AnimatorParameter& parameter);

		//
		// Find functions.
		//
		[[nodiscard]]
		Ref<MeshResource::AnimationClip> Find(std::string_view name) const;

		[[nodiscard]]
		Ref<BaseState> FindStateByName(std::string_view name) const;

		//
		// Obtain proceed name.
		//
		[[nodiscard]]
		std::string ObtainProceedName(std::string_view name, const uint32_t maxTrialCount = 100) const;

		[[nodiscard]]
		std::vector<std::string> ObtainParameterNames() const;

		[[nodiscard]]
		std::vector<AnimatorValue> ObtainAnimatorValues() const;

		[[nodiscard]]
		std::vector<std::string> ObtainAnimatorValueTypesAsString() const;

		[[nodiscard]]
		std::vector<std::string> ObtainAnimatorValueAsString() const;

		[[nodiscard]]
		std::optional<AnimatorValue::Type> FindParameterType(std::string_view parameterName) const;

		//
		// Setter
		//
		void SetAnimator(const Ref<Animator>& animator) { _animator = animator; }

		//
		// Getter
		//
		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::Controller; }

		[[nodiscard]]
		Ref<Animator> GetAnimator() const { return _animator; }

		[[nodiscard]]
		const AnimatorParameters& GetParameters() const { return _parameters; }
	
		[[nodiscard]]
		AnimatorControllerID GetAnimatorControllerID() const { return _animatorControllerID; }

		[[nodiscard]]
		bool HasAnimator() const { return (_animator != nullptr); }

		[[nodiscard]]
		bool HasParameters() const { return (!_parameters.empty()); }

		//
		// Static functions.
		//
		static void LoadAssetTexture();

		[[nodiscard]]
		static Ref<Texture2D> GetAssetTexture() { return _animatorControllerAssetTexture; }
	public:
		//
		// Static variables.
		//
		static const String ADD_FILENAME_AT_END;
	private:
		void OnPlay(float playSpeed);
		void OnPause();
		void OnStop();

		void OnParameterNameChanged();

		// Use when add path to animation clip paths.
		[[nodiscard]]
		bool PathAlreadyExists(const FilePath& filePath) const;

		[[nodiscard]]
		bool IsParameterNameDuplicated(std::string_view name) const;

		[[nodiscard]]
		bool IsAlreadyAddedToReserveRemoveParameter(std::string_view parameterName) const;

		[[nodiscard]]
		bool IsAlreadyAddedToReserveRemoveState(std::string_view stateName) const;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_parameters),
					CEREAL_NVP(_states),
					CEREAL_NVP(_animatorControllerID)
			    );
			}

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_parameters),
					CEREAL_NVP(_states),
					CEREAL_NVP(_animatorControllerID)
			    );
			}

			OnDeserialized();
		}
	private:
		// Parent animator.
		Ref<Animator> _animator;

		// Doesn't serialize.
		AnimationClips _animationClips;

		AnimatorParameters _parameters;
		std::vector<std::string> _reservedRemoveParameters;

		States _states;
		std::vector<std::string> _reservedRemoveStates;

		AnimatorControllerID _animatorControllerID;

		// Use when edit parameter name.
		EditParameterName _editParameterName;

		Ref<BaseState> _selectedState;
	private:
		static Ref<Texture2D> _animatorControllerAssetTexture;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::AnimatorController, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::AnimatorController)