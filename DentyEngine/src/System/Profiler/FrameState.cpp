#include "stdafx.h"

#include "FrameState.h"

#include "System/Core/System.h"

#include "MeasuredData.h"

namespace DentyEngine
{
	FrameState::FrameState(const FrameStateSpecification& specification) :
		_specification(specification)
	{
		Begin();
	}

	FrameState::~FrameState()
	{
		End();
	}

	void FrameState::OnGui()
	{
	}

	void FrameState::Begin()
	{
		QueryPerformanceCounter(&_specification.startPoint);
	}

	void FrameState::End()
	{
		LARGE_INTEGER endPoint = { };
		QueryPerformanceCounter(&endPoint);

		const auto ms = static_cast<float>(endPoint.QuadPart - _specification.startPoint.QuadPart) / static_cast<float>(System::GetCpuFreq());
		const auto fps = 1.0f / ms;

		// Add frame data to measured data.
		{
			const FrameDataSpecification frameDataSpecification =
			{
				_specification.name, ms, fps
			};

			MeasuredData::GetInstance().AddFrameState(frameDataSpecification);
		}
	}
}
