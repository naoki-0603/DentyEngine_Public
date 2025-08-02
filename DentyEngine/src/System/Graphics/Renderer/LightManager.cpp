#include "stdafx.h"

#include "LightManager.h"

#include "System/Core/Timer.h"

#include "Graphics/Buffer/ConstantBuffer.h"

#include "System/Events/EventScene.h"
#include "System/Events/EventRender.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
    LightManager::LightManager() : IManager(),
        _directionalLight(), _hemisphereLight(), _pointLights(), _spotLights(), _constantBuffer(), _constants()
    {
    }

    void LightManager::Initialize()
    {
        ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

        _constantBuffer = std::make_shared<ConstantBuffer>(device, sizeof(Constants));
    }

    void LightManager::Finalize()
    {
        Reset();
    }

    void LightManager::Update()
    {
        if (_directionalLight && _directionalLight->Enabled())
        {
            _directionalLight->OnUpdate(Timer::GetDeltaTime());
        }

        if (_hemisphereLight && _hemisphereLight->Enabled())
        {
            _hemisphereLight->OnUpdate(Timer::GetDeltaTime());
        }

        for (auto&& pointLight : _pointLights)
        {
            if (not pointLight->Enabled()) continue;

            pointLight->OnUpdate(Timer::GetDeltaTime());
        }

        for (auto&& spotLight : _spotLights)
        {
            if (not spotLight->Enabled()) continue;

            spotLight->OnUpdate(Timer::GetDeltaTime());
        }

        UpdateConstants();
    }

    void LightManager::OnEvent(Event* e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<EventSceneRendering>(BIND_EVENT_FUNCTION(OnSceneRendering));
        dispatcher.Dispatch<EventOnSceneDeActivated>(BIND_EVENT_FUNCTION(OnSceneDeActivated));
    }

    void LightManager::OnGui()
    {
        if (ImGui::CollapsingHeader("LightManager##CoreLayer", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // Directional light.
            {
                if (DirectionalLightExists())
                {
                    ImGui::Text("Directional Light Exists.");
                }
                else
                {
                    ImGui::Text("Directional Light Doesn't Exists.");
                }
            }

            // Hemisphere light.
            {
	            if (HemisphereLightExists())
	            {
                    ImGui::Text("Hemisphere Light Exists.");
	            }
                else
                {
                    ImGui::Text("Hemisphere Light Doesn't Exists.");
                }
            }

            // Point lights.
            {
                if (PointLightExists())
                {
                    ImGui::Text("Point Light Count: %d", static_cast<int32_t>(_pointLights.size()));

                }
                else
                {
                    ImGui::Text("Point Lights Doesn't Exists.");

                }
            }

            // Spot lights.
            {
                if (SpotLightExists())
                {
                    ImGui::Text("Spot Light Count: %d", static_cast<int32_t>(_spotLights.size()));
                }
                else
                {
                    ImGui::Text("Spot Lights Doesn't Exists.");
                }
            }
        }
    }

    void LightManager::AddLight(Ref<Light> light)
    {
        using LightType = Light::LightType;

        switch (light->GetType())
        {
        case LightType::Directional:
        {
        	if (!_directionalLight)
        	{
                _directionalLight = std::static_pointer_cast<DirectionalLight>(light);
        	}
        }
        break;
        case LightType::Point:
        {
            _pointLights.emplace_back(std::static_pointer_cast<PointLight>(light));
        }
        break;
        case LightType::Spot:
        {
            _spotLights.emplace_back(std::static_pointer_cast<SpotLight>(light));
        }
        break;
		case LightType::Hemisphere:
		{
			if (!_hemisphereLight)
			{
				_hemisphereLight = std::static_pointer_cast<HemisphereLight>(light);
			}
		}
		break;
        default:
            DENTY_ASSERT(false, "Unknown light type! (in LightManager::AddLight)");
            break;
        }
    }

    void LightManager::RemoveLight(const Ref<Light>& light)
    {
        using LightType = Light::LightType;

        switch (light->GetType())
        {
        case LightType::Directional:
        {
            _directionalLight = nullptr;
        }
        break;
        case LightType::Point:
        {
            auto&& it = std::ranges::find(_pointLights.begin(), _pointLights.end(), light);
            if (it != _pointLights.end())
            {
                _pointLights.erase(it);
            }
        }
        break;
        case LightType::Spot:
        {
            auto&& it = std::ranges::find(_spotLights.begin(), _spotLights.end(), light);
            if (it != _spotLights.end())
            {
                _spotLights.erase(it);
            }
        }
        break;
        case LightType::Hemisphere:
	        {
            _hemisphereLight = nullptr;
	        }
            break;
        default:
            DENTY_ASSERT(false, "Unknown light type! (in LightManager::RemoveLight)");
            break;
        }
    }

    void LightManager::Reset()
    {
        _directionalLight = nullptr;
        _hemisphereLight = nullptr;
        _pointLights.clear();
        _spotLights.clear();
    }

    void LightManager::UpdateConstants()
    {
        // Directional light
        if (_directionalLight)
        {
            _constants.directionalLightConstants = _directionalLight->constants;
        }

        // Hemisphere light.
        if (_hemisphereLight)
        {
            _constants.hemisphereLightConstants = _hemisphereLight->constants;
        }

        // Point lights
        for (int index = 0; index < _pointLights.size(); ++index)
        {
            const auto& pointLight = _pointLights.at(index);

            if (not pointLight->Enabled())
            {
                _constants.pointLightConstants.at(index).lightPosition = { };
                _constants.pointLightConstants.at(index).lightColor = { };
                _constants.pointLightConstants.at(index).lightViewProjection = Matrix::Identity();

                continue;
            }

            _constants.pointLightConstants.at(index) = pointLight->constants;
        }

        // Spot Lights.
        for (int index = 0; index < _spotLights.size(); ++index)
        {
            const auto& spotLight = _spotLights.at(index);

            if (not spotLight->Enabled())
            {
                _constants.spotLightConstants.at(index).position = { };
                _constants.spotLightConstants.at(index).color = { };
                _constants.spotLightConstants.at(index).lightViewProjection = Matrix::Identity();

                continue;
            }

            _constants.spotLightConstants.at(index) = spotLight->constants;
        }

        // Each lights count.
        _constants.pointLightCount = static_cast<int>(_pointLights.size());
        _constants.spotLightCount = static_cast<int>(_spotLights.size());
    }

    void LightManager::OnSceneRendering(EventSceneRendering* e)
    {
        _constantBuffer->UpdateSubresource(e->_context, &_constants);
        _constantBuffer->BindAll(e->_context, static_cast<UINT>(ConstantBuffer::ReservedBindSlot::Light));
    }

    void LightManager::OnSceneDeActivated([[maybe_unused]] EventOnSceneDeActivated* e)
    {
        Reset();
    }
}
