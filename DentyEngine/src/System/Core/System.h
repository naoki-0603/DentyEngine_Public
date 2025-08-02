#pragma once

namespace DentyEngine
{
	// Inspire from https://github.com/Siv3D/OpenSiv3D/blob/7f939b3ecdfe6d3f2b4120fb03e49ca98c1ddabe/Siv3D/include/Siv3D/System.hpp#L4

	namespace System
	{
		//
		// functions.
		//
		extern void OnLaunch();

		[[nodiscard]]
		extern LONGLONG GetCpuFreq();

		//
		// variables.
		//
		extern LARGE_INTEGER cpuFreq;
	}
}