#pragma once

#include "Component/Component.h"
#include "Component/ComponentGenerator.h"

namespace DentyEngine
{
	struct CheckPointSpecification
	{
	public:
		CheckPointSpecification();
		~CheckPointSpecification() = default;

	public:
		int32_t desireEntityPassingCheckPointCount;
		int32_t number;
		int32_t nextCheckPointNumber;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(desireEntityPassingCheckPointCount),
					CEREAL_NVP(number),
					CEREAL_NVP(nextCheckPointNumber)
				);    
			}
		}

		template <class Archive>
		void load(Archive& archive, uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(desireEntityPassingCheckPointCount),
					CEREAL_NVP(number),
					CEREAL_NVP(nextCheckPointNumber)
				);    
			}
		}
	};

	class CheckPoint final : public Component, public std::enable_shared_from_this<CheckPoint>
	{
	public:
		CheckPoint();
		CheckPoint(const CheckPoint& source);
		~CheckPoint() override = default;

		void Awake() override;
		void Start() override;
		void OnGui() override;
		void OnPrefabGui() override;
		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnDestroy() override;

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		//
		// Getter
		//
		[[nodiscard]]
		const CheckPointSpecification& GetSpecification() const { return _specification; }

		DENTY_REGISTER_BUILD_IN_COMPONENT(CheckPoint)
	private:
		void ShowGui();

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Component>(this),
					CEREAL_NVP(_specification)
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
					cereal::base_class<Component>(this),
					CEREAL_NVP(_specification)
				);    
			}

			OnDeserialized();
		}
	private:
		CheckPointSpecification _specification;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::CheckPointSpecification, 1u)
CEREAL_CLASS_VERSION(DentyEngine::CheckPoint, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::CheckPoint)