#include "stdafx.h"

#include "CheckPoint.h"

namespace DentyEngine
{
	CheckPointSpecification::CheckPointSpecification() :
		desireEntityPassingCheckPointCount(), number(), nextCheckPointNumber()
	{

	}

	CheckPoint::CheckPoint() :
		Component(), _specification()
	{
		_name = "CheckPoint";
	}

	CheckPoint::CheckPoint(const CheckPoint& source) :
		Component(source), _specification(source._specification)
	{
	}

	void CheckPoint::Awake()
	{
		Component::Awake();
	}

	void CheckPoint::Start()
	{
		Component::Start();
	}

	void CheckPoint::OnGui()
	{
		Component::OnGui();

		ShowGui();
	}

	void CheckPoint::OnPrefabGui()
	{
		ShowGui();
	}

	void CheckPoint::OnSerialized() const
	{
		Component::OnSerialized();
	}

	void CheckPoint::OnDeserialized()
	{
		Component::OnDeserialized();
	}

	void CheckPoint::OnDestroy()
	{
		Component::OnDestroy();
	}

	Ref<Object> CheckPoint::MakeCopy() const
	{
		return std::make_shared<CheckPoint>(*shared_from_this());
	}

	void CheckPoint::ShowGui()
	{
		ImGui::InputInt("desireEntityPassingCheckPointCount##CheckPoint", &_specification.desireEntityPassingCheckPointCount);
		ImGui::InputInt("Number##CheckPoint", &_specification.number);
		ImGui::InputInt("NextCheckPointNumber##CheckPoint", &_specification.nextCheckPointNumber);
	}
}
