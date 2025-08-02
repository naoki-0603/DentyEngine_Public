#pragma once

namespace DentyEngine
{
	class CoordinateTransformation final
	{
	public:
		CoordinateTransformation();
		~CoordinateTransformation() = default;

		static void ScreenToNdc(float& leftTopX, float& leftTopY,
			float& rightTopX, float& rightTopY,
			float& leftBottomX, float& leftBottomY,
			float& rightBottomX, float& rightBottomY,
			float viewportWidth, float viewportHeight);
	};
}