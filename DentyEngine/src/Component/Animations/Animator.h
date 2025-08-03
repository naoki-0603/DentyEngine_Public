#pragma once

#include "Component/Behavior.h"

#include "Component/ComponentGenerator.h"

#include "System/Resource/Animations/AnimatorController.h"

namespace DentyEngine
{
	class Animator final : public Behavior, public std::enable_shared_from_this<Animator>
	{
	public:
		Animator();
		Animator(const Animator& source);
		~Animator() override = default;

		//
		// Common functions.
		//
		void Awake() override;
		void Start() override;

		void OnUpdate(float deltaTime) override;
		void OnFixedUpdate() override;

		void OnGui() override;
		void OnPrefabGui() override;
		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Destroy() override;
		void OnDestroy() override;

		void OnGameObjectDeserialized() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		// Register as build component.
		DENTY_REGISTER_BUILD_IN_COMPONENT(Animator)
	public:
		void Play();
		void Pause();
		void Stop();

		//
		// Setter
		// 
		void SetInt(std::string_view name, int32_t value);
		void SetFloat(std::string_view name, float value);
		void SetBool(std::string_view name, bool value);
		void SetTrigger(std::string_view name);

		//
		// Getter
		//
		[[nodiscard]]
		std::optional<int32_t> GetInt(std::string_view name) const;

	    [[nodiscard]]
	    std::optional<float> GetFloat(std::string_view name) const;

	    [[nodiscard]]
		std::optional<bool> GetBool(std::string_view name) const;

	    [[nodiscard]]
		bool HasController() const { return (_controller != nullptr); }
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
				    cereal::base_class<Behavior>(this),
				    CEREAL_NVP(_speed)
			    );    
			}

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
				    cereal::base_class<Behavior>(this),
				    CEREAL_NVP(_speed)
			    );    
			}
			
			OnDeserialized();
		}
	private:
		Ref<AnimatorController> _controller;

		// Play speed, default is 1.0f.
		float _speed;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Animator, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Animator)