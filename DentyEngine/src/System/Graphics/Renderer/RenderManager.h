#pragma once

#include "System/Core/IManager.h"
#include "System/Events/EventRender.h"

namespace DentyEngine
{
	class Renderer;
	// Events
    class EventLayerInitialized;
    class EventRendererCreated;
    class EventRendererDestroyed;

    class EventRequiredSceneViewColorData;
    class EventRequiredGameViewColorData;

    class EventSceneRendering;

    class EventBeginGUIRendering;
    class EventEndGUIRendering;

    class EventPresent;

    class EventOnSceneActivated;
    class EventOnSceneDeActivated;

    // Renderers
    class MeshRenderer;
    class SkinnedMeshRenderer;
    class SpriteRenderer;

    class SceneBuffer;
    class ColorBuffer;

    class Skybox;
    class GridRenderer;

    class RenderManager final : public IManager
    {
    public:
        RenderManager();
        ~RenderManager() override = default;

        DELETE_COPY_MOVE_CONSTRUCTORS(RenderManager)

        void Initialize() ;
        void Update() ;
        void OnGui() override;
        void OnEvent(Event* e) ;
        void Finalize() ;

        void AddRenderer(Ref<Renderer> renderer);
        void RemoveRenderer(const Ref<Renderer>& renderer);

        // Call on Renderer::OnGui function.
        void OnRendererOrderTypeChanged(Ref<Renderer> targetRenderer);

        // Call on Scene::OnSkyboxItemReceived function.
        void OnSceneSkyboxChanged(Ref<Skybox> skybox);

        void Reset();

        //
        // Setter
        //
        void SetSkybox(Ref<Skybox> skybox) { _skybox = skybox; }

        //
        // Singleton function
        //
        static RenderManager& GetInstance()
        {
            static RenderManager instance;
            return instance;
        }
    private:
        void Render3D(ID3D11DeviceContext* context) const;
        void Render2D(ID3D11DeviceContext* context) const;

        void ResetShaderAndResources(ID3D11DeviceContext* context);

        //
        // Events
        //
        void OnLayerInitialized(EventLayerInitialized* e);

        void OnSceneActivated(EventOnSceneActivated* e);
        void OnSceneDeActivated(EventOnSceneDeActivated* e);

        void OnRequiredSceneViewColorData(EventRequiredSceneViewColorData* e);
        void OnRequiredGameViewColorData(EventRequiredGameViewColorData* e);

        void OnSceneRender(EventSceneRendering* e);

        void BeginPreRendering(ID3D11DeviceContext* context, EventSceneRendering::ViewType viewType);
        void EndPreRendering(ID3D11DeviceContext* context);

        void BeginRendering(ID3D11DeviceContext* context);
        void EndRendering(ID3D11DeviceContext* context);

        void BeginPostEffect(ID3D11DeviceContext* context);
        void EndPostEffect(ID3D11DeviceContext* context);

        void BeginHUDRendering(ID3D11DeviceContext* context);
        void EndHUDRendering(ID3D11DeviceContext* context);

        void OnBeginGUIRendering(EventBeginGUIRendering* e);
        void OnEndGUIRendering(EventEndGUIRendering* e);

        void OnPresent(EventPresent* e);
    private:
        std::vector<std::vector<Ref<MeshRenderer>>> _meshRenderers;
        std::vector<std::vector<Ref<SkinnedMeshRenderer>>> _skinnedMeshRenderers;
        std::vector<Ref<SpriteRenderer>> _spriteRenderers;

        Ref<SceneBuffer> _sceneViewBuffer;
        Ref<SceneBuffer> _gameViewBuffer;

        Ref<Skybox> _skybox;
        //
        // static variables.
        //
    private:
        static constexpr Vector2 CELL_SIZE = { 0.5f, 0.5f };
        static constexpr float CELL_LINE_THICKNESS = 0.01f;
        static constexpr float CELL_LINE_LENGTH = 100.0f;
    };

    
};