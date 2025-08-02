#include "stdafx.h"

#include "Color.h"

namespace DentyEngine
{
	Color::Color() :
		color(WHITE)
	{
	}

	Color::Color(float r, float g, float b, float a) :
		color(r, g, b, a)
	{
	}

	Color::Color(const Vector4& color) :
		color(color)
	{
	}

	bool Color::IsApproxEqual(const Color& target) const noexcept
	{
		return color.IsApproxEqual(target.color);
	}

	Color Color::Lerp(const Color& a, const Color& b, float t)
	{
		Color result = { };
		result.color.x = std::lerp(a.color.x, b.color.x, t);
		result.color.y = std::lerp(a.color.y, b.color.y, t);
		result.color.z = std::lerp(a.color.z, b.color.z, t);
		result.color.w = std::lerp(a.color.w, b.color.w, t);

		return result;
	}
};