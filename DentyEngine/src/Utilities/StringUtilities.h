#pragma once

namespace DentyEngine
{
	namespace StringUtils
	{
        //
        // Insert functions.
        //
        [[nodiscard]]
        extern std::string InsertBefore(std::string_view source, std::string_view insert);

        [[nodiscard]]
		extern std::string InsertAfter(std::string_view source, std::string_view insert);

        //
        // Remove functions.
        //
        [[nodiscard]]
        extern std::string RemoveExtension(std::string_view source, std::string_view extension);

        [[nodiscard]]
		extern std::string RemoveUntil(std::string_view source, std::string_view target);

        //
        // Convert functions.
        //
        [[nodiscard]]
		extern std::wstring StringToWString(const std::string& str);

        [[nodiscard]]
		extern std::string WStringToString(const std::wstring& wstr);

        [[nodiscard]]
		extern std::string ExtractFileNameFromPath(const std::string& filePath);

        [[nodiscard]]
        extern std::string ToValidFileName(std::string_view fileName);

        [[nodiscard]]
        extern std::string ReplaceAll(std::string_view source, std::string_view target, std::string_view replace);

        extern bool StringContains(const std::string& findSource, std::string_view findTarget);
	}
};