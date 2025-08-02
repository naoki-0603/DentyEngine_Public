#include "stdafx.h"

#include "Utilities.h"

#include "System/Core/Window.h"

namespace DentyEngine
{
	void ShowErrorMessageBox()
	{
		const DWORD errorCode = GetLastError();
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, errorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
			reinterpret_cast<LPTSTR>(&lpMsgBuf),
			0,
			nullptr
		);

		MessageBox(Window::GetInfo().GetHwnd(), static_cast<LPCTSTR>(lpMsgBuf), _TEXT("Error"), MB_OK | MB_ICONINFORMATION);
	}
};