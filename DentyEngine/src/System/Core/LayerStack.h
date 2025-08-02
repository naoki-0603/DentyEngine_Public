#pragma once

namespace DentyEngine
{
	class Event;
	class EventEditorEndRenderingProcess;

	class Layer;
	class LayerStack final
	{
	public:
		LayerStack() = default;
		~LayerStack() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(LayerStack)

		void OnAttach();
		void OnUpdate(const float deltaTime);
		void OnLateUpdate(float deltaTime);
		void OnFixedUpdate();
		void OnGui();
		void OnEvent(Event* e);
		void OnDetach();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		const std::vector<Layer*>::iterator Begin();
		const std::vector<Layer*>::iterator End();
	private:
		std::vector<Layer*> _layers;
	};
};