#include "stdafx.h"

#include "DirectionalLight.h"

#include "GameCamera.h"
#include "Graphics/Sprite/Sprite3D.h"

#include "Scene/GameObject.h"

#include "System/Graphics/Renderer/LightManager.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

namespace DentyEngine
{
	DirectionalLight::DirectionalLight() :
		Light(), constants(), _direction(-1, -1, 0.0f, 0.0f), _lightCameraRef()
	{
		_requiredComponentNames.emplace_back("GameCamera");

		_name = "DirectionalLight";
	}

	DirectionalLight::DirectionalLight(const DirectionalLight& source) :
		Light(source), constants(source.constants),
		_direction(source._direction)
	{
		CreateGizmoSprite();
	}

	void DirectionalLight::Awake()
	{
		Light::Awake();

		// Setup camera.
		{
			const auto camera = GetParent()->GetComponent<GameCamera>();

			if (camera)
			{
				_lightCameraRef = camera;
			}
			else
			{
				_lightCameraRef = GetParent()->AddComponent<GameCamera>().get();
			}
		}
	}

	void DirectionalLight::Start()
	{
		Light::Start();
	}
	
	void DirectionalLight::OnUpdate(float deltaTime)
	{
		Light::OnUpdate(deltaTime);

		// Update constants.
		{
		    constants.lightPosition = Vector4(GetParent()->transform.position, 1.0f);
		    constants.lightColor = _lightColor;
		    constants.lightDirection = _direction;

			for (auto&& lightViewProjection : constants.lightViewProjections)
			{
		        lightViewProjection = Matrix::Identity();
			}
		}
	}

	void DirectionalLight::OnEvent(Event* e)
	{
		Light::OnEvent(e);
	}

	void DirectionalLight::OnGui()
	{
		Light::OnGui();

		ImGui::DragFloat3("Direction##DirectionalLight", &_direction.x, 0.1f, -1.0f, +1.0f);
	}

	void DirectionalLight::OnPrefabGui()
	{
		Light::OnPrefabGui();

		ImGui::DragFloat3("Direction##DirectionalLight", &_direction.x, 0.1f, -1.0f, +1.0f);
	}

	void DirectionalLight::OnSerialized() const
	{
		Light::OnSerialized();
	}

	void DirectionalLight::OnDeserialized()
	{
		Light::OnDeserialized();
	}

	void DirectionalLight::OnDestroy()
	{
		Light::OnDestroy();
	}

	void DirectionalLight::OnGameObjectDeserialized()
	{
		Light::OnGameObjectDeserialized();
	}

    Ref<Object> DirectionalLight::MakeCopy() const
	{
		return std::make_shared<DirectionalLight>(*std::static_pointer_cast<const DirectionalLight>(shared_from_this()));
	}

    Light::LightType DirectionalLight::GetType() const noexcept
    {
		return LightType::Directional;
    }

    void DirectionalLight::CreateGizmoSprite()
    {
		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::DIRECTIONAL_LIGHT_GIZMO_FILE_PATH);
		DENTY_CHECK_CONDITION(texture.has_value());

		_gizmoSprite = std::make_shared<Sprite3D>();
		_gizmoSprite->Create(texture.value());
    }
}
