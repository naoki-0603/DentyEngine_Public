#include "stdafx.h"

#include "CapsuleShape.h"

namespace DentyEngine
{
	CapsuleShape::CapsuleShape() :
		IShape(), _shape(), _radius(), _height()
	{
	}

	CapsuleShape::CapsuleShape(const CapsuleShape& source) :
		_radius(source._radius), _height(source._height)
	{
		Create(_radius, _height);
	}

	void CapsuleShape::Create()
	{
		_radius = DEFAULT_RADIUS;
		_height = DEFAULT_HEIGHT;

		_shape = std::make_shared<btCapsuleShape>(_radius, _height);
	}

	void CapsuleShape::Create(float radius, float height)
	{
		_radius = radius;
		_height = height;

		_shape = std::make_shared<btCapsuleShape>(_radius, _height);
	}

	void CapsuleShape::Update(const Vector3& size)
	{
		_shape->setLocalScaling(size.ToBtVector3());
	}

	void CapsuleShape::OnGui()
	{
		ImGui::DragFloat("Height##CapsuleShape", &_height, 0.001f, 0.001f);
	}

	void CapsuleShape::OnSerialized() const
	{
	}

	void CapsuleShape::OnDeserialized()
	{
		Create(_radius, _height);
	}
}
