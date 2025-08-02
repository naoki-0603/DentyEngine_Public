#include "stdafx.h"

#include "CascadeShadowMap.h"

#include "Graphics/Buffer/FrameBuffer.h"
#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/ConstantBuffer.h"

#include "Graphics/Renderer/Camera.h"

#include "Component/Graphics/Renderer/DirectionalLight.h"

#include "Graphics/Renderer/LightCamera.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Scene/Scene.h"

namespace DentyEngine
{
	CascadeShadowMapParameters CascadeShadowMap::_parameters = {};

	CascadeShadowMapConstants::CascadeShadowMapConstants()
	{
	}

	CascadeShadowMapParameters::CascadeShadowMapParameters() :
		lightCamera(), shadowMaps(), cascadeAreaTable(), lightViewProjectionClopMatrices()
	{
	}

	CascadeShadowMap::CascadeShadowMap()
	{
	}

	CascadeShadowMap::~CascadeShadowMap()
	{
	}

	void CascadeShadowMap::Setup()
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

		// Create light camera.
		{
			_parameters.lightCamera = std::make_unique<LightCamera>();
			_parameters.lightCamera->Create();
		}

		// Create shadow maps.
		{
			for (uint8_t index = 0u; index < CascadeShadowMapConstants::NUM_SHADOW_MAP; ++index)
			{
				const float resolution = CascadeShadowMapConstants::SHADOW_MAP_RESOLUTIONS.at(index);

				_parameters.shadowMaps.at(index) = std::make_unique<FrameBuffer>();
				_parameters.shadowMaps.at(index)->Create(device, Vector2(resolution, resolution));
			}
		}
	}

	void CascadeShadowMap::Begin(ID3D11DeviceContext* context, uint32_t areaNo)
    {
		_parameters.shadowMaps.at(areaNo)->Clear(context, Color::GRAY);
		_parameters.shadowMaps.at(areaNo)->Activate(context);
    }

    void CascadeShadowMap::End(ID3D11DeviceContext* context, uint32_t areaNo)
    {
		_parameters.shadowMaps.at(areaNo)->DeActivate(context);
    }

    void CascadeShadowMap::OnUpdate()
    {
		_parameters.lightCamera->Update();
    }

    void CascadeShadowMap::OnGui()
    {
		if (ImGui::TreeNodeEx("ShadowParameters##CascadeShadowMap", ImGuiTreeNodeFlags_DefaultOpen))
		{
			_parameters.lightCamera->OnGui();

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("ShadowMaps##CascadeShadowMap", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::TextWrapped("NearShadowMap");
			ImGui::Image(_parameters.shadowMaps.at(0)->GetColorBuffer()->GetResourceAsComPtr().Get(), ImVec2(128, 128));

			ImGui::TextWrapped("MiddleShadowMap");
			ImGui::Image(_parameters.shadowMaps.at(1)->GetColorBuffer()->GetResourceAsComPtr().Get(), ImVec2(128, 128));

			ImGui::TextWrapped("FarShadowMap");
			ImGui::Image(_parameters.shadowMaps.at(2)->GetColorBuffer()->GetResourceAsComPtr().Get(), ImVec2(128, 128));

			ImGui::TreePop();
		}
		
    }

    void CascadeShadowMap::CalculateLightViewProjections(const Camera* camera)
	{
		DENTY_CHECK_NULL(camera);

		const Matrix& lightViewProjection = _parameters.lightCamera->GetParameters().viewProjection;

		const Camera::Perspective& perspective = camera->GetPerspective();

		const Vector3 right = camera->GetRight().Normalize();
		const Vector3 front = camera->GetFront().Normalize();
		const Vector3 up = right.Cross(front).Normalize();

		// Define cascade area table.
		{
			const std::array<float, 3u> cascadeAreaTable = _parameters.lightCamera->GetParameters().cascadeAreaTable;

			_parameters.cascadeAreaTable.at(0) = cascadeAreaTable[0];
			_parameters.cascadeAreaTable.at(1) = cascadeAreaTable[1];
			_parameters.cascadeAreaTable.at(2) = cascadeAreaTable[2];
		}

		float nearDepth = perspective.nearZ;

		// Calculate 8 points within frustum.
		for (uint32_t areaNo = 0u; areaNo < CascadeShadowMapConstants::NUM_SHADOW_MAP; ++areaNo)
		{
			const float nearY = std::tanf(perspective.GetFovAngleRadian() * 0.5f) * nearDepth;
			const float nearX = nearY * perspective.aspectRatio;
			const float farY = std::tanf(perspective.GetFovAngleRadian() * 0.5f) * _parameters.cascadeAreaTable.at(areaNo);
			const float farX = farY * perspective.aspectRatio;

			// Calculate center of near and far position.
			const Vector3 nearCenterPosition = camera->GetPosition() + front * nearDepth;
			const Vector3 farCenterPosition = camera->GetPosition() + front * _parameters.cascadeAreaTable.at(areaNo);

			std::array<Vector3, 8u> vertices = { };

			// Near plane.
			{
				// Top right
				vertices.at(0) = nearCenterPosition + up * nearY + right * nearX;

				// Top left
				vertices.at(1) = nearCenterPosition + up * nearY + right * -nearX;

				// Bottom right
				vertices.at(2) = nearCenterPosition + up * -nearY + right * nearX;

				// Bottom left
				vertices.at(3) = nearCenterPosition + up * -nearY + right * -nearX;
			}

			// Far plane.
			{
				// Top right
				vertices.at(4) = farCenterPosition + up * farY + right * farX;

				// Top left
				vertices.at(5) = farCenterPosition + up * farY + right * -farX;

				// Bottom right
				vertices.at(6) = farCenterPosition + up * -farY + right * farX;

				// Bottom left
				vertices.at(7) = farCenterPosition + up * -farY + right * -farX;
			}

			// Calculate min max.
			Vector3 max, min;
			{
				max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
				min = { FLT_MAX, FLT_MAX, FLT_MAX };

				for (auto& vertex : vertices)
				{
					vertex = lightViewProjection.TransformCoord(vertex);

					max.x = std::max(vertex.x, max.x);
					max.y = std::max(vertex.y, max.y);
					max.z = std::max(vertex.z, max.z);

					min.x = std::min(vertex.x, min.x);
					min.y = std::min(vertex.y, min.y);
					min.z = std::min(vertex.z, min.z);
				}
			}

			// Calculate crop matrix.
			{
				Matrix cropMatrix = Matrix::Identity();

				const float aabbXLength = (max.x - min.x);
				const float aabbYLength = (max.y - min.y);
				const float centerOfX = (max.x + min.x) * 0.5f;
				const float centerOfY = (max.y + min.y) * 0.5f;

				const float xScale = 2.0f / aabbXLength;
				const float yScale = 2.0f / aabbYLength;
				const float xOffset = centerOfX * -1.0f * xScale;
				const float yOffset = centerOfY * -1.0f * yScale;

				cropMatrix._11 = xScale;
				cropMatrix._22 = yScale;
				cropMatrix._41 = xOffset;
				cropMatrix._42 = yOffset;

				_parameters.lightViewProjectionClopMatrices.at(areaNo) = lightViewProjection * cropMatrix;
			}

			nearDepth = _parameters.cascadeAreaTable.at(areaNo);
		}
	}

	void CascadeShadowMap::OnEndPreRendering(ID3D11DeviceContext* context)
	{
		_parameters.lightCamera->Update();
		_parameters.lightCamera->Bind();

		for (uint32_t cascadeIndex = 0u; cascadeIndex < CascadeShadowMapConstants::NUM_SHADOW_MAP; ++cascadeIndex)
		{
			context->PSSetShaderResources(
				CascadeShadowMapConstants::SHADOW_MAP_BIND_SLOTS.at(cascadeIndex), 1u,
				_parameters.shadowMaps.at(cascadeIndex)->GetColorBuffer()->GetResource()
			);
		}
	}

	CascadeShadowMapParameters& CascadeShadowMap::GetParameters()
	{
		return _parameters;
	}
}
