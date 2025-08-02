#pragma once

namespace DentyEngine
{
	// https://www.youtube.com/watch?v=zn7N7zHgCcs&t=431s
	class FileDialogs final
	{
	public:
		FileDialogs() = default;
		~FileDialogs() = default;

		// Return empty strings if cancelled
		[[nodiscard]]
		static std::string OpenFile(const char* filter);

		[[nodiscard]]
		static std::string SaveFile(const char* filter);
	public:
		static const char* SCENE_FILTER;
		static const char* TEXTURE_FILTER;
		static const char* MATERIAL_FILTER;
	};
}