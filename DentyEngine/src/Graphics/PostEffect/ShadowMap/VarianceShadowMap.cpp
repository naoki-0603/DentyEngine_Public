#include "stdafx.h"

#include "VarianceShadowMap.h"

#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/FrameBuffer.h"
#include "Graphics/Buffer/SceneBuffer.h"

#include "Graphics/PostEffect/GaussianBlur.h"
#include "Graphics/Buffer/ConstantBuffer.h"

#include "Graphics/Renderer/LightCamera.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
	VarianceShadowMapConstants::VarianceShadowMapConstants()
	{
	}

	VarianceShadowMapParameters::VarianceShadowMapParameters() :
		lightCamera(), shadowMap(), gaussianBlur(), playerLightCamera(), playerShadowMap(), playerShadowGaussianBlur()
	{
	}

	VarianceShadowMapParameters VarianceShadowMap::_parameters = {  };

	VarianceShadowMap::VarianceShadowMap()
	{

	}

	void VarianceShadowMap::Setup()
	{
		{
			_parameters.lightCamera = std::make_unique<LightCamera>();
			_parameters.lightCamera->Create();
		}

		{
			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			_parameters.shadowMap = std::make_shared<SceneBuffer>();
			_parameters.shadowMap->Create(
				device, 
				Vector2(VarianceShadowMapConstants::SHADOW_MAP_RESOLUTION, VarianceShadowMapConstants::SHADOW_MAP_RESOLUTION)
			);

			_parameters.gaussianBlur = std::make_shared<GaussianBlur>();
			_parameters.gaussianBlur->Create(
				_parameters.shadowMap,
				Vector2(VarianceShadowMapConstants::SHADOW_MAP_RESOLUTION, VarianceShadowMapConstants::SHADOW_MAP_RESOLUTION)
				);

			// Player shadow map.
			_parameters.playerShadowMap = std::make_shared<SceneBuffer>();
			_parameters.playerShadowMap->Create(
				device, Vector2(VarianceShadowMapConstants::PLAYER_SHADOW_MAP_RESOLUTION, VarianceShadowMapConstants::PLAYER_SHADOW_MAP_RESOLUTION)
			);

			_parameters.playerShadowGaussianBlur = std::make_shared<GaussianBlur>();
			_parameters.playerShadowGaussianBlur->Create(
				_parameters.playerShadowMap,
				Vector2(VarianceShadowMapConstants::PLAYER_SHADOW_MAP_RESOLUTION, VarianceShadowMapConstants::PLAYER_SHADOW_MAP_RESOLUTION)
			);
		}
	}

	void VarianceShadowMap::Begin(ID3D11DeviceContext* context)
	{
		_parameters.shadowMap->Clear(context, Color::GRAY);
		_parameters.shadowMap->Activate(context);
	}

	void VarianceShadowMap::End(ID3D11DeviceContext* context)
	{
		_parameters.shadowMap->DeActivate(context);
		_parameters.gaussianBlur->Process();
	}

	void VarianceShadowMap::BeginPlayerShadow(ID3D11DeviceContext* context)
	{
		_parameters.playerShadowMap->Clear(context, Color::GRAY);
		_parameters.playerShadowMap->Activate(context);
	}

	void VarianceShadowMap::EndPlayerShadow(ID3D11DeviceContext* context)
	{
		_parameters.playerShadowMap->DeActivate(context);
		_parameters.playerShadowGaussianBlur->Process();
	}

	void VarianceShadowMap::OnUpdate()
	{
		_parameters.lightCamera->Update();
	}

	void VarianceShadowMap::OnGui()
	{
		if (ImGui::TreeNodeEx("ShadowParameters##VarianceShadowMap", ImGuiTreeNodeFlags_DefaultOpen))
		{
			_parameters.lightCamera->OnGui();

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("ShadowMap##VarianceShadowMap", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("ShadowMap");
			ImGui::Image(_parameters.shadowMap->GetColorBuffer()->GetResourceAsComPtr().Get(), ImVec2(256, 256));

			ImGui::TextWrapped("Processed shadow map");
			ImGui::Image(_parameters.gaussianBlur->GetResultBuffer()->GetColorBuffer()->GetResourceAsComPtr().Get(), ImVec2(256, 256));

			ImGui::TreePop();
		}
	}

	void VarianceShadowMap::OnEndPreRendering(ID3D11DeviceContext* context)
	{
		_parameters.lightCamera->Bind();

		context->PSSetShaderResources(20u, 1u, _parameters.gaussianBlur->GetResultBuffer()->GetColorBuffer()->GetResource());
	}

	VarianceShadowMapParameters& VarianceShadowMap::GetParameters()
	{
		return _parameters;
	}
}
