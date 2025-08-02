#include <stdafx.h>

#include "DString.h"

#include <imgui/misc/cpp/imgui_stdlib.h>

namespace DentyEngine
{
	String::String() noexcept :
		_string()
	{
	}

	String::String(const std::string& str) noexcept :
		_string(str)
	{
	}

	String::String(std::string_view str) noexcept :
		_string(str)
	{
	}

	String::String(const char* str) noexcept :
		_string(str)
	{
	}

	String::String(const String& source) :
		_string(source._string)
	{
	}

    void String::Append(const char* str)
    {
		_string.append(str);
    }

	void String::Append(const String& str)
	{
		_string.append(str._string);
	}

	void String::InsertAt(const String& str, uint32_t index)
	{
		const auto length = Length();
		if (index > length)
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Index overflow! " << "(in String::RemoveUntil)";
			return;
		}

		_string = _string.at(index) + str._string;
	}

	void String::InsertBefore(const String& str)
	{
		const auto s = str + _string;
		_string = s._string;
	}

	void String::RemoveExtension()
	{
		auto path = ToPath(_string);
		path.ReplaceExtension("");

		_string = path.GetAsString();
	}

	void String::RemoveUntil(const String& str)
	{
		_string = _string.substr(_string.find(str._string));
	}

	void String::RemoveUntil(uint32_t index)
	{
		const auto length = Length();
		if (index > length)
		{
			DENTY_SET_ERR_COLOR

			DENTY_ERR_LOG << "Index overflow! " << "(in String::RemoveUntil)";
			return;
		}

		_string = _string.substr(index);
	}

	std::optional<String> String::ClipUntil(uint32_t index) const
	{
		const auto length = Length();
		if (index > length)
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Index overflow! " << "(in String::RemoveUntil)";

			return std::nullopt;
		}

		std::string result = { };
		for (uint32_t i = 0; i < index; ++i)
		{
			result += _string.at(i);
		}

		return String(result);
	}

	String String::SubStr(uint32_t index) const
	{
		String string = {};
		string = _string.substr(index);

		return string;
	}

	uint32_t String::FindFirstOf(const String& str) const noexcept
	{
		const auto index = static_cast<uint32_t>(_string.find_first_of(str._string));

		return index;
	}

	uint32_t String::FindLastOf(const String& str) const noexcept
	{
		const auto index = static_cast<uint32_t>(_string.find_last_of(str._string));

		return index;
	}

    bool String::Contains(const String& str) const noexcept
    {
		return (_string.find(str._string) != std::string::npos);
    }

    bool String::Equal(const String& str) const noexcept
    {
		return (_string == str._string);
    }

    String String::operator+(const String& str) const
	{
		return this->_string + str._string;
	}

    bool String::StrICmp(const String& lhs, const String& rhs)
    {
		return (_stricmp(lhs.CStr(), rhs.CStr()) == 0);
    }

	bool String::InputTextGui(const char* label, std::string* text, ImGuiInputTextFlags flags)
	{
		return ImGui::InputText(label, text, flags);
	}

	void String::InputTextGui(const char* label, String* text, ImGuiInputTextFlags flags)
	{
		ImGui::InputText(label, &text->_string, flags);
	}

	// https://stackoverflow.com/questions/64653747/how-to-center-align-text-horizontally
	void String::TextCentered(const std::string& text)
	{
		const auto windowWidth = ImGui::GetWindowSize().x;
		const auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}

	void String::TextCentered(const std::string& text, const Vector2& windowSize)
	{
		const auto windowWidth = windowSize.x;
		const auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}

	std::ostream& operator<<(std::ostream& os, const String& r)
    {
		os << r.Get();

		return os;
    }
}
