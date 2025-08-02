#pragma once

#include "System/Core/Object.h"
#include "Graphics/Color.h"

#include "System/UI/UserInterfaceItem.h"

namespace DirectX
{
	inline namespace DX11
	{
		class SpriteFont;
		class SpriteBatch;
	}
}

namespace DentyEngine
{
	struct UITextData
	{
	public:
		UITextData();

	public:
		float spacing;

		std::string text;

		Color textColor;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(spacing),
					CEREAL_NVP(text),
					CEREAL_NVP(textColor)
				);
			}
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					CEREAL_NVP(spacing),
					CEREAL_NVP(text),
					CEREAL_NVP(textColor)
				);
			}
		}
	};

	class UIText final : public UserInterfaceItem, public Object
	{
	public:
		UIText();
		~UIText() override = default;

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
		void SetText(std::string_view text);
		void SetLineSpacing(float spacing);
		void SetTextColor(const Color& color);

		//
		// Getter
		//
		[[nodiscard]]
		const std::string& GetText() const;

		[[nodiscard]]
		float GetSpacing() const;

		[[nodiscard]]
		const Color& GetTextColor() const;

		[[nodiscard]]
		const char* GetItemType() const override;

		[[nodiscard]]
		Vector2 GetSize() const override;
	private:
		void Initialize();

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
		Ref<DirectX::DX11::SpriteFont> _font;
		Ref<DirectX::DX11::SpriteBatch> _batch;
		
		UITextData _data;
	private:
		static const std::wstring DEFAULT_SPRITE_FONT_FILE_PATH;

	};
}

CEREAL_CLASS_VERSION(DentyEngine::UIText, 1u)
CEREAL_CLASS_VERSION(DentyEngine::UITextData, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::UIText)