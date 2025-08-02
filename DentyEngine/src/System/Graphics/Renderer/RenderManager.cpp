#include "stdafx.h"

#include "RenderManager.h"

#include "CPUParticleSystem.h"
#include "System/Core/Timer.h"
#include "System/Core/Window.h"
#include "Component/Graphics/Renderer/GameCamera.h"
#include "Component/Graphics/Renderer/Light.h"

#include "Scene/GameObject.h"

#include "Graphics/Skybox.h"

#include "Graphics/Buffer/SceneBuffer.h"

#include "Graphics/Renderer/GridRenderer.h"

#include "Graphics/States/BlendState.h"
#include "Graphics/States/DepthStencilState.h"
#include "Graphics/States/SamplerState.h"
#include "Graphics/States/RasterizerState.h"

#include "Component/Graphics/Renderer/Renderer.h"
#include "Component/Graphics/Renderer/MeshRenderer.h"
#include "Component/Graphics/Renderer/SkinnedMeshRenderer.h"
#include "Editor/Graphics/Rendering/EditorCamera.h"

#include "Editor/Parts/EditorSceneView.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/PostEffect/ShadowMap/VarianceShadowMap.h"
#include "Graphics/Renderer/LineRenderer.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Events/EventRender.h"
#include "System/Events/EventScene.h"
#include "System/Events/EventEditor.h"
#include "System/Events/EventSystem.h"

#include "System/Physics/PhysicsEngine.h"

#include "System/Profiler/RenderingState.h"
#include "System/Profiler/FunctionState.h"

#include "System/UI/UISystem.h"

namespace DentyEngine
{
	RenderManager::RenderManager() :
		IManager(), _meshRenderers(), _skinnedMeshRenderers(),
		_spriteRenderers(), _sceneViewBuffer(), _gameViewBuffer(), _skybox()
	{
	}

	void RenderManager::Initialize()
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
		const Vector2 size = Window::GetInfo().GetSize<Vector2>();

		_sceneViewBuffer = std::make_shared<SceneBuffer>();
		_sceneViewBuffer->Create(device, size);

		_gameViewBuffer = std::make_shared<SceneBuffer>();
		_gameViewBuffer->Create(device, size);

		GridRenderer::Create(device, CELL_SIZE, CELL_LINE_THICKNESS, CELL_LINE_LENGTH);

		// Resize vectors.
		{
			_meshRenderers.resize(static_cast<uint32_t>(Renderer::OrderType::Max));
			_skinnedMeshRenderers.resize(static_cast<uint32_t>(Renderer::OrderType::Max));
		}

		LineRenderer::Initialize();

		VarianceShadowMap::Setup();
		//CascadeShadowMap::Setup();
	}

	void RenderManager::Update()
	{
		for (auto&& meshRenderers : _meshRenderers)
		{
			for (auto&& meshRenderer : meshRenderers)
			{
				if (not meshRenderer->Enabled()) continue;
				if (not meshRenderer->HasMesh()) continue;

				meshRenderer->Update(Timer::GetDeltaTime());
			}
		}

		for (auto&& skinnedMeshRenderers : _skinnedMeshRenderers)
		{
			for (auto&& skinnedMeshRenderer : skinnedMeshRenderers)
			{
				if (not skinnedMeshRenderer->Enabled()) continue;
				if (not skinnedMeshRenderer->HasMesh()) continue;

				skinnedMeshRenderer->Update(Timer::GetDeltaTime());
			}
		}

		for ([[maybe_unused]] auto&& spriteRenderer : _spriteRenderers)
		{
		}

		//CascadeShadowMap::OnUpdate();
		VarianceShadowMap::OnUpdate();
	}

	void RenderManager::OnGui()
	{

	}

	void RenderManager::OnEvent(Event* e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<EventLayerInitialized>(BIND_EVENT_FUNCTION(OnLayerInitialized));

		dispatcher.Dispatch<EventOnSceneActivated>(BIND_EVENT_FUNCTION(OnSceneActivated));
		dispatcher.Dispatch<EventOnSceneDeActivated>(BIND_EVENT_FUNCTION(OnSceneDeActivated));

		dispatcher.Dispatch<EventRequiredSceneViewColorData>(BIND_EVENT_FUNCTION(OnRequiredSceneViewColorData));
		dispatcher.Dispatch<EventRequiredGameViewColorData>(BIND_EVENT_FUNCTION(OnRequiredGameViewColorData));

		dispatcher.Dispatch<EventSceneRendering>(BIND_EVENT_FUNCTION(OnSceneRender));

		dispatcher.Dispatch<EventBeginGUIRendering>(BIND_EVENT_FUNCTION(OnBeginGUIRendering));
		dispatcher.Dispatch<EventEndGUIRendering>(BIND_EVENT_FUNCTION(OnEndGUIRendering));

		dispatcher.Dispatch<EventPresent>(BIND_EVENT_FUNCTION(OnPresent));
	}

	void RenderManager::Finalize()
	{

	}

	void RenderManager::AddRenderer(Ref<Renderer> renderer)
	{
		using RendererType = Renderer::Type;

		switch (renderer->GetRendererType())
		{
		case RendererType::MeshRenderer:
			{
				_meshRenderers.at(static_cast<uint32_t>(renderer->GetOrderType()))
				.emplace_back(std::static_pointer_cast<MeshRenderer>(renderer));
			}
			break;
		case RendererType::SkinnedMeshRenderer:
			{
				_skinnedMeshRenderers.at(static_cast<uint32_t>(renderer->GetOrderType()))
				.emplace_back(std::static_pointer_cast<SkinnedMeshRenderer>(renderer));
			}
			break;
		case RendererType::SpriteRenderer:
			break;
		default:
			DENTY_ASSERT(false, "Unknown renderer type! (in RenderManager::AddRenderer)");
			break;
		}
	}

	void RenderManager::RemoveRenderer(const Ref<Renderer>& renderer)
	{
		using RendererType = Renderer::Type;

		switch (renderer->GetRendererType())
		{
		case RendererType::MeshRenderer:
		{
			for (auto&& meshRenderers : _meshRenderers)
			{
				auto&& it = std::ranges::find(meshRenderers.begin(), meshRenderers.end(), renderer);
				if (it != meshRenderers.end())
				{
					meshRenderers.erase(it);
				}
			}
		}
		break;
		case RendererType::SkinnedMeshRenderer:
		{
			for (auto&& skinnedMeshRenderers : _skinnedMeshRenderers)
			{
				auto&& it = std::ranges::find(skinnedMeshRenderers.begin(), skinnedMeshRenderers.end(), renderer);
				if (it != skinnedMeshRenderers.end())
				{
					skinnedMeshRenderers.erase(it);
				}
			}
		}
		break;
		case RendererType::SpriteRenderer:
			break;
		default:
			DENTY_ASSERT(false, "Unknown renderer type! (in RenderManager::RemoveRenderer)");
			break;
		}
	}

	void RenderManager::OnRendererOrderTypeChanged(Ref<Renderer> targetRenderer)
	{
		// First delete target renderer from manager.
		RemoveRenderer(targetRenderer);

		// Re add to manager.
		AddRenderer(targetRenderer);
	}

	void RenderManager::OnSceneSkyboxChanged(Ref<Skybox> skybox)
	{
		_skybox = skybox;
	}

	void RenderManager::Reset()
	{
		for (auto&& meshRenderers : _meshRenderers)
		{
			meshRenderers.clear();
		}

		for (auto&& skinnedMeshRenderers : _skinnedMeshRenderers)
		{
			skinnedMeshRenderers.clear();
		}
		_spriteRenderers.clear();

		_skybox = nullptr;
	}

	void RenderManager::Render2D([[maybe_unused]] ID3D11DeviceContext* context) const
	{
		UISystem::OnSceneRender();
	}

    void RenderManager::ResetShaderAndResources(ID3D11DeviceContext* context)
    {
		ID3D11ShaderResourceView* nullShaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};

		context->VSSetShaderResources(0, _countof(nullShaderResourceViews), nullShaderResourceViews);
		context->PSSetShaderResources(0, _countof(nullShaderResourceViews), nullShaderResourceViews);
	}

    void RenderManager::OnLayerInitialized([[maybe_unused]] EventLayerInitialized* e)
    {
		Reset();
    }

    void RenderManager::OnSceneActivated([[maybe_unused]] EventOnSceneActivated* e)
    {
		//CascadeShadowMap::OnSceneActivated();
    }

    void RenderManager::Render3D(ID3D11DeviceContext* context) const
	{
		// Static mesh.
		for (auto&& meshRenderers : _meshRenderers)
		{
			for (auto&& meshRenderer : meshRenderers)
			{
				if (not meshRenderer->GetParent()->Enabled()) continue;
				if (not meshRenderer->HasMesh()) continue;
				if (not meshRenderer->Enabled()) continue;

				{
					RenderingStateSpecification specification =
					{
						meshRenderer->GetMesh()->GetName().c_str()
					};
					RenderingState renderingState(specification);

					meshRenderer->Render(context);
				}
			}
			
		}

		// Skinned mesh.
		for (auto&& skinnedMeshRenderers : _skinnedMeshRenderers)
		{
			for (auto&& skinnedMeshRenderer : skinnedMeshRenderers)
			{
				if (not skinnedMeshRenderer->GetParent()->Enabled()) continue;
				if (not skinnedMeshRenderer->HasMesh()) continue;
				if (not skinnedMeshRenderer->Enabled()) continue;

				{
					RenderingStateSpecification specification =
					{
						skinnedMeshRenderer->GetMesh()->GetName().c_str()
					};
					RenderingState renderingState(specification);

					skinnedMeshRenderer->Render(context);
				}
			}
		}
	}

	void RenderManager::OnSceneDeActivated([[maybe_unused]] EventOnSceneDeActivated* e)
	{
		Reset();
	}

	void RenderManager::OnRequiredSceneViewColorData(EventRequiredSceneViewColorData* e)
	{
		e->SetSceneBuffer(_sceneViewBuffer);
	}

	void RenderManager::OnRequiredGameViewColorData(EventRequiredGameViewColorData* e)
	{
		e->SetSceneBuffer(_gameViewBuffer);
	}

	void RenderManager::OnSceneRender(EventSceneRendering* e)
	{
		/*if (ApplicationLayer::IsPlay() && e->_viewType == EventSceneRendering::ViewType::Scene)
			return;*/

		{
			FunctionStateSpecification specification =
			{
				"RenderManager::OnSceneRender"
			};
			FunctionState functionState(specification);

			// Set main buffer
			{
				const float clearColor[4] = { Color::GRAY.x, Color::GRAY.y, Color::GRAY.z, Color::GRAY.w };
				e->_context->ClearRenderTargetView(e->_renderTargetView, clearColor);

				constexpr FLOAT blendFactor[4] = { 1,1,1,1 };
				ID3D11BlendState* blendState = BlendState::GetBuildInBlendStatesAt(BlendState::Type::NoBlend);
				ID3D11DepthStencilState* depthStencilState = DepthStencilState::GetBuildInDepthStencilStatesAt(DepthStencilState::Type::DepthOnStencilOn);
				ID3D11RasterizerState* rasterizerState = RasterizerState::GetBuildInRasterizerStatesAt(RasterizerState::Type::FillCullBack);
				ID3D11SamplerState* samplerStates[] =
				{
					SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::PointWrap),
					SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::LinearWrap),
					SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::AnisotropicWrap)
				};

				e->_context->OMSetBlendState(blendState, blendFactor, 0xFFFFFFFF);
				e->_context->OMSetDepthStencilState(depthStencilState, 0);
				e->_context->PSSetSamplers(0, _countof(samplerStates), samplerStates);
				e->_context->RSSetState(rasterizerState);
				e->_context->OMSetRenderTargets(1, &e->_renderTargetView, nullptr);
				e->_context->RSSetViewports(1, &e->_viewport);

				//ShadowMap::GetInstance()->BindCamera(e->_context);

				// Tells the camera that you have started drawing.
				switch (e->_viewType)
				{
				case EventSceneRendering::ViewType::Scene:
				{
					EventEditorBeginRenderingProcess event(e->_context, static_cast<uint32_t>(ConstantBuffer::ReservedBindSlot::Camera));
					NOTIFY_EVENT(event);
				}
				break;
				case EventSceneRendering::ViewType::Game:
				{
					EventGameBeginRenderingProcess event(e->_context);
					NOTIFY_EVENT(event);
				}
				break;
				default:
					DENTY_ASSERT(false, L"Unknown view type. (in RenderManager::OnSceneRender)");
					break;
				}
			}

			// PreRendering
			{
				RenderingStateSpecification preRenderingStateSpecification =
				{
					"PreRendering"
				};
				RenderingState preRenderingState(preRenderingStateSpecification);

				BeginPreRendering(e->_context, e->_viewType);

				EndPreRendering(e->_context);
			}

			switch (e->_viewType)
			{
			case EventSceneRendering::ViewType::Scene:
				_sceneViewBuffer->Clear(e->_context, Color::BLACK);
				_sceneViewBuffer->Activate(e->_context);

				break;
			case EventSceneRendering::ViewType::Game:
				_gameViewBuffer->Clear(e->_context, Color::BLACK);
				_gameViewBuffer->Activate(e->_context);

				break;
			default:
				DENTY_ASSERT(false, L"Unknown view type. (in RenderManager::OnSceneRender)");
				break;
			}

			// Rendering
			{
				BeginRendering(e->_context);

				// Render3D
				Render3D(e->_context);

				// If view type is scene, render grid and bullet physics debug draw.
				if (e->_viewType == EventSceneRendering::ViewType::Scene)
				{
					if (GridRenderer::IsEnabled())
						GridRenderer::Render(e->_context);

					// Bullet physics debug draw.
					PhysicsEngine::GetInstance()->RenderDebugDrawer();

					// Gizmo render
					EditorSceneView::RenderGizmos();
				}

				EndRendering(e->_context);
			}

			// PostEffect
			{
				BeginPostEffect(e->_context);

				EndPostEffect(e->_context);
			}

			// Particle
			{
				switch (e->_viewType)
				{
				case EventSceneRendering::ViewType::Scene:
					{
						const Scope<EditorCamera>& camera = EditorSceneView::GetEditorCamera();

						CPUParticleSystem::GetInstance()->OnRender(camera->GetView());
					}
					break;
				case EventSceneRendering::ViewType::Game:
					{
						Ref<GameCamera> camera = GameCamera::GetMainCamera();

						CPUParticleSystem::GetInstance()->OnRender(camera->GetView());
					}
					break;
				}

			}

			// Render Lines.
			{
				LineRenderer::Flush();
			}

			// HUD
			if (e->_viewType == EventSceneRendering::ViewType::Game)
			{
				BeginHUDRendering(e->_context);

				Render2D(e->_context);

				EndHUDRendering(e->_context);
			}

			switch (e->_viewType)
			{
			case EventSceneRendering::ViewType::Scene:
				_sceneViewBuffer->DeActivate(e->_context);

				{
					EventEditorEndRenderingProcess event(e->_context);
					NOTIFY_EVENT(event);

					ResetShaderAndResources(e->_context);
				}
				break;
			case EventSceneRendering::ViewType::Game:
				_gameViewBuffer->DeActivate(e->_context);

				{
					EventGameEndRenderingProcess event(e->_context);
					NOTIFY_EVENT(event);

					ResetShaderAndResources(e->_context);
				}
				break;
			default:
				DENTY_ASSERT(false, L"Unknown view type. (in RenderManager::OnSceneRender)");
				break;
			}
		}
	}

	void RenderManager::BeginPreRendering(ID3D11DeviceContext* context, EventSceneRendering::ViewType viewType)
	{
		// Shadow map process.
		{
			// Has game camera?
			//if (not GameCamera::GetMainCamera())
			//	return;

			//// Calculate light view projections.
			//{
			//	switch (viewType)
			//	{
			//	case EventSceneRendering::ViewType::Scene:
			//		{
			//			CascadeShadowMap::CalculateLightViewProjections(EditorSceneView::GetEditorCamera().get());
			//		}
			//		break;
			//	case EventSceneRendering::ViewType::Game:
			//		{
			//			CascadeShadowMap::CalculateLightViewProjections(GameCamera::GetMainCamera().get());
			//		}
			//		break;
			//	}
			//}
			
			//ShadowMap::GetInstance()->Begin(context);

			VarianceShadowMap::Begin(context);

			//for (uint32_t cascadeIndex = 0u; cascadeIndex < CascadeShadowMapConstants::NUM_SHADOW_MAP; ++cascadeIndex)
			{
				//CascadeShadowMap::Begin(context, cascadeIndex);

				for (auto&& meshRenderers : _meshRenderers)
				{
					for (auto&& meshRenderer : meshRenderers)
					{
						if (not meshRenderer->GetParent()->Enabled()) continue;
						if (not meshRenderer->shadowCastEnabled) continue;
						if (not meshRenderer->Enabled()) continue;
						if (not meshRenderer->HasMesh()) continue;

						meshRenderer->RenderShadow(context, 0u);
					}
				}

				for (auto&& skinnedMeshRenderers : _skinnedMeshRenderers)
				{
					for (auto&& skinnedMeshRenderer : skinnedMeshRenderers)
					{
						if (not skinnedMeshRenderer->GetParent()->Enabled()) continue;
						if (not skinnedMeshRenderer->shadowCastEnabled) continue;
						if (not skinnedMeshRenderer->Enabled()) continue;
						if (not skinnedMeshRenderer->HasMesh()) continue;

						skinnedMeshRenderer->RenderShadow(context, 0u);
					}
				}

				//CascadeShadowMap::End(context, cascadeIndex);
			}

			VarianceShadowMap::End(context);
		}
	}

	void RenderManager::EndPreRendering(ID3D11DeviceContext* context)
	{
		//CascadeShadowMap::OnEndPreRendering(context);
		VarianceShadowMap::OnEndPreRendering(context);
		//ShadowMap::GetInstance()->End(context);
	}

	void RenderManager::BeginRendering(ID3D11DeviceContext* context)
	{
		if (_skybox)
		{
			_skybox->Render(context);

			_skybox->BindCubeMap();
		}
	}

	void RenderManager::EndRendering([[maybe_unused]] ID3D11DeviceContext* context)
	{
	}

	void RenderManager::BeginPostEffect([[maybe_unused]] ID3D11DeviceContext* context)
	{
	}

	void RenderManager::EndPostEffect([[maybe_unused]] ID3D11DeviceContext* context)
	{

	}

	void RenderManager::BeginHUDRendering([[maybe_unused]] ID3D11DeviceContext* context)
	{
	}

	void RenderManager::EndHUDRendering(ID3D11DeviceContext* context)
	{
		ID3D11RenderTargetView* nullRenderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};

		context->OMSetRenderTargets(_countof(nullRenderTargetViews), nullRenderTargetViews, nullptr);

		ResetShaderAndResources(context);
	}

	void RenderManager::OnBeginGUIRendering([[maybe_unused]] EventBeginGUIRendering* e)
	{
	}

	void RenderManager::OnEndGUIRendering([[maybe_unused]] EventEndGUIRendering* e)
	{

	}

	void RenderManager::OnPresent(EventPresent* e)
	{
		e->_swapChain->Present(1, 0);
	}
};