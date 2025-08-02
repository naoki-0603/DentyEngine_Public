#pragma once

#include "IManager.h"
#include "LayerStack.h"

namespace DentyEngine
{
	class EditorLayer;

	class LayerStack;
	class LayerManager final : public IManager
	{
	public:
		LayerManager() : IManager(), _layerStack(std::make_unique<LayerStack>()) {}
		~LayerManager() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(LayerManager)

		void OnLaunch();
		void OnTerminate();
		void Gui();

		void Update();
		void LateUpdate();
		void FixedUpdate();
		void OnEvent(Event* e) ;

	private:
		void Initialize();
		void Finalize();

		void OnGui() override;
	private:
		Scope<LayerStack> _layerStack;
	};
};