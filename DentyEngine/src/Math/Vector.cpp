#include "stdafx.h"

#include "Vector.h"

namespace DentyEngine
{
	Vector3 Vector3::WorldToScreen(const Vector3& worldPosition, float viewportX, float viewportY,
		float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ, 
		const Matrix* view, const Matrix* projection)
	{
		return DirectX::XMVector3Project(
			worldPosition.GetXM(), viewportX, viewportY,
			viewportWidth, viewportHeight, viewportMinZ, viewportMaxZ,
			projection->GetXM(), view->GetXM(), Matrix::Identity().GetXM()
		);
	}

	Vector3 Vector3::ScreenToWorld(const Vector3& screenPosition, float viewportX, float viewportY, float viewportWidth,
		float viewportHeight, float viewportMinZ, float viewportMaxZ, const Matrix* view, const Matrix* projection)
	{
		return DirectX::XMVector3Unproject(
			screenPosition.GetXM(), viewportX, viewportY,
			viewportWidth, viewportHeight, viewportMinZ, viewportMaxZ,
			projection->GetXM(), view->GetXM(), Matrix::Identity().GetXM()
		);
	}
}

