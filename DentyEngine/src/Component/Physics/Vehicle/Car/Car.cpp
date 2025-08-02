#include "stdafx.h"

#include "Car.h"

#include "Component/Transform.h"

namespace DentyEngine
{
	Car::Car() :
		PhysicalMovement()
	{
		_name = "Car";
	}

	Car::Car(const Car& source) :
		PhysicalMovement(source)
	{
	}

	void Car::OnUpdate(float deltaTime)
	{
		Gravity(deltaTime);

		Integrate(deltaTime);
	}

	void Car::OnEvent(Event* e)
	{
		PhysicalMovement::OnEvent(e);
	}

	void Car::OnSerialized() const
	{
		PhysicalMovement::OnSerialized();
	}

	void Car::OnDeserialized()
	{
		PhysicalMovement::OnDeserialized();
	}

	void Car::OnDestroy()
	{
		PhysicalMovement::OnDestroy();
	}

	Ref<Object> Car::MakeCopy() const
	{
		return std::make_shared<Car>(*std::static_pointer_cast<const Car>(shared_from_this()));
	}

	void Car::Gravity(float deltaTime)
	{
		PhysicalMovement::Gravity(deltaTime);
	}
}
