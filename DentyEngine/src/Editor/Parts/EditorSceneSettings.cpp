#include "stdafx.h"

#include "EditorSceneSettings.h"

#include "Graphics/Buffer/SceneBuffer.h"

#include "Graphics/PostEffect/ShadowMap/VarianceShadowMap.h"
#include "Graphics/Renderer/GridRenderer.h"
#include "Graphics/Renderer/LightCamera.h"

#include "System/Core/Setting/GlobalSettingsManager.h"
#include "System/Editor/EditorSceneManager.h"

#include "System/Events/EventRender.h"
#include "System/Events/EventSystem.h"

#include "System/Layer/ApplicationLayer.h"
#include "System/Physics/PhysicsEngine.h"

#include "System/Scene/Scene.h"

namespace DentyEngine
{
	EditorSceneSettings::EditorSceneSettings() :
		IEditor(), _open(true)
	{
		
	}

	void EditorSceneSettings::OnUpdate()
	{
	}

	void EditorSceneSettings::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventRequiredSceneViewColorData>(BIND_EVENT_FUNCTION(OnRequiredSceneViewColorData));
		dispatcher.Dispatch<EventOnSceneActivated>(BIND_EVENT_FUNCTION(OnSceneActivated));
	}

	void EditorSceneSettings::OnGui(Ref<Scene> activeScene)
	{
		if (ImGui::Begin("EditorSceneSettings##Editor", &_open))
		{
			if (ApplicationLayer::IsPlay())
            {
                ImGui::End();

                return;
            }

			if (ImGui::CollapsingHeader("Gizmos##EditorSceneSettings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				GizmosGui(activeScene);
			}

			if (ImGui::CollapsingHeader("PhysicsSettings##EditorSceneSettings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				PhysicsSettingsGui(activeScene);
			}

			if (ImGui::CollapsingHeader("ShadowMap##EditorSceneSettings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ShadowMapGui(activeScene);
			}

			if (ImGui::CollapsingHeader("PostEffects##EditorSceneSettings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				PostEffectsGui(activeScene);
			}

			if (ImGui::CollapsingHeader("Info##EditorSceneSettings", ImGuiTreeNodeFlags_DefaultOpen))
			{
				activeScene->OnGuiInfo();
			}
		}

		ImGui::End();

	}

	void EditorSceneSettings::GizmosGui(const Ref<Scene>& activeScene)
	{
		SceneSetting& sceneSetting = activeScene->GetSceneSetting();
		SceneGizmos& sceneGizmos = sceneSetting.gizmos;
		
		if (ImGui::Checkbox("RenderGrid##EditorSceneSettings", &sceneGizmos.renderGrid))
		{
			if (sceneGizmos.renderGrid)
				GridRenderer::Enable();
			else
				GridRenderer::Disable();
		}
	}

	void EditorSceneSettings::PhysicsSettingsGui(const Ref<Scene>& activeScene)
	{
		SceneSetting& sceneSetting = activeScene->GetSceneSetting();
		ScenePhysics& scenePhysics = sceneSetting.physics;

		if (ImGui::TreeNodeEx("Parameters##PhysicsSettingsGui", ImGuiTreeNodeFlags_DefaultOpen))
		{
		    ImGui::SliderFloat3("Gravity##PhysicsParameters", &sceneSetting.physics.gravity.x, -20.0f, 20.0f);

			ImGui::TreePop();
		}

		if (ImGui::Checkbox("VisibleDebugLines##PhysicsSettingsGui", &scenePhysics.debugDrawerEnable))
		{
			PhysicsEngine::GetInstance()->VisibleDebugLines(scenePhysics.debugDrawerEnable);
		}
	}

    void EditorSceneSettings::ShadowMapGui([[maybe_unused]] const Ref<Scene>& activeScene)
	{
		VarianceShadowMap::OnGui();
		//CascadeShadowMap::OnGui();
	}

	void EditorSceneSettings::PostEffectsGui([[maybe_unused]] const Ref<Scene>& activeScene)
	{
	}

	void EditorSceneSettings::OnRequiredGameViewColorData(EventRequiredGameViewColorData* e)
    {
		Ref<SceneBuffer> sceneBuffer = e->GetSceneBuffer();
    }

    void EditorSceneSettings::OnRequiredSceneViewColorData(EventRequiredSceneViewColorData* e)
    {
		Ref<SceneBuffer> sceneBuffer = e->GetSceneBuffer();

		// Bloom
        {
            
        }

    }

    void EditorSceneSettings::OnSceneActivated([[maybe_unused]] EventOnSceneActivated* e)
    {
		FetchSceneSetting();
    }

    void EditorSceneSettings::FetchSceneSetting()
    {
		const Ref<Scene> activeScene = EditorSceneManager::GetActiveScene();
		DENTY_CHECK_NULL(activeScene);

		// Shadow
		{
			const SceneShadow& shadow = activeScene->GetSceneSetting().shadow;

			// Variance
			{
				LightCameraParameters& parameters = VarianceShadowMap::GetParameters().lightCamera->GetParameters();
				parameters.orthographic.viewSize = (shadow.variance.viewSize == 0.0f ? 256.0f : shadow.variance.viewSize);
				parameters.orthographic.nearZ = (shadow.variance.near == 0.0f ? 0.1f : shadow.variance.near);
				parameters.orthographic.farZ = (shadow.variance.far == 0.0f ? 5000.0f : shadow.variance.far);
				parameters.orthographic.aspectRatio = 1.0f;
				parameters.targetPosition = (shadow.variance.targetPosition);
			}

			// Cascade
			{
				
			}
		}
    }
}
