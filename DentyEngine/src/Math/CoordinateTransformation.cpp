#include "stdafx.h"

#include "CoordinateTransformation.h"

namespace DentyEngine
{
	CoordinateTransformation::CoordinateTransformation()
	{
	}

	void CoordinateTransformation::ScreenToNdc(float& leftTopX, float& leftTopY, float& rightTopX, float& rightTopY,
		float& leftBottomX, float& leftBottomY, float& rightBottomX, float& rightBottomY, float viewportWidth,
		float viewportHeight)
	{
		/*
			X axis
			Example: x0 = 960;
			x0 = 2.0f * x0 -> 1920
			1920 / 1280 = 1.5
			1.5 - 1.0 = 0.5; -> Coordinates in NDC coordinates can be calculated.
			Y axis
			Example: y0 = 270;
			y0 = -2.0f * y0 / 1080 -> -0.5f
			y0 = -0.5f + 1.0f = 0.5f
			The coordinate transformation is done.
		*/
		leftTopX = 2.0f * leftTopX / viewportWidth - 1.0f;
		leftTopY = 1.0f + (-2.0f * leftTopY / viewportHeight);
		rightTopX = 2.0f * rightTopX / viewportWidth - 1.0f;
		rightTopY = 1.0f + (-2.0f * rightTopY / viewportHeight);
		leftBottomX = 2.0f * leftBottomX / viewportWidth - 1.0f;
		leftBottomY = 1.0f + (-2.0f * leftBottomY / viewportHeight);
		rightBottomX = 2.0f * rightBottomX / viewportWidth - 1.0f;
		rightBottomY = 1.0f + (-2.0f * rightBottomY / viewportHeight);
	}
}
