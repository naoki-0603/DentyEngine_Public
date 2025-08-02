#include "stdafx.h"

#include "BoxShape.h"

namespace DentyEngine
{
	BoxShape::BoxShape() : IShape(),
		_shape(), _size()
	{
		
	}

	BoxShape::BoxShape(const BoxShape& source) :
		_size(source._size)
	{
		Create();
	}

	BoxShape::~BoxShape()
	{
	}

	void BoxShape::Create()
	{
		_shape = std::make_shared<btBoxShape>(DEFAULT_HALF_EXTENTS.ToBtVector3());
	}

	void BoxShape::Create(const Vector3& size)
	{
		_size = size;

		_shape = std::make_shared<btBoxShape>(_size.ToBtVector3());
	}

	void BoxShape::Update(const Vector3& size)
	{
		_shape->setLocalScaling(size.ToBtVector3());
	}

	void BoxShape::OnGui()
	{
		ImGui::DragFloat3("Size##BoxShape", &_size.x, 0.01f);
	}

	void BoxShape::OnSerialized() const
	{
	}

	void BoxShape::OnDeserialized()
	{
		Create();
	}
}
