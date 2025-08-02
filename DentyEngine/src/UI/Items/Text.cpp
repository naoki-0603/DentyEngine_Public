#include "stdafx.h"

#include "Text.h"

#include "System/Graphics/GraphicsManager.h"

#include <directxtk/include/directxtk/SpriteFont.h>
#include <directxtk/include/directxtk/SpriteBatch.h>

namespace DentyEngine
{
	UITextData::UITextData() :
		spacing(0.0f), text("Sample"), textColor(Color::WHITE)
	{

	}

	const std::wstring UIText::DEFAULT_SPRITE_FONT_FILE_PATH = L"Assets\\SpriteFont\\RussoOne-Regular.spritefont";

	UIText::UIText() :
		UserInterfaceItem(), Object(), _font(), _data()
	{
		_name = "UIText";
	}

	void UIText::Create()
	{
		Initialize();
	}

	void UIText::Render()
	{
		const Vector2 origin = transform.pivot.GetPivotOffset();

		_batch->Begin();
		_font->DrawString(_batch.get(), _data.text.c_str(), transform.position, _data.textColor.color.GetXM(), transform.rotation, origin, transform.scale);
		_batch->End();
	}

	void UIText::OnGui()
	{
		{
			ImGui::TextWrapped("Text");
			ImGui::SameLine();

			String::InputTextGui("##InputTextUIText", &_data.text);
		}

		{
			ImGui::TextWrapped("ColorEdit");
			ImGui::SameLine();
			ImGui::ColorEdit4("##ColorEditUIText", &_data.textColor.color.x);
		}
		
		ImGui::Separator();

		UserInterfaceItem::OnGui();
	}

	void UIText::OnUpdate()
	{
		
	}

	void UIText::OnSerialized() const
	{
		Object::OnSerialized();
	}

	void UIText::OnDeserialized()
	{
		Object::OnDeserialized();
	}

	void UIText::OnSceneActivated()
	{
		Initialize();
	}

	void UIText::OnSceneDeActivated()
	{

	}

	void UIText::OnRuntimeSceneChanged()
	{

	}

	void UIText::SetText(std::string_view text)
	{
		_data.text = text;
	}

	void UIText::SetLineSpacing(float spacing)
	{
		_data.spacing = spacing;

		_font->SetLineSpacing(spacing);
	}

	void UIText::SetTextColor(const Color& color)
	{
		_data.textColor = color;
	}

	const std::string& UIText::GetText() const
	{
		return _data.text;
	}

	float UIText::GetSpacing() const
	{
		return _data.spacing;
	}

	const Color& UIText::GetTextColor() const
	{
		return _data.textColor;
	}

	const char* UIText::GetItemType() const
	{
		return "Text";
	}

	Vector2 UIText::GetSize() const
	{
		const RECT rect = _font->MeasureDrawBounds(_data.text.c_str(), transform.position);

		return Vector2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));
	}

	void UIText::Initialize()
	{
		ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		_font = std::make_shared<DirectX::DX11::SpriteFont>(device, DEFAULT_SPRITE_FONT_FILE_PATH.c_str());
		_batch = std::make_shared<DirectX::DX11::SpriteBatch>(context);

		_font->SetLineSpacing(_data.spacing);
	}
}
