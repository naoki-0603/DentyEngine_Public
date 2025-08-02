#pragma once

namespace DentyEngine
{
	class AffineTransform final
	{
	public:
		AffineTransform();
		~AffineTransform() = default;

		[[nodiscard]]
		static Vector2 Scale(float x, float y, float baseX, float baseY, float scaleFactorX, float scaleFactorY);

		[[nodiscard]]
		static Vector2 Rotate(float x, float y, float baseX, float baseY, float cos, float sin);
	};
}