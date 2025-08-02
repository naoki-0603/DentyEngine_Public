#pragma once

#include "Graphics/States/BlendState.h"
#include "System/Core/ID/TextureID.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

#include "System/UI/UserInterfaceItem.h"

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

	struct Rect
	{
	public:
		Rect();
		Rect(int32_t left, int32_t top, int32_t right, int32_t bottom);

	public:
		int32_t left;
		int32_t top;
		int32_t right;
		int32_t bottom;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(left),
					CEREAL_NVP(top),
					CEREAL_NVP(right),
					CEREAL_NVP(bottom)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(left),
					CEREAL_NVP(top),
					CEREAL_NVP(right),
					CEREAL_NVP(bottom)
				);
			}
		}
	};

	struct UIImageData
	{
	public:
		UIImageData();

	public:
		Rect clipRect;

		FilePath textureFilePathToSerialized;

		BlendState::Type blendType;

		bool useClipRect;
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
			else if (version <= 2u)
			{
				archive(
					CEREAL_NVP(clipRect),
					CEREAL_NVP(textureFilePathToSerialized),
					CEREAL_NVP(blendType),
					CEREAL_NVP(useClipRect)
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
			else if (version <= 2u)
			{
				archive(
					CEREAL_NVP(clipRect),
					CEREAL_NVP(textureFilePathToSerialized),
					CEREAL_NVP(blendType),
					CEREAL_NVP(useClipRect)
				);
			}
		}
	};

	class UIImage final : public UserInterfaceItem, public Object
	{
	public:
		UIImage();
		~UIImage() override = default;

		void Create();
		void Render() override;

		void OnGui() override;
		void OnUpdate() override;

		void OnSerialized() const override;;
		void OnDeserialized() override;

		void OnSceneActivated() override;
		void OnSceneDeActivated() override;
		void OnRuntimeSceneChanged() override;

		//
		// Setter
		//
		void SetTexture(Ref<Texture2D> texture);
		void SetClipPosition(int32_t left, int32_t top, int32_t right, int32_t bottom);

		//
		// Getter
		//
		[[nodiscard]]
		const char* GetItemType() const override;

		[[nodiscard]]
		Vector2 GetSize() const override;

		[[nodiscard]]
		Vector4I GetClipPosition() const;

		[[nodiscard]]
		Vector2 GetCenterOfImage() const;

		[[nodiscard]]
		const FilePath& GetTextureFilePath() const;
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
					cereal::base_class<UserInterfaceItem>(this),
					CEREAL_NVP(_data)
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
					cereal::base_class<UserInterfaceItem>(this),
					CEREAL_NVP(_data)
				);
			}

			OnDeserialized();
		}
	private:
		Ref<DirectX::DX11::SpriteBatch> _spriteBatch;
		Ref<Texture2D> _texture;

		UIImageData _data;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::UIImage, 1u)
CEREAL_CLASS_VERSION(DentyEngine::UIImageData, 2u)
CEREAL_CLASS_VERSION(DentyEngine::Rect, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::UIImage)