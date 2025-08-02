#include "stdafx.h"

#include "FunctionState.h"

#include "System/Core/System.h"

#include "MeasuredData.h"

namespace DentyEngine
{
	FunctionState::FunctionState(const FunctionStateSpecification& specification) :
		_specification(specification)
	{
		Begin();
	}

	FunctionState::~FunctionState()
	{
		End();
	}

	void FunctionState::OnGui()
	{
	}

	void FunctionState::Begin()
	{
		QueryPerformanceCounter(&_specification.startPoint);
	}

	void FunctionState::End()
	{
		LARGE_INTEGER endPoint = { };
		QueryPerformanceCounter(&endPoint);

		const auto ms = static_cast<float>(endPoint.QuadPart - _specification.startPoint.QuadPart) / static_cast<float>(System::GetCpuFreq());

		// Add function data to measured data.
		{
			const FunctionDataSpecification functionDataSpecification =
			{
				_specification.name,
				ms
			};

			MeasuredData::GetInstance().AddFunctionState(functionDataSpecification);
		}
	}
}
