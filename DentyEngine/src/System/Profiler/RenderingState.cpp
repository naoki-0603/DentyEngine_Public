#include "stdafx.h"

#include "RenderingState.h"

#include "System/Core/System.h"

#include "MeasuredData.h"

namespace DentyEngine
{
	RenderingState::RenderingState(const RenderingStateSpecification& specification) :
		_specification(specification)
	{
		Begin();
	}

	RenderingState::~RenderingState()
	{
		End();
	}

	void RenderingState::OnGui()
	{
	}

	void RenderingState::Begin()
	{
		QueryPerformanceCounter(&_specification.startPoint);
	}

	void RenderingState::End()
	{
		LARGE_INTEGER endPoint = { };
		QueryPerformanceCounter(&endPoint);

		const auto ms = static_cast<float>(endPoint.QuadPart - _specification.startPoint.QuadPart) / static_cast<float>(System::GetCpuFreq());

		// Add rendering data to measured data.
		{
			const RenderingDataSpecification renderingDataSpecification = { _specification.meshName, ms };

			MeasuredData::GetInstance().AddRenderingState(renderingDataSpecification);
		}
	}
}
