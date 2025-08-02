#pragma once

namespace DentyEngine
{
	enum class SpritePivot : uint16_t
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Center,

		Max
	};

	enum class SpriteSortMode : uint16_t
    {
        Deferred, // Enable batching.
        Immediate, // Disable batching.
        //Texture, // Auto.
        //BackToFront, // 
        //FrontToBack,

        Max
    };


	enum class SpriteColor : uint16_t
	{
		White,
	    Red,
		Green,
		Blue,
		Cyan,
		Gray,
		Magenta,
		Black,

		Max
	};

}