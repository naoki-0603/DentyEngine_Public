#pragma once

#include <string>

#include "DFilePath.h"

namespace DentyEngine
{
	// Wrapper class for std::string.
	class String final
	{
	public:
		String() noexcept;
		String(const std::string&) noexcept;
		String(std::string_view) noexcept;
		String(const char*) noexcept;

		String(const String&);
		~String() = default;

		//
		// Member functions.
		//
		void Append(const char*);
		void Append(const String&);
		
		void InsertAt(const String&, uint32_t);
		void InsertBefore(const String&);

		void RemoveExtension();

		void RemoveUntil(const String&);
		void RemoveUntil(uint32_t index);

		[[nodiscard]]
		std::optional<String> ClipUntil(uint32_t index) const;

		[[nodiscard]]
		String SubStr(uint32_t index) const;

		[[nodiscard]]
		uint32_t FindFirstOf(const String&) const noexcept;

		[[nodiscard]]
		uint32_t FindLastOf(const String&) const noexcept;

		[[nodiscard]]
		bool Contains(const String&) const noexcept;

		[[nodiscard]]
		bool Equal(const String&) const noexcept;

		[[nodiscard]]
		constexpr const std::string& Get() const noexcept { return _string; }

		[[nodiscard]]
		constexpr const void* GetData() const noexcept { return _string.data(); }

		[[nodiscard]]
		constexpr const char* CStr() const noexcept { return _string.c_str(); }

		[[nodiscard]]
		constexpr uint64_t Length() const noexcept { return static_cast<uint64_t>(_string.length()); }

		[[nodiscard]]
		constexpr bool IsEmpty() const noexcept { return _string.empty(); }

		//
		// operators.
		//
		[[nodiscard]]
		String operator+(const String&) const;

		friend std::ostream& operator<<(std::ostream& os, const String& r);

		//
		// Static functions.
		//
		[[nodiscard]]
		static FilePath ToPath(const String& str)
		{
			return FilePath(str._string);
		}

		// returns case-insensitive comparison results
		[[nodiscard]]
		static bool StrICmp(const String&, const String&);

#if DENTY_USE_IMGUI
		static bool InputTextGui(const char* label, std::string* text, ImGuiInputTextFlags flags = 0);
		static void InputTextGui(const char* label, String* text, ImGuiInputTextFlags flags = 0);

		static void TextCentered(const std::string& text);
		static void TextCentered(const std::string& text, const Vector2& windowSize);
#endif
	private:
		std::string _string;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_string)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(_string)
			);
		}
	};
}
CEREAL_CLASS_VERSION(DentyEngine::String, 1u)