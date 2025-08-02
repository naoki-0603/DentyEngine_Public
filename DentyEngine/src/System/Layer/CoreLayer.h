#pragma once

#include "../Layer/Layer.h"

#include "System/Graphics/Renderer/LightManager.h"

namespace DentyEngine
{
	class CoreLayer final : public Layer
	{
	public:
		CoreLayer(const bool enabled) : Layer("CoreLayer", enabled) {}
		~CoreLayer() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(CoreLayer)

		void OnAttach() override;
		void OnUpdate(const float deltaTime) override;
		void OnLateUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnGui() override;
		void OnDetach() override;
		void OnEvent(Event* e) override;
	};
};