#pragma once

namespace DentyEngine
{
	enum class KeyCode : uint16_t
	{
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,

		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,

		U,
		V,
		W,
		X,
		Y,
		Z,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		ArrowLeft,
		ArrowUp,
		ArrowDown,
		ArrowRight,

		Tab,
		Space,
		LCtrl,
		RCtrl,
		LShift,
		RShift,
		Escape,
		BackSpace,
		Return,
		Alt,

		Max
	};

	enum class PadKeyCode : uint16_t
	{
		// è\éöÉLÅ[
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,

		Start,
		Back,

		LeftThumb,
		RightThumb,

		LeftShoulder,
		RightShoulder,

		A,
		B,
		X,
		Y,

		Max
	};

	enum class MouseButton : uint16_t
	{
		LeftButton,
		MiddleButton,
		RightButton,

		Max,
	};
}

DEFINE_ENUM_CLASS_BIT_OPERATORS(DentyEngine::KeyCode)
DEFINE_ENUM_CLASS_BIT_OPERATORS(DentyEngine::PadKeyCode)