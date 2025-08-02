#include "stdafx.h"

#include "FileDialogs.h"

#include "System/Core/Window.h"

#include <commdlg.h>

namespace DentyEngine
{
	const char* FileDialogs::SCENE_FILTER = ("DentyScene(*.DentyScene)\0*.DentyScene\0");
	const char* FileDialogs::TEXTURE_FILTER = ("DentyTexture(*.DentyTexture)\0*.DentyTexture\0");
	const char* FileDialogs::MATERIAL_FILTER = ("DentyMaterial(*.DentyMaterial)\0*.DentyMaterial\0");

	std::string FileDialogs::OpenFile(const char* filter)
	{
		std::array<char, 260> buffer = {};
		OPENFILENAMEA openFileNameA = {};
		openFileNameA.lStructSize = sizeof(OPENFILENAMEA);
		openFileNameA.hwndOwner = Window::GetInfo().GetHwnd();
		openFileNameA.lpstrFilter = filter;
		openFileNameA.nMaxFile = static_cast<DWORD>(buffer.size());
		openFileNameA.lpstrFile = buffer.data();
		openFileNameA.nFilterIndex = 1;
		openFileNameA.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&openFileNameA) == TRUE)
		{
			return openFileNameA.lpstrFile;
		}

		return {};
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
		std::array<char, 260> buffer = {};
		OPENFILENAMEA openFileNameA = {};
		openFileNameA.lStructSize = sizeof(OPENFILENAMEA);
		openFileNameA.hwndOwner = Window::GetInfo().GetHwnd();
		openFileNameA.lpstrFilter = filter;
		openFileNameA.nMaxFile = static_cast<DWORD>(buffer.size());
		openFileNameA.lpstrFile = buffer.data();
		openFileNameA.nFilterIndex = 1;
		openFileNameA.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&openFileNameA) == TRUE)
		{
			return openFileNameA.lpstrFile;
		}

		return {};
	}
}