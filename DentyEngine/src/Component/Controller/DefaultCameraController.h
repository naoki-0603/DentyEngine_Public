#pragma once

#include "Component/Behaviour.h"
#include "Component/ComponentGenerator.h"

#include "System/Core/ID/EntityID.h"

namespace DentyEngine
{
	class VehicleComponent;
	class GameCamera;

	class DefaultCameraController final : public Behaviour, public std::enable_shared_from_this<DefaultCameraController>
	{
	public:
		DefaultCameraController();
		DefaultCameraController(const DefaultCameraController& source);
		~DefaultCameraController() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnGui() override;
		void OnPrefabGui() override;

		void OnEvent(Event* e) override;
		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnDestroy() override;
		void OnGameObjectDeserialized() override;

		void OnSceneActivated();
		void OnSceneDeActivated();

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		DENTY_REGISTER_BUILD_IN_COMPONENT(DefaultCameraController)
	private:
		void RestoreTargetGameObject();

		void UpdateCameraTransform();
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
				    cereal::base_class<Behaviour>(this),
				    CEREAL_NVP(_targetID),
				    CEREAL_NVP(_offset),
				    CEREAL_NVP(_distance),
				    CEREAL_NVP(_height)
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
				    cereal::base_class<Behaviour>(this),
				    CEREAL_NVP(_targetID),
				    CEREAL_NVP(_offset),
				    CEREAL_NVP(_distance),
				    CEREAL_NVP(_height)
			    );    
			}

			OnDeserialized();
		}
	private:
		Ref<GameObject> _targetGameObject;
		Ref<GameCamera> _gameCamera;
		Ref<VehicleComponent> _vehicleComponent;

		EntityID _targetID;

		Vector3 _offset;

		float _height;
		float _previousHeight;
		float _distance;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::DefaultCameraController, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::DefaultCameraController)