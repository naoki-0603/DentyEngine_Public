#pragma once

#include "System/Editor/DragAndDrop/DragAndDrop.h"
#include "System/UI/UserInterfaceItem.h"
#include "System/UI/ISelectableUserInterfaceItem.h"

namespace DirectX
{
	inline namespace DX11
	{
		class SpriteBatch;
	}
}

namespace DentyEngine
{
	class Texture2D;

	struct UIButtonData
	{
	public:
		UIButtonData();

	public:
		FilePath textureFilePathToSerialized;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(textureFilePathToSerialized)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(textureFilePathToSerialized)
				);
			}
		}
	};

	class UIButton final : public UserInterfaceItem, public ISelectableUserInterfaceItem, public Object
	{
	public:
		UIButton();
		~UIButton() override = default;

		void Create();
		void Render() override;

		void OnGui() override;
		void OnUpdate() override;

		void OnSerialized() const override;;
		void OnDeserialized() override;

		void OnSceneActivated() override;
		void OnSceneDeActivated() override;
		void OnRuntimeSceneChanged() override;

		void OnSelected() override;

		[[nodiscard]]
		const char* GetItemType() const override;

		[[nodiscard]]
		Vector2 GetSize() const override;
	private:
		void Initialize();

		//
		// Callback function.
		//
		void TextureAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData = nullptr);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Object>(this),
					cereal::base_class<UserInterfaceItem>(this)
				);
			}

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Object>(this),
					cereal::base_class<UserInterfaceItem>(this)
				);
			}

			OnDeserialized();
		}
	private:
		Ref<DirectX::DX11::SpriteBatch> _spriteBatch;
		Ref<Texture2D> _texture;

		UIButtonData _data;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::UIButton, 1u)
CEREAL_CLASS_VERSION(DentyEngine::UIButtonData, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::UIButton)