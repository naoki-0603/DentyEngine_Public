#include "stdafx.h"

#include "PhysicalMovement.h"

namespace DentyEngine
{
	Vector3 PhysicalMovement::GRAVITY = { 0.0f, -9.8f, 0.0f };

	PhysicalMovement::PhysicalMovement() :
		Object(), _velocity(), _acceleration(), _position(), _mass(1.0f), _gravity(true), _resultant()
	{
		_name = "PhysicalMovement";
	}

	PhysicalMovement::PhysicalMovement(const PhysicalMovement& source) :
		Object(source), _velocity(source._velocity), _acceleration(source._acceleration),
		_position(source._position),
		_mass(source._mass), _gravity(source._gravity), _resultant(source._resultant)
	{
	}

	void PhysicalMovement::OnUpdate([[maybe_unused]] float deltaTime)
	{
	}

	void PhysicalMovement::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void PhysicalMovement::OnGui()
	{
		ImGui::SliderFloat("Mass##PhysicalMovement", &_mass, 1.0f, 10000.0f);
		ImGui::Checkbox("Gravity##PhysicalMovement", &_gravity);

		ImGui::Text("Velocity: %f, %f, %f", _velocity.x, _velocity.y, _velocity.z);
		ImGui::Text("Speed: %f", _velocity.Length());
		ImGui::Text("Acceleration: %f, %f, %f", _acceleration.x, _acceleration.y, _acceleration.z);
		ImGui::Text("Resultant: %f, %f, %f", _resultant.x, _resultant.y, _resultant.z);
		ImGui::Text("Position: %f, %f, %f", _position.x, _position.y, _position.z);
	}

	void PhysicalMovement::OnPrefabGui()
	{
		ImGui::SliderFloat("Mass##PhysicalMovement", &_mass, 1.0f, 10000.0f);
	}

	void PhysicalMovement::OnSerialized() const
	{
	}

	void PhysicalMovement::OnDeserialized()
	{
	}

	void PhysicalMovement::OnDestroy()
	{
		Object::OnDestroy();
	}

	Ref<Object> PhysicalMovement::MakeCopy() const
	{
		return std::make_shared<PhysicalMovement>(*std::static_pointer_cast<const PhysicalMovement>(shared_from_this()));
	}

	void PhysicalMovement::Integrate(float deltaTime)
	{
		_acceleration = _resultant / _mass;
		_velocity += _acceleration * deltaTime;
		_position += _velocity * deltaTime;

		_resultant = {};
	}

	void PhysicalMovement::Gravity([[maybe_unused]] float deltaTime)
	{
		if (_gravity)
		{
		    AddForce(GRAVITY);
		}
	}

	void PhysicalMovement::AddForce(const Vector3& force)
	{
		_resultant += force;
	}
};