#include "stdafx.h"

#include "PointLight.h"

#include "Graphics/Sprite/Sprite3D.h"

#include "Scene/GameObject.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

namespace DentyEngine
{
	PointLight::PointLight() : Light(), constants(), _constantAttenuation(0.5f),
		_linearAttenuation(0.3f), _secondOrderAttenuation(0.2f), _intensity(1.0f), _range(5.0f)
	{
		_name = "PointLight";
	}

	PointLight::PointLight(const PointLight& source) :
		Light(source), constants(source.constants),
		_constantAttenuation(source._constantAttenuation),
		_linearAttenuation(source._linearAttenuation),
		_secondOrderAttenuation(source._secondOrderAttenuation),
		_intensity(source._intensity), _range(source._range)
	{
		CreateGizmoSprite();
	}

	void PointLight::Awake()
	{
		Light::Awake();
	}

	void PointLight::Start()
	{
		Light::Start();
	}

	void PointLight::OnUpdate(float deltaTime)
	{
		Light::OnUpdate(deltaTime);

		// Update constants.
		{
			constants.lightPosition = Vector4(GetParent()->transform.position, 1.0f);
			constants.lightColor = _lightColor;
			constants.intensity = _intensity;
			constants.range = _range;
			constants.padding = Vector2();

			constants.lightViewProjection = Matrix::Identity();
		}
	}

	void PointLight::OnEvent(Event* e)
	{
		Light::OnEvent(e);
	}

	void PointLight::OnGui()
	{
		Light::OnGui();

		ImGui::DragFloat("Intensity##PointLight", &_intensity, 0.01f, 1.0f, 8.0f);
		ImGui::DragFloat("Range##PointLight", &_range, 1, 1.0f, 15.0f);
	}

	void PointLight::OnPrefabGui()
	{
		Light::OnPrefabGui();

		ImGui::DragFloat("Intensity##PointLight", &_intensity, 0.01f, 1.0f, 8.0f);
		ImGui::DragFloat("Range##PointLight", &_range, 1, 1.0f, 15.0f);
	}

	void PointLight::OnSerialized() const
	{
		Light::OnSerialized();
	}

	void PointLight::OnDeserialized()
	{
		Light::OnDeserialized();
	}

	void PointLight::OnDestroy()
	{
		Light::OnDestroy();
	}

	void PointLight::OnGameObjectDeserialized()
	{
		Light::OnGameObjectDeserialized();
	}

    Ref<Object> PointLight::MakeCopy() const
	{
		return std::make_shared<PointLight>(*std::static_pointer_cast<const PointLight>(shared_from_this()));
	}

    Light::LightType PointLight::GetType() const noexcept
    {
		return LightType::Point;
    }

    void PointLight::CreateGizmoSprite()
    {
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::POINT_LIGHT_GIZMO_FILE_PATH);
		DENTY_CHECK_CONDITION(texture.has_value());

		_gizmoSprite = std::make_shared<Sprite3D>();
		_gizmoSprite->Create(texture.value());
    }
}
