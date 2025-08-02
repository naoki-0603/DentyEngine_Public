#include "stdafx.h"

#include "Window.h"

#include "System/Events/EventWindow.h"
#include "System/Events/EventMouse.h"

#include "Utilities/StringUtilities.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DentyEngine
{
	WindowInfo Window::_windowInfo = {};
	bool Window::_created = false;

	template <>
	Vector2I WindowInfo::GetSize() const
	{
		RECT rect = {};
		GetClientRect(_hwnd, &rect);

		return Vector2I(rect.right, rect.bottom);
	}

	template <>
	Vector2 WindowInfo::GetSize() const
	{
		RECT rect = {};
		GetClientRect(_hwnd, &rect);

		return Vector2(static_cast<float>(rect.right), static_cast<float>(rect.bottom));
	}

	void Window::Create(std::string_view applicationName, const HINSTANCE hInstance, const Vector2& size)
	{
		DENTY_ASSERT(!_created, "Already created.");

		const std::wstring name = StringUtils::StringToWString(std::string(applicationName));

		_windowInfo._hInstance = hInstance;
		_windowInfo._name = applicationName;

		const auto hicon = static_cast<HICON>(LoadImage(nullptr, L"Assets/Logo/icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
		const auto hiconSmall = static_cast<HICON>(LoadImage(nullptr, L"Assets/Logo/small.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));

		WNDCLASSEX windowClass = {};
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = hInstance;
		windowClass.hIcon = hicon;
		windowClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = name.c_str();
		windowClass.hIconSm = hiconSmall;

		// Register wnd class structure
		if (!RegisterClassEx(&windowClass))
		{
			DENTY_ASSERT(false, "Failed to register WNDCLASSEX structure.");
		}

		// Create window
		_windowInfo._hwnd = CreateWindowEx(
			0,                                      // Window style
			name.c_str(),							// Register window name
			name.c_str(),                           // Window title name
			(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),  // Window style
			CW_USEDEFAULT,                          // Display position
			CW_USEDEFAULT,                          // Display position
			static_cast<int>(size.x),									// Screen size width
			static_cast<int>(size.y),									// Screen size height
			nullptr,                                // Parent handle
			nullptr,                                // Menu handle
			hInstance,								// Window handle
			nullptr                                 // Value to be passed to lpparam in WM_CREATE message
		);

		// If not able to create window
		if (_windowInfo._hwnd == nullptr)
		{
			const DWORD errorCode = GetLastError();
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPTSTR>(&lpMsgBuf),
				0,
				nullptr
			);

			MessageBox(_windowInfo._hwnd, static_cast<LPCTSTR>(lpMsgBuf), _TEXT("Error"), MB_OK | MB_ICONINFORMATION);
			LocalFree(_windowInfo._hwnd);

			DENTY_ASSERT(false, "Failed to create window.");
		}

		RECT windowRect = {};

		// Get window size
		if (!GetWindowRect(_windowInfo._hwnd, &windowRect))
		{
			DENTY_ASSERT(false, "Failed to get window size");
		}

		RECT clientRect = {};

		// Get client area
		if (!GetClientRect(_windowInfo._hwnd, &clientRect))
		{
			DENTY_ASSERT(false, "Failed to get client size");
		}

		// Calc frame size
		const int frameSizeX = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
		const int frameSizeY = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);

		// Calc frame size for resize
		const int resizeWidth = static_cast<int>(frameSizeX + size.x);
		const int resizeHeight = static_cast<int>(frameSizeY + size.y);

		// Update window size
		SetWindowPos(
			_windowInfo._hwnd,
			nullptr,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			resizeWidth,
			resizeHeight,
			SWP_NOMOVE
		);

		// Display window
		ShowWindow(_windowInfo._hwnd, SW_SHOWMAXIMIZED | SW_MAXIMIZE);

		_created = true;
	}

	void Window::OnUpdate()
	{
		MSG message = {};

		while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	void Window::OnGui()
	{
		if (ImGui::CollapsingHeader("Window##CoreLayer"))
		{
			const auto size = GetInfo().GetSize<Vector2I>();
			ImGui::Text("WindowSize: %d, %d", size.x, size.y);
		}
	}

	LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

		// switching by message
		switch (msg)
		{
		case WM_DESTROY:
			{
				PostQuitMessage(0);

				EventWindowClosed closed = {};
				NOTIFY_EVENT(closed);
			}
			break;
		case WM_CREATE:
			break;
		case WM_ENTERSIZEMOVE:
			break;
		case WM_EXITSIZEMOVE:
			break;
		case WM_PAINT:
			break;
		case WM_KEYDOWN:
			break;
			//
			// Mouse events.
			//

			// Mouse release event.
		case WM_LBUTTONUP:
			{
				EventMouseButtonReleased mouseButtonReleased(static_cast<std::byte>(VK_LBUTTON));
				NOTIFY_EVENT(mouseButtonReleased);
			}
			break;
		case WM_MBUTTONUP:
			{
				EventMouseButtonReleased mouseButtonReleased(static_cast<std::byte>(VK_MBUTTON));
				NOTIFY_EVENT(mouseButtonReleased);
			}
			break;
		case WM_RBUTTONUP:
			{
				EventMouseButtonReleased mouseButtonReleased(static_cast<std::byte>(VK_RBUTTON));
				NOTIFY_EVENT(mouseButtonReleased);
			}
			break;
		case WM_MOUSEWHEEL:
			{
				const auto wheelSpeed = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA);

				Input::SetMouseWheelSpeed(wheelSpeed);

				EventMouseScrolled mouseScrolled(wheelSpeed);
				NOTIFY_EVENT(mouseScrolled);
			}
			break;
		default:
			break;
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}