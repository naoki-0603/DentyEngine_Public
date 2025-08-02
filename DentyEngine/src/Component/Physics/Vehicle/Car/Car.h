#pragma once

#include "Component/Physics/PhysicalMovement.h"

namespace DentyEngine
{
	//
	// Unused class (2024/01/09).
	//
    class Car final : public PhysicalMovement
	{
		friend class CarController;
	public:
		Car();
		Car(const Car&);
		~Car() override = default;

		void OnUpdate(float deltaTime) override;

		void OnEvent(Event* e) override;

		//
		// Serialize/Deserialize functions.
		//
		void OnSerialized() const override;
		void OnDeserialized() override;

		void OnDestroy() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;
	private:
		void Gravity(float deltaTime) override;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::base_class<PhysicalMovement>(this)
			);

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive)
		{
			archive(
				cereal::base_class<PhysicalMovement>(this)
			);

			OnDeserialized();
		}
	private:
	};
}

CEREAL_REGISTER_TYPE(DentyEngine::Car)