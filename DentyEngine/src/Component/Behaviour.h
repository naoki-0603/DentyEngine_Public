#pragma once

#include "Component/Component.h"

namespace DentyEngine
{
	class Behaviour : public Component
	{
	public:
		Behaviour();
		Behaviour(const Behaviour& source);
		virtual ~Behaviour() override = default;

		virtual void Awake() override;
		virtual void Start() override;
		virtual void OnUpdate(float deltaTime) = 0;

		// Call when scene state is play.
		virtual void OnFixedUpdate() {}
		virtual void OnGui() override;
		virtual void OnPrefabGui() override;

		virtual void OnEvent(Event* e) override;
		virtual void OnSerialized() const override;
		virtual void OnDeserialized() override;
		virtual void OnDestroy() override;
		virtual void OnGameObjectDeserialized() override;

		virtual void OnGizmoRender() override {}

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] uint32_t version) const
		{
			archive(
				cereal::base_class<Component>(this)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] uint32_t version)
		{
			archive(
				cereal::base_class<Component>(this)
			);
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Behaviour, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Behaviour)