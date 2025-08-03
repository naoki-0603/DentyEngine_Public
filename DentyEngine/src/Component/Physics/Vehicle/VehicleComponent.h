#pragma once

#include "Component/ComponentGenerator.h"
#include "Component/PhysicsBehavior.h"

#include "Physics/Vehicle/Vehicle.h"
#include "Physics/Vehicle/SimplifiedCar.h"

namespace DentyEngine
{
	class EventOnScenePlayButtonPressed;
	class VehicleComponent final : public PhysicsBehavior, public std::enable_shared_from_this<VehicleComponent>
	{
	public:
		VehicleComponent();
		VehicleComponent(const VehicleComponent& source);
		~VehicleComponent() override = default;

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
		void OnRuntimeSceneChanged();

		void OnGameObjectTagChanged();

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		[[nodiscard]]
		SimplifiedCar& GetCar() { return _car; }

		[[nodiscard]]
		const SimplifiedCar& GetCar() const { return _car; }

		DENTY_REGISTER_BUILD_IN_COMPONENT(VehicleComponent)
	protected:
		virtual void AddWorld();
		virtual void RemoveWorld();

		virtual void Recreate();
	private:
		void UpdateRigidBodyTransform();

		// Called on OnFixedUpdate function.
		void UpdateTransform();

		void OnScenePlayButtonPressed(EventOnScenePlayButtonPressed* e);

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<PhysicsBehavior>(this)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<PhysicsBehavior>(this)
			);

			OnDeserialized();
		}

	private:
		// New
		SimplifiedCar _car;

		// Old
		[[maybe_unused]] Vehicle _vehicle;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::VehicleComponent, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::VehicleComponent)