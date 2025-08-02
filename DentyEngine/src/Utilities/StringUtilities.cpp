#include "stdafx.h"

#include "StringUtilities.h"

#include <regex>

namespace DentyEngine
{
    namespace StringUtils
    {
        std::string InsertBefore(std::string_view source, std::string_view insert)
        {
            return std::string(insert) + std::string(source);
        }

        std::string InsertAfter(std::string_view source, std::string_view insert)
        {
            return std::string(source) + std::string(insert);
        }

        std::string RemoveExtension(std::string_view source, std::string_view extension)
        {
            std::string sourceString(source);

            if (StringContains(sourceString, extension))
            {
                std::filesystem::path path(sourceString);
                sourceString = path.stem().string();
            }

            return sourceString;
        }

        std::string RemoveUntil(std::string_view source, std::string_view target)
        {
            const auto s = std::string(source);
            const auto t = std::string(target);

            return s.substr(s.find(t));
        }

        std::wstring StringToWString(const std::string& str)
        {
            _bstr_t bstr(str.c_str());

            return std::wstring(bstr);
        }

        std::string WStringToString(const std::wstring& wstr)
        {
            _bstr_t bstr(wstr.c_str());

            return std::string(bstr);
        }

        std::string ExtractFileNameFromPath(const std::string& filePath)
        {
            const std::filesystem::path path(filePath);

            return path.stem().string();
        }

        std::string ToValidFileName(std::string_view fileName)
        {
            auto name = std::string(fileName);

            // Check is contain ":".
            if (StringUtils::StringContains(name, ":"))
            {
                // Replace ":" to "_"
                name = StringUtils::ReplaceAll(name, ":", "_");
            }

            return name;
        }

        // https://marycore.jp/prog/cpp/std-regex-replace-first-all/
        std::string ReplaceAll(std::string_view source, std::string_view target, std::string_view replace)
        {
            auto sourceString = std::string(source);
            auto targetString = std::string(target);
            auto replaceString = std::string(replace);

            auto regexStr = std::string("(") + targetString + ")";

            std::string convertedString = std::regex_replace(
                sourceString,
                std::regex(regexStr),
                replaceString
            );

            return convertedString;
        }

        bool StringContains(const std::string& findSource, std::string_view findTarget)
        {
            return (findSource.find(findTarget) != std::string::npos);
        }
    }
};