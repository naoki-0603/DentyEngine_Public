#include "stdafx.h"

#include "MeasuredData.h"

namespace DentyEngine
{
	void MeasuredData::AddFrameState(const FrameDataSpecification& specification)
	{
		_frameStates.emplace(specification.name, specification);
	}

	void MeasuredData::AddFunctionState(const FunctionDataSpecification& specification)
	{
		_functionStates.emplace(specification.name, specification);
	}

	void MeasuredData::AddRenderingState(const RenderingDataSpecification& specification)
	{
		_renderingState.emplace(specification.name, specification);
	}

	void MeasuredData::Clear()
	{
		_frameStates.clear();
		_functionStates.clear();
		_renderingState.clear();
	}
}
