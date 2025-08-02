#pragma once

namespace DentyEngine
{
	enum class GravityDirection : int32_t
	{
		None = -1,
		Down = 0,
		Up,
		Left,
		Right,

		Max
	};

	enum class SteerDirection : int32_t
	{
		None = -1,
		Left = 0,
		Right,

		Max
	};

	struct DynamicCharacterStatus
	{
		DynamicCharacterStatus();

		void ResetReverseState();
	public:
		GravityDirection gravityDirection;
		SteerDirection steerDirection;

		// Unit is radian.
		Vector3 eulerAngles;

		std::string km;

		float reverseFrameCounter;

		bool handling;
		bool reverse;
		bool onGround;

		// Constants
	public:
		constexpr static float TO_CHANGE_REVERSE_FRAME_TIME = 20.0f;
	};
}