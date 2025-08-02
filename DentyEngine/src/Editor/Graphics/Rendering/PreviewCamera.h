#pragma once

#include "Graphics/Renderer/Camera.h"

#include "Component/Transform.h"

namespace DentyEngine
{
	class PreviewCamera final : public Camera
	{
	public:
		PreviewCamera();
		PreviewCamera(const PreviewCamera&);
		~PreviewCamera() override = default;

		void OnEvent(Event* e) override;
		void OnGui() override;

	public:
		Transform transform;
	};
}
