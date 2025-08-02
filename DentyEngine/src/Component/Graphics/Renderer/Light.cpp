#include "stdafx.h"

#include "Light.h"

#include "Graphics/Renderer/Texture2D.h"

#include "Graphics/Sprite/Sprite3D.h"

#include "Scene/GameObject.h"

#include "System/Graphics/Renderer/LightManager.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
	const std::vector<std::string> Light::SHADOW_TYPES =
	{
		"Hard", "Soft"
	};

	Light::Light() : Component(),
		_lightColor(Color::WHITE), _shadowType(ShadowType::Hard), _gizmoSprite(), _shadowTypeString()
	{

		_shadowTypeString = std::string(NAMEOF_ENUM(_shadowType));

		_multipleAllowed = false;
	}

	Light::Light(const Light& source) :
		Component(source), _lightColor(source._lightColor),
		_shadowType(source._shadowType),
		_shadowTypeString(source._shadowTypeString)
	{
	}

	void Light::Awake()
	{
		
	}

	void Light::Start()
	{
		{
			CreateGizmoSprite();
		}

		// Add to light manager.
		{
			LightManager::GetInstance().AddLight(shared_from_this());
		}
	}

	void Light::OnUpdate([[maybe_unused]] float deltaTime)
	{
		// Update gizmo sprite.
		if (_gizmoSprite)
		{
			_gizmoSprite->Update(GetParent()->transform.world, _lightColor, GetParent()->GetEntityID());
		}
	}

	void Light::OnEvent([[maybe_unused]] Event* e)
	{
	}

	void Light::OnGui()
	{
		ImGui::ColorEdit4("LightColor##Light", &_lightColor.color.x);

		if (ImGui::CollapsingHeader("ShadowSettings##Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			GuiUtils::ComboBoxGui("ShadowType##Light", SHADOW_TYPES.data(), static_cast<int>(SHADOW_TYPES.size()), _shadowTypeString);
		}
	}

	void Light::OnPrefabGui()
	{
		ImGui::ColorEdit4("LightColor##Light", &_lightColor.color.x);

		if (ImGui::CollapsingHeader("ShadowSettings##Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			GuiUtils::ComboBoxGui("ShadowType##Light", SHADOW_TYPES.data(), static_cast<int>(SHADOW_TYPES.size()), _shadowTypeString);
		}
	}

	void Light::OnSerialized() const
	{
	}

	void Light::OnDeserialized()
	{
		CreateGizmoSprite();
	}

	void Light::OnDestroy()
	{
		Component::OnDestroy();

		// Remove from light manager.
		{
			LightManager::GetInstance().RemoveLight(shared_from_this());
		}
	}

	void Light::OnGameObjectDeserialized()
	{
	}

	void Light::OnGizmoRender()
	{
		if (_gizmoSprite)
		{
			_gizmoSprite->Render();
		}
	}
}
