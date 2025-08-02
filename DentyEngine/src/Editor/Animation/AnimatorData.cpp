#include "stdafx.h"

#include "AnimatorData.h"

namespace DentyEngine
{
	AnimatorValue::AnimatorValue() :
		valueType(), valueInt(), valueFloat(), valueBool(), valueTrigger()
	{
	}

    std::optional<std::string> AnimatorValue::GetValueAsString() const
    {
		switch (valueType)
		{
		case Type::Int:
			return std::to_string(valueInt);
		case Type::Float:
			return std::to_string(valueFloat);
		case Type::Bool:
			return (valueBool ? "true" : "false");
		case Type::Trigger:
			return std::nullopt;
		}

		return std::nullopt;
    }

    AnimatorParameter::AnimatorParameter() :
		name(), value()
	{

	}
}
