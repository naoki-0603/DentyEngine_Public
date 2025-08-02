#pragma once

#include "Event.h"

#include "System/Core/ID/ComponentID.h"

namespace DentyEngine
{
	class EditorCamera;
	class Camera;
    class Renderer;
    class Light;

    class Mesh;

    class SceneBuffer;

    class EventTellRenderBulletDebug final : public Event
    {
    public:
        EventTellRenderBulletDebug() : Event(false) {}
        ~EventTellRenderBulletDebug() override = default;

        EVENT_CLASS_TYPE(TellRenderBulletDebug)
    };

    class EventRendererCreated final : public Event
    {
    public:
        EventRendererCreated(Renderer* renderer) : Event(false), _renderer(renderer) { }
        ~EventRendererCreated() override = default;

        Renderer* GetRenderer() const { return _renderer; }

        EVENT_CLASS_TYPE(RendererCreated)
    private:
        Renderer* _renderer;
    };

    class EventRendererDestroyed final : public Event
    {
    public:
        EventRendererDestroyed(Renderer* renderer) : Event(false), _renderer(renderer) { }
        ~EventRendererDestroyed() override = default;

        Renderer* GetRenderer() const { return _renderer;  }

        EVENT_CLASS_TYPE(RendererDestroyed)
    private:
        Renderer* _renderer;
    };

    class EventLightCreated final : public Event
    {
    public:
        EventLightCreated(Light* light) : Event(false), _light(light) { }
        ~EventLightCreated() override = default;

        Light* GetLight() const { return _light; }

        EVENT_CLASS_TYPE(LightCreated)
    private:
        Light* _light;
    };

    class EventLightDestroyed final : public Event
    {
    public:
        EventLightDestroyed(Light* light) : Event(false), _light(light) { }
        ~EventLightDestroyed() override = default;

        Light* GetLight() const { return _light; }

        EVENT_CLASS_TYPE(LightDestroyed)
    private:
        Light* _light;
    };

    class EventOnMeshChanged final : public Event
    {
    public:
        EventOnMeshChanged(ComponentID id, const Ref<Mesh>& mesh) : Event(true), _id(id), _mesh(mesh) {}
        ~EventOnMeshChanged() override = default;

        [[nodiscard]]
        ComponentID GetComponentID() const { return _id; }

        [[nodiscard]]
        Ref<Mesh> GetMesh() const { return _mesh; }

        EVENT_CLASS_TYPE(OnMeshChanged);
    private:
        ComponentID _id;

        Ref<Mesh> _mesh;
    };

    class EventRequiredSceneViewColorData final : public Event
    {
    public:
        EventRequiredSceneViewColorData() : Event(false) {}
        ~EventRequiredSceneViewColorData() override = default;

        void SetSceneBuffer(Ref<SceneBuffer> sceneBuffer) { _sceneBuffer = sceneBuffer; }
        Ref<SceneBuffer> GetSceneBuffer() const { return _sceneBuffer; }

        EVENT_CLASS_TYPE(RequiredSceneViewColorData)
    private:
        Ref<SceneBuffer> _sceneBuffer;
    };

    class EventRequiredGameViewColorData final : public Event
    {
    public:
        EventRequiredGameViewColorData() : Event(false) {}
        ~EventRequiredGameViewColorData() override = default;

        void SetSceneBuffer(Ref<SceneBuffer> sceneBuffer) { _sceneBuffer = sceneBuffer; }
        Ref<SceneBuffer> GetSceneBuffer() const { return _sceneBuffer; }

        EVENT_CLASS_TYPE(RequiredGameViewColorData)
    private:
        Ref<SceneBuffer> _sceneBuffer;
    };

    class EventRequiredEditorCameraData final : public Event
    {
    public:
        EventRequiredEditorCameraData() : Event(false), _view(), _projection() {}
        ~EventRequiredEditorCameraData() override = default;

        void SetView(const Matrix& view) { _view = view; }
        void SetProjection(const Matrix& projection) { _projection = projection; }

        const Matrix& GetView() const { return _view; }
        const Matrix& GetProjection() const { return _projection; }
        const Matrix& CalcViewProjection() const { return _view * _projection; }

    	EVENT_CLASS_TYPE(RequiredEditorCameraData)
    private:
        Matrix _view;
        Matrix _projection;
    };

    class EventOnSceneViewSizeChanged final : public Event
    {
    public:
        EventOnSceneViewSizeChanged(const Vector2& size) : Event(false), _size(size) {}
        ~EventOnSceneViewSizeChanged() override = default;

        const Vector2& GetSize() const { return _size; }

        EVENT_CLASS_TYPE(SceneViewSizeChanged)
    private:
        Vector2 _size;
    };

    class EventOnGameViewSizeChanged final : public Event
    {
    public:
        EventOnGameViewSizeChanged(const Vector2& size) : Event(true), _size(size) {}
        ~EventOnGameViewSizeChanged() override = default;

        const Vector2& GetSize() const { return _size; }

        EVENT_CLASS_TYPE(GameViewSizeChanged)
    private:
        Vector2 _size;
    };

    // Rendering-process
    class EventEditorBeginRenderingProcess final : public Event
    {
    public:
        EventEditorBeginRenderingProcess(ID3D11DeviceContext* context, uint32_t bindSlot) :
            Event(true), _context(context), _bindSlot(bindSlot) {}
        ~EventEditorBeginRenderingProcess() override = default;

        ID3D11DeviceContext* GetContext() const { return _context; }

        EVENT_CLASS_TYPE(EditorBeginRendering)
    private:
        ID3D11DeviceContext* _context;
        uint32_t _bindSlot;

        friend class EditorCamera;
    };

	class EventEditorEndRenderingProcess final : public Event
    {
    public:
        EventEditorEndRenderingProcess(ID3D11DeviceContext* context) :
            Event(true), _context(context) {}
        ~EventEditorEndRenderingProcess() override = default;

        EVENT_CLASS_TYPE(EditorEndRendering)
    private:
        ID3D11DeviceContext* _context;

        friend class EditorCamera;
    };

    class EventGameBeginRenderingProcess final : public Event
    {
    public:
        EventGameBeginRenderingProcess(ID3D11DeviceContext* context) :
            Event(false), _context(context) {}
        ~EventGameBeginRenderingProcess() override = default;

        EVENT_CLASS_TYPE(GameBeginRendering)
    private:
        ID3D11DeviceContext* _context;

        friend class GameCamera;
    };

    class EventGameEndRenderingProcess final : public Event
    {
    public:
        EventGameEndRenderingProcess(ID3D11DeviceContext* context) :
            Event(false), _context(context) {}
        ~EventGameEndRenderingProcess() override = default;

        EVENT_CLASS_TYPE(GameEndRendering)
    private:
        ID3D11DeviceContext* _context;

        friend class GameCamera;
    };

    class EventSceneRendering final : public Event
    {
    public:
        enum class ViewType
        {
	        Scene,
            Game
        };
    public:
        EventSceneRendering(ID3D11DeviceContext* context, 
            ID3D11RenderTargetView* rtv, 
            const D3D11_VIEWPORT& viewport, 
            ViewType type
        ) :
			Event(true), _context(context), _renderTargetView(rtv),
    		_viewport(viewport), _viewType(type) {}
        ~EventSceneRendering() override = default;

        EVENT_CLASS_TYPE(SceneRendering)
    private:
        ID3D11DeviceContext* _context;
        ID3D11RenderTargetView* _renderTargetView;
        D3D11_VIEWPORT _viewport;

        ViewType _viewType;

        friend class RenderManager;
        friend class LightManager;
    };

    class EventBeginGUIRendering final : public Event
    {
    public:
        EventBeginGUIRendering(ID3D11DeviceContext* context) :
            Event(false), _context(context) {}
        ~EventBeginGUIRendering() override = default;

        EVENT_CLASS_TYPE(BeginGUIRendering)
    private:
        ID3D11DeviceContext* _context;

        friend class RenderManager;
    };

    class EventEndGUIRendering final : public Event
    {
    public:
        EventEndGUIRendering(ID3D11DeviceContext* context) :
            Event(false), _context(context) {}
        ~EventEndGUIRendering() override = default;

        EVENT_CLASS_TYPE(EndGUIRendering)
    private:
        ID3D11DeviceContext* _context;

        friend class RenderManager;
    };

    class EventPresent final : public Event
    {
    public:
        EventPresent(IDXGISwapChain1* swapChain) :
			Event(false), _swapChain(swapChain) {}
        ~EventPresent() override = default;

        EVENT_CLASS_TYPE(Present)
    private:
        IDXGISwapChain1* _swapChain;

        friend class RenderManager;
    };
};