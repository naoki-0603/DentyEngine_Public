#include "stdafx.h"

#include "EffectModuleBehaviour.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	const std::array<std::string, 2u> EffectModuleBehaviour::EFFECT_BEHAVIOUR_TYPES_STRING =
	{
		"DriftSpark", "DriftSmoke"
	};

	const std::array<uint32_t, 2u> EffectModuleBehaviour::EFFECT_BEHAVIOUR_EXPECTED_PARTICLE_COUNT_PER_FRAME =
	{
		250u, 250u
	};

	EffectModuleBehaviour::EffectModuleBehaviour() :
		EffectModule(), _previousBehaviourString(), _behaviourType(EffectBehaviourType::DriftSpark)
	{
	}

	void EffectModuleBehaviour::Initialize()
	{

	}

	void EffectModuleBehaviour::OnGui()
	{
		if (GuiUtils::ComboBoxGui(
				"Behaviour##EffectModuleBehaviour",
				EFFECT_BEHAVIOUR_TYPES_STRING.data(),
				EFFECT_BEHAVIOUR_TYPES_STRING.size(), _previousBehaviourString))
		{
			// Behaviour changed.
		}
	}

	void EffectModuleBehaviour::OnUpdateCPU(float deltaTime)
	{
		switch (_behaviourType)
		{
		case EffectBehaviourType::DriftSpark:
			DriftSpark(deltaTime);
			break;
		case EffectBehaviourType::DriftSmoke:
			DriftSmoke(deltaTime);
			break;
		default: [[unlikely]]
			break;
		}
	}


	EffectModuleType EffectModuleBehaviour::GetType() const
	{
		return EffectModuleType::Behaviour;
	}

	EffectBehaviourType EffectModuleBehaviour::GetBehaviourType() const
	{
		return _behaviourType;
	}

	uint32_t EffectModuleBehaviour::GetExpectedParticleCountPerFrame() const
	{
		return EFFECT_BEHAVIOUR_EXPECTED_PARTICLE_COUNT_PER_FRAME.at(
			static_cast<std::array<unsigned, 2>::size_type>(_behaviourType)
		);
	}

	void EffectModuleBehaviour::DriftSpark(float deltaTime)
	{

	}

	void EffectModuleBehaviour::DriftSmoke(float deltaTime)
	{

	}
}
