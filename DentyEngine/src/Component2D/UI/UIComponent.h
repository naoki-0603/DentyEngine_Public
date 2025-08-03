#pragma once

#include "Component/Behavior.h"
#include "Component/ComponentGenerator.h"

#include "UI/Items/Text.h"
#include "UI/Items/Image.h"
#include "UI/Items/Button.h"

namespace DentyEngine
{
	struct UIComponentSerializeData
	{
	public:
		UIComponentSerializeData();

	public:
		UITextData text;
		UIImageData image;

		std::string uiType;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(text),
					CEREAL_NVP(image),
					CEREAL_NVP(uiType)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(text),
					CEREAL_NVP(image),
					CEREAL_NVP(uiType)
				);
			}
		}
	};

	class UIComponent final : public Behavior, public std::enable_shared_from_this<UIComponent>
	{
	public:
		UIComponent();
		UIComponent(const UIComponent& source);
		~UIComponent() override = default;

		void Awake() override;
		void Start() override;
		void OnUpdate(float deltaTime) override;

		void OnFixedUpdate() override;
		void OnGui() override;
		void OnPrefabGui() override;

		void OnEvent(Event* e) override;
		void OnSerialized() const override;
		void OnDeserialized() override;
		void OnDestroy() override;
		void OnGameObjectDeserialized() override;

		void OnSceneActivated();
		void OnSceneDeActivated();
		void OnRuntimeSceneChanged();

		// Called on GameObject::CreateUI functions.
		void Setup(Ref<UserInterfaceItem> item);

		[[nodiscard]]
		Ref<Object> MakeCopy() const override;

		[[nodiscard]]
		const Ref<UserInterfaceItem>& GetItem() const;

		DENTY_REGISTER_BUILD_IN_COMPONENT(UIComponent)
	private:
		void Restore();
		void RestoreText();
		void RestoreImage();
		void RestoreButton();

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Behavior>(this),
					CEREAL_NVP(_item)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Behavior>(this),
					CEREAL_NVP(_item)
				);
			}
		}
	private:
		Ref<UserInterfaceItem> _item;
	};	
}

CEREAL_CLASS_VERSION(DentyEngine::UIComponent, 1u)
CEREAL_CLASS_VERSION(DentyEngine::UIComponentSerializeData, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::UIComponent)