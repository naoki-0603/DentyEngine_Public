#include "stdafx.h"

#include "ConsoleLogger.h"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Initializers/ConsoleInitializer.h"

namespace DentyEngine
{
	plog::ColorConsoleAppender<plog::TxtFormatter> ConsoleLogger::_consoleAppender = { };

	// http://www.orangemaker.sakura.ne.jp/labo/memo/sdk-mfc/
	// http://www.kumei.ne.jp/c_lang/intro/no_58.htm
	void ConsoleLogger::OnLaunch()
	{
		// Alloc console.
		{
			AllocConsole();

			FILE* fp = nullptr;
			freopen_s(&fp, "CONOUT$", "w", stdout);
			freopen_s(&fp, "CONIN$", "r", stdin);
		}

		// Initialize log library.
		{
			plog::init(plog::debug, &_consoleAppender);
		}
	}

	void ConsoleLogger::OnTerminate()
	{
		// Terminate console.
		{
			FreeConsole();
		}
	}

	const WORD ConsoleColor::RED = FOREGROUND_RED;
	const WORD ConsoleColor::GREEN = FOREGROUND_GREEN;
	const WORD ConsoleColor::BLUE = FOREGROUND_BLUE;
	const WORD ConsoleColor::WHITE = RED | GREEN | BLUE;

	const WORD ConsoleColor::DEBUG_COLOR = GREEN;
	const WORD ConsoleColor::INFO_COLOR = GREEN;
	const WORD ConsoleColor::WARN_COLOR = RED | GREEN;
	const WORD ConsoleColor::ERROR_COLOR = RED;
	const WORD ConsoleColor::FATAL_COLOR = WHITE | BACKGROUND_RED;

	ConsoleColor::ConsoleColor(WORD color)
	{
		const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, color);
	}

	ConsoleColor::~ConsoleColor()
	{
		const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, WHITE);
	}
}
