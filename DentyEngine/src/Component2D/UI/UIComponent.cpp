#include "stdafx.h"

#include "UIComponent.h"

#include "System/UI/UISystem.h"

namespace DentyEngine
{
	UIComponentSerializeData::UIComponentSerializeData() :
		text(), image(), uiType()
	{
	}

	UIComponent::UIComponent() :
		Behaviour(), _item()
	{
		_name = "UIComponent";
	}

	UIComponent::UIComponent(const UIComponent& source) :
		Behaviour(source), _item(source._item)
	{
	}

	void UIComponent::Awake()
	{
		Behaviour::Awake();
	}

	void UIComponent::Start()
	{
		Behaviour::Start();
	}

	void UIComponent::OnUpdate([[maybe_unused]] float deltaTime)
	{
		_item->OnUpdate();
	}

	void UIComponent::OnFixedUpdate()
	{
		Behaviour::OnFixedUpdate();
	}

	void UIComponent::OnGui()
	{
		Behaviour::OnGui();

		_item->OnGui();
	}

	void UIComponent::OnPrefabGui()
	{
		Behaviour::OnPrefabGui();
	}

	void UIComponent::OnEvent(Event* e)
	{
		Behaviour::OnEvent(e);
	}

	void UIComponent::OnSerialized() const
	{
		Behaviour::OnSerialized();
	}

	void UIComponent::OnDeserialized()
	{
		Behaviour::OnDeserialized();
	}

	void UIComponent::OnDestroy()
	{
		Behaviour::OnDestroy();

		if (_item)
		{
			UISystem::RemoveItem(_item);
		}
	}

	void UIComponent::OnGameObjectDeserialized()
	{
		Behaviour::OnGameObjectDeserialized();
	}

	void UIComponent::OnSceneActivated()
	{
		if (_item->GetItemType() == std::string("Text"))
		{
			std::static_pointer_cast<UIText>(_item)->Create();
		}
		else if (_item->GetItemType() == std::string("Image"))
		{
			std::static_pointer_cast<UIImage>(_item)->Create();
		}

		UISystem::AddItem(_item);
	}

	void UIComponent::OnSceneDeActivated()
	{
	}

	void UIComponent::OnRuntimeSceneChanged()
	{
	}

	void UIComponent::Setup(Ref<UserInterfaceItem> item)
	{
		_item = item;

		UISystem::AddItem(_item);
	}

	Ref<Object> UIComponent::MakeCopy() const
	{
		return std::make_shared<UIComponent>(*shared_from_this());
	}

	const Ref<UserInterfaceItem>& UIComponent::GetItem() const
	{
		return _item;
	}

	void UIComponent::Restore()
	{
	}

	void UIComponent::RestoreText()
	{
		/*const auto text = std::make_shared<UIText>();
		text->Create();

		text->SetText(_serializeData.text.text);
		text->SetLineSpacing(_serializeData.text.spacing);
		text->SetTextColor(_serializeData.text.textColor);

		Setup(text);*/
	}

	void UIComponent::RestoreImage()
	{
		/*const auto image = std::make_shared<UIImage>();
		image->Create();

		const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

		const std::optional<Ref<Texture2D>> texture2d = textureLibrary->Find<Texture2D>(_serializeData.image.textureFilePathToSerialized);
		if (texture2d.has_value())
		{
			image->SetTexture(texture2d.value());
		}

		Setup(image);*/
	}

	void UIComponent::RestoreButton()
	{
	}
}
