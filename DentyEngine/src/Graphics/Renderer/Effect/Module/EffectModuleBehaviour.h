#pragma once

#include "EffectModule.h"

namespace DentyEngine
{
	enum class EffectBehaviourType : int32_t
	{
		None = -1,
		DriftSpark = 0,
		DriftSmoke = 1,

		Max
	};

	class EffectModuleBehaviour final : public EffectModule
	{
	public:
		EffectModuleBehaviour();
		~EffectModuleBehaviour() override = default;

		void Initialize() override;

		void OnGui() override;

		void OnUpdateCPU(float deltaTime);

		//
		// Getter
		//
		[[nodiscard]]
		EffectModuleType GetType() const override;

		[[nodiscard]]
		EffectBehaviourType GetBehaviourType() const;

		[[nodiscard]]
		uint32_t GetExpectedParticleCountPerFrame() const;
	private:
		void DriftSpark(float deltaTime);
		void DriftSmoke(float deltaTime);

	private:
		std::string _previousBehaviourString;

		EffectBehaviourType _behaviourType;
	private:
		static const std::array<std::string, 2u> EFFECT_BEHAVIOUR_TYPES_STRING;
		static const std::array<uint32_t, 2u> EFFECT_BEHAVIOUR_EXPECTED_PARTICLE_COUNT_PER_FRAME;
	};
}
