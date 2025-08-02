#pragma once

#include "Behaviour.h"

namespace DentyEngine
{
	class MonoBehaviour : public Behaviour
	{
	public:
		MonoBehaviour();
		MonoBehaviour(const MonoBehaviour& source);
		virtual ~MonoBehaviour() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(float deltaTime) override;

		virtual void OnFixedUpdate() override;
		virtual void OnLateUpdate(float deltaTime);
		virtual void OnEnabled();
		virtual void OnDisable();
		
		virtual void OnGui() override;
		virtual void OnPrefabGui() override;
		virtual void OnSerialized() const override;
		virtual void OnDeserialized() override;
		virtual void OnDestroy() override;
		virtual void OnGameObjectDeserialized() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
				    cereal::base_class<Behaviour>(this)
			    );    
			}
		}

		template <class Archive>
		void load(Archive& archive, uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
				    cereal::base_class<Behaviour>(this)
			    );    
			}
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::MonoBehaviour, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::MonoBehaviour)