#pragma once

namespace DentyEngine
{
	class Window;
	class WindowInfo final
	{
	public:
		WindowInfo() : _hwnd(), _hInstance(), _name() {}
		~WindowInfo() = default;

		[[nodiscard]]
		HWND GetHwnd() const { return _hwnd; }

		[[nodiscard]]
		HINSTANCE GetHInstance() const { return _hInstance; }

		[[nodiscard]]
		const std::string& GetName() const { return _name; }

		template <class T>
		[[nodiscard]]
		T GetSize() const;
	private:
	    HWND _hwnd;
		HINSTANCE _hInstance;
		
		std::string _name;

		friend class Window;
	};

	class Event;
	class Window final
	{
	public:
		Window() = default;
		~Window() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(Window)

		static void Create(
			std::string_view applicationName,
			HINSTANCE hInstance,
			const Vector2& size
		);

		static void OnUpdate();

		static void OnGui();

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		static const WindowInfo& GetInfo() { return _windowInfo; }
	private:
		static WindowInfo _windowInfo;
		static bool _created;
	};
};