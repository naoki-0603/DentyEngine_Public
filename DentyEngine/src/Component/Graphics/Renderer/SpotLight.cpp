#include "stdafx.h"

#include "SpotLight.h"

#include "Graphics/Sprite/Sprite3D.h"

#include "Scene/GameObject.h"

#include "System/Graphics/Renderer/LightManager.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

namespace DentyEngine
{
	SpotLight::SpotLight() : Light(), constants(), _direction(0, -1, 0, 0),
        _phi(0.959931f), _theta(0.610865f), _intensity(1.0f)
	{
		_name = "SpotLight";
	}

	SpotLight::SpotLight(const SpotLight& source) :
		Light(source), constants(source.constants), _direction(source._direction),
		_cone(source._cone), _constantAttenuation(source._constantAttenuation),
		_linearAttenuation(source._linearAttenuation), _secondOrderAttenuation(source._secondOrderAttenuation), _phi(source._phi),
	    _theta(source._theta), _intensity(source._intensity)
	{
		CreateGizmoSprite();
	}

	void SpotLight::Awake()
	{
		Light::Awake();
	}

	void SpotLight::Start()
	{
		Light::Start();
	}

	void SpotLight::OnUpdate(float deltaTime)
	{
		Light::OnUpdate(deltaTime);

		// Update constants.
		{
			constants.position = Vector4(GetParent()->transform.position, 1.0f);
			constants.direction = _direction;

			constants.color = _lightColor;
			constants.phi = _phi;
			constants.theta = _theta;
			constants.intensity = _intensity;
			constants.padding = 0.0f;

			constants.lightViewProjection = Matrix::Identity();
		}
	}

	void SpotLight::OnEvent(Event* e)
	{
		Light::OnEvent(e);
	}

	void SpotLight::OnGui()
	{
		Light::OnGui();

		ImGui::DragFloat3("Direction##SpotLight", &_direction.x, 0.1f, -1.0f, +1.0f);
		{
			ImGui::SliderAngle("Phi##SpotLight", &_phi, -90, 90);
			ImGui::SliderAngle("Theta##SpotLight", &_theta, -90.0f, _phi);
		}

		ImGui::DragFloat("Intensity##SpotLight", &_intensity, 0.1f, 1.0f, 8.0f);
	}

	void SpotLight::OnPrefabGui()
	{
		Light::OnPrefabGui();

		ImGui::DragFloat3("Direction##SpotLight", &_direction.x, 0.1f, -1.0f, +1.0f);
		{
			ImGui::SliderAngle("Phi##SpotLight", &_phi, -90, 90);
			ImGui::SliderAngle("Theta##SpotLight", &_theta, -90.0f, _phi);
		}

		ImGui::DragFloat("Intensity##SpotLight", &_intensity, 0.1f, 1.0f, 8.0f);
	}

	void SpotLight::OnSerialized() const
	{
		Light::OnSerialized();
	}

	void SpotLight::OnDeserialized()
	{
		Light::OnDeserialized();
	}

	void SpotLight::OnDestroy()
	{
		Light::OnDestroy();
	}

	void SpotLight::OnGameObjectDeserialized()
	{
		Light::OnGameObjectDeserialized();
	}

    Ref<Object> SpotLight::MakeCopy() const
	{
		return std::make_shared<SpotLight>(*std::static_pointer_cast<const SpotLight>(shared_from_this()));
	}

    Light::LightType SpotLight::GetType() const noexcept
    {
		return LightType::Spot;
    }

    void SpotLight::CreateGizmoSprite()
    {
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::SPOT_LIGHT_GIZMO_FILE_PATH);
		DENTY_CHECK_CONDITION(texture.has_value());

		_gizmoSprite = std::make_shared<Sprite3D>();
		_gizmoSprite->Create(texture.value());
    }
}
