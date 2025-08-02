#pragma once

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

namespace DentyEngine
{
	class ConsoleLogger final
	{
	public:
		DEFINE_SINGLETON_CLASS(ConsoleLogger)

		void OnLaunch();
		void OnTerminate();

	private:
		static plog::ColorConsoleAppender<plog::TxtFormatter> _consoleAppender;
	};

	class ConsoleColor final
	{
	public:
		ConsoleColor() = delete;
		ConsoleColor(WORD color);
		~ConsoleColor();

		DELETE_COPY_MOVE_CONSTRUCTORS(ConsoleColor)
	public:
		static const WORD RED;
		static const WORD GREEN;
		static const WORD BLUE;
		static const WORD WHITE;

		static const WORD DEBUG_COLOR;
		static const WORD INFO_COLOR;
		static const WORD WARN_COLOR;
		static const WORD ERROR_COLOR;
		static const WORD FATAL_COLOR;
	};
}

#ifdef _DEBUG
#define DENTY_SET_DEBUG_COLOR ConsoleColor color(ConsoleColor::DEBUG_COLOR);
#define DENTY_SET_INFO_COLOR  ConsoleColor color(ConsoleColor::INFO_COLOR);
#define DENTY_SET_WARN_COLOR  ConsoleColor color(ConsoleColor::WARN_COLOR);
#define DENTY_SET_ERR_COLOR   ConsoleColor color(ConsoleColor::ERROR_COLOR);
#define DENTY_SET_FATAL_COLOR ConsoleColor color(ConsoleColor::FATAL_COLOR);

#define DENTY_DEBUG_LOG LOGD
#define DENTY_INFO_LOG  LOGI
#define DENTY_WARN_LOG  LOGW
#define DENTY_ERR_LOG	LOGE
#define DENTY_FATAL_LOG LOGF
#else
#define DENTY_SET_DEBUG_COLOR ConsoleColor color(ConsoleColor::DEBUG_COLOR);
#define DENTY_SET_INFO_COLOR  ConsoleColor color(ConsoleColor::INFO_COLOR);
#define DENTY_SET_WARN_COLOR  ConsoleColor color(ConsoleColor::WARN_COLOR);
#define DENTY_SET_ERR_COLOR   ConsoleColor color(ConsoleColor::ERROR_COLOR);
#define DENTY_SET_FATAL_COLOR ConsoleColor color(ConsoleColor::FATAL_COLOR);

#define DENTY_DEBUG_LOG LOGD
#define DENTY_INFO_LOG  LOGI
#define DENTY_WARN_LOG  LOGW
#define DENTY_ERR_LOG	LOGE
#define DENTY_FATAL_LOG LOGF
#endif