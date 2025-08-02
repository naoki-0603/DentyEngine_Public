#pragma once

#include "Graphics/Buffer/FrameBuffer.h"
#include "Graphics/Renderer/ShadowCamera.h"

namespace DentyEngine
{
	class Texture2D;
	class EventOnCreatedGameObject;

	enum class ShadowMapType
	{
		Soft,

		Max
	};

	// Shadow maps are treated specially,
	// so they do not inherit from the PostEffect class.
	class ShadowMap final
	{
	public:
		ShadowMap();
		~ShadowMap() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ShadowMap)

		// Call on EditorSceneSettings constructor.
		void Create();

		// Call on EditorSceneSettings::OnUpdate function.
		void OnUpdate();

		// Call on EditorSceneSettings::OnEvent function.
		void OnEvent(Event* e);

		// Call on EditorSceneSettings::OnGui function.
		void OnGui();

		// Call on RenderManager::BeginPreRendering function.
		void Begin(ID3D11DeviceContext* context);

		// Call on RenderManager::EndPreRendering function.
		void End(ID3D11DeviceContext* context);

		// Call on RenderManager::OnSceneRender function.
		void BindCamera(ID3D11DeviceContext* context);

		//
		// Setter
		//

		// Call on GameObject::OnSceneActivated and OnSceneDeActivated functions.
		void SetDirectionalLight(Ref<DirectionalLight> directionalLight);

		//
		// Getter
		//
		[[nodiscard]]
		ID3D11ShaderResourceView* const* GetShadowMap() const;
	public:
		static Scope<ShadowMap>& GetInstance();
		
	public:
		static constexpr uint32_t SRV_BIND_SLOT = 20u;
	private:
		Scope<Texture2D> _texture2d;

		Scope<ShadowCamera> _shadowCamera;
		Scope<FrameBuffer> _frameBuffer;

		ShadowMapType _type;

		Vector2 _size;
	private:
		static Scope<ShadowMap> _instance;
	};
}
