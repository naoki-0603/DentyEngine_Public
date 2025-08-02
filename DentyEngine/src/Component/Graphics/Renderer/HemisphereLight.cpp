#include "stdafx.h"

#include "HemisphereLight.h"

#include "Graphics/Sprite/Sprite3D.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

namespace DentyEngine
{
	HemisphereLightConstants::HemisphereLightConstants() :
		skyDirection(), padding(),
		skyColor(), groundColor()
	{
	}

	HemisphereLight::HemisphereLight() :
		Light(), constants(), _skyDirection(Vector3::Up()), _skyColor(Color::BLUE), _groundColor(Color::GREEN)
	{
		_name = "HemisphereLight";
	}

	HemisphereLight::HemisphereLight(const HemisphereLight& source) :
		Light(source), _skyDirection(source._skyDirection), _skyColor(source._skyColor),
		_groundColor(source._groundColor)
	{
		CreateGizmoSprite();
	}

	void HemisphereLight::Awake()
	{
		Light::Awake();
	}

	void HemisphereLight::Start()
	{
		Light::Start();
	}

	void HemisphereLight::OnUpdate(float deltaTime)
	{
		Light::OnUpdate(deltaTime);

		// Update constants.
		constants.skyDirection = _skyDirection;
		constants.skyColor = _skyColor;
		constants.groundColor = _groundColor;
	}

	void HemisphereLight::OnEvent(Event* e)
	{
		Light::OnEvent(e);
	}

	void HemisphereLight::OnGui()
	{
		//Light::OnGui();

		ImGui::DragFloat3("SkyDirection##HemisphereLight", &_skyDirection.x, 1.0f, -1.0f, +1.0f);
		ImGui::ColorEdit4("SkyColor##HemisphereLight", &_skyColor.color.x);
		ImGui::ColorEdit4("GroundColor##HemisphereLight", &_groundColor.color.x);
	}

	void HemisphereLight::OnPrefabGui()
	{
		Light::OnPrefabGui();
	}

	void HemisphereLight::OnSerialized() const
	{
		Light::OnSerialized();
	}

	void HemisphereLight::OnDeserialized()
	{
		Light::OnDeserialized();
	}

	void HemisphereLight::OnDestroy()
	{
		Light::OnDestroy();
	}

	void HemisphereLight::OnGameObjectDeserialized()
	{
		Light::OnGameObjectDeserialized();
	}

	Ref<Object> HemisphereLight::MakeCopy() const
	{
		return std::make_shared<HemisphereLight>(*std::static_pointer_cast<const HemisphereLight>(shared_from_this()));

	}

	Light::LightType HemisphereLight::GetType() const noexcept
	{
		return LightType::Hemisphere;
	}

	void HemisphereLight::CreateGizmoSprite()
	{
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::HEMISPHERE_LIGHT_GIZMO_FILE_PATH);
		DENTY_CHECK_CONDITION(texture.has_value());

		_gizmoSprite = std::make_shared<Sprite3D>();
		_gizmoSprite->Create(texture.value());
	}
}
