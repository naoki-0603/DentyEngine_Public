#pragma once

// https://easings.net/ja
namespace DentyEngine
{
	class Easing final
	{
	public:
		Easing();
		~Easing() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(Easing)

		//
		// Static functions.
		//
		static float EaseOutCirc(float x);
	};
}