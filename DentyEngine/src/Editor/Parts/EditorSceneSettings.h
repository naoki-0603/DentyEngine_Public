#pragma once

#include "IEditor.h"

namespace DentyEngine
{
	class EventRequiredGameViewColorData;
	class EventRequiredSceneViewColorData;

	class EventOnSceneActivated;
	
	class EditorSceneSettings final : public IEditor
	{
	public:
		EditorSceneSettings();
		~EditorSceneSettings() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(EditorSceneSettings)

		void OnUpdate();
		void OnEvent(Event* e) override;
		void OnGui(Ref<Scene> activeScene) override;
	private:
		void GizmosGui(const Ref<Scene>& activeScene);
		void PhysicsSettingsGui(const Ref<Scene>& activeScene);
		void ShadowMapGui(const Ref<Scene>& activeScene);
		void PostEffectsGui(const Ref<Scene>& activeScene);

		//
		// Event functions.
		//
		void OnRequiredGameViewColorData(EventRequiredGameViewColorData* e);
		void OnRequiredSceneViewColorData(EventRequiredSceneViewColorData* e);

		void OnSceneActivated(EventOnSceneActivated* e);

		void FetchSceneSetting();
	private:
		bool _open;
	};

}
