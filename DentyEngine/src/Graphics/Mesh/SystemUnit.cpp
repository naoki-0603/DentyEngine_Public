#include "stdafx.h"

#include "SystemUnit.h"

namespace DentyEngine
{
    namespace MeshResource
    {
	    SystemUnit::SystemUnit() noexcept
	    {
	    }

	    SystemUnit::SystemUnit(const SystemUnitResource& resource) noexcept :
			_resource(resource)
	    {

	    }

	    void SystemUnit::CalculateScalingMatrix()
	    {
			const float scale = (_resource.scaleFactor == 1u ? 0.01f : 1.0f);

			_resource.scaling = DirectX::XMMatrixScaling(scale, scale, scale);
	    }
    }
}