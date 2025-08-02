#include "stdafx.h"

#include "SphereShape.h"

namespace DentyEngine
{
	SphereShape::SphereShape() :
		IShape(), _radius()
	{
	}

	SphereShape::SphereShape(const SphereShape& source) :
		IShape(source), _radius(source._radius)
	{
		Create(source._radius);
	}

	void SphereShape::Create()
	{
		_radius = DEFAULT_RADIUS;

		_shape = std::make_shared<btSphereShape>(_radius);
	}

	void SphereShape::Create([[maybe_unused]] float radius)
	{
		_shape = std::make_shared<btSphereShape>(_radius);
	}

	void SphereShape::Update(const Vector3& size)
	{
		_shape->setUnscaledRadius(_radius);

		_shape->setLocalScaling(size.ToBtVector3());
	}

	void SphereShape::OnGui()
	{
		ImGui::DragFloat("Radius##SphereShape", &_radius, 0.001f, 0.001f, 2000.0f);
	}

	void SphereShape::OnSerialized() const
	{
	}

	void SphereShape::OnDeserialized()
	{
		Create(_radius);
	}
}
