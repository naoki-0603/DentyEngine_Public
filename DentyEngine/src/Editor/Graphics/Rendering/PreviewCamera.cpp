#include "stdafx.h"

#include "PreviewCamera.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
	PreviewCamera::PreviewCamera() :
		Camera(GraphicsManager::GetInstance().GetDevice()), transform()
	{
	}

	PreviewCamera::PreviewCamera(const PreviewCamera& source) :
		Camera(source), transform(source.transform)
	{
	}

	void PreviewCamera::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void PreviewCamera::OnGui()
	{
	}
}
