#include "stdafx.h"

#include "AffineTransform.h"

namespace DentyEngine
{
	AffineTransform::AffineTransform()
	{
	}

	Vector2 AffineTransform::Scale(float x, float y, float baseX, float baseY, float scaleFactorX, float scaleFactorY)
	{
		// Bring it to the pivot position.
		x -= baseX;
		y -= baseY;

		// Holds values used for calculations
		float tx = x, ty = y;

		// Affine transformation (scaling)
		x = scaleFactorX * tx;
		y = scaleFactorY * ty;

		// return something (that has been moved) to its original position
		x += baseX;
		y += baseY;

		return { x, y };
	}

	Vector2 AffineTransform::Rotate(float x, float y, float baseX, float baseY, float cos, float sin)
	{
		// Bring it to the pivot position.
		x -= baseX;
		y -= baseY;

		// Holds values used for calculations
		float tx = x, ty = y;

		// Affine transformation (rotation)
		x = cos * tx + -sin * ty;
		y = sin * tx + cos * ty;

		// return something (that has been moved) to its original position
		x += baseX;
		y += baseY;

		return { x, y };
	}
}
