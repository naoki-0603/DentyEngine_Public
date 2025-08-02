#include "stdafx.h"

#include "System.h"

namespace DentyEngine
{
	namespace System
	{
		LARGE_INTEGER cpuFreq = { };

		void OnLaunch()
		{
			QueryPerformanceFrequency(&cpuFreq);
		}

		LONGLONG GetCpuFreq()
		{
			return cpuFreq.QuadPart;
		}
	}
}