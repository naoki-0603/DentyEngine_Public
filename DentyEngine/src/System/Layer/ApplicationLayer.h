#pragma once

#include "../Layer/Layer.h"

namespace DentyEngine
{
	class EventWindowClosed;
	class EventWindowResized;

	class EventOnSceneActivated;
	class EventOnSceneDeActivated;

	class EventOnScenePlayButtonPressed;
	class EventOnSceneResumeButtonPressed;
	class EventOnScenePauseButtonPressed;

	class EventOnCreatedGameObjectInHierarchy;

	class Window;
	class Scene;

	class ApplicationLayer final : public Layer
	{
	public:
		enum class State
		{
			Edit,
			Play,
			Pause,
			Step,

			Max
		};
	public:
		ApplicationLayer(const bool enabled) : Layer("ApplicationLayer", enabled),
			_activeScene(), _currentPlayingSceneName(), _open( true ) {}
		~ApplicationLayer() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ApplicationLayer)

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(const float deltaTime) override;
		void OnLateUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnGui() override;
		void OnEvent(Event* e) override;

	public:
		[[nodiscard]]
		static State GetCurrentState() { return _state; }

		[[nodiscard]]
	    static bool IsPlay() { return (_state == State::Play); }

		[[nodiscard]]
		static bool IsEdit() { return (_state == State::Edit); }

		[[nodiscard]]
		static bool IsPause() { return (_state == State::Pause); }

		[[nodiscard]]
		static bool IsStep() { return (_state == State::Step); }
	private:
		// Events
		void OnSceneActivated(EventOnSceneActivated* e);
		void OnSceneDeactivated(EventOnSceneDeActivated* e);

		void OnScenePlayButtonPressed(EventOnScenePlayButtonPressed* e);
		void OnSceneResumeButtonPressed(EventOnSceneResumeButtonPressed* e);
		void OnScenePauseButtonPressed(EventOnScenePauseButtonPressed* e);

		//void OnSceneStepButtonPressed(EventOnSceneStepButtonPressed* e);
	private:
		Ref<Scene> _activeScene;

		std::string _currentPlayingSceneName;

		bool _open;
	private:
		static State _state;
	};
};