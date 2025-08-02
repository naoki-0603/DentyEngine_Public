#pragma once

namespace DentyEngine
{
    // Copy from imgui_demo.cpp
    struct ExampleAppConsole
    {
        char                  InputBuf[256];
        ImVector<char*>       Items;
        ImVector<const char*> Commands;
        ImVector<char*>       History;
        int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter       Filter;
        bool                  AutoScroll;
        bool                  ScrollToBottom;

        ExampleAppConsole();
        ~ExampleAppConsole();

        // Portable helpers
        static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
        static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
        static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
        static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

        void    ClearLog();
        void    AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void    Draw(const char* title, bool* p_open);
        void    ExecCommand(const char* command_line);

        // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
        {
            ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
            return console->TextEditCallback(data);
        }

        int     TextEditCallback(ImGuiInputTextCallbackData* data);
    };

	class ImGuiConsole final
	{
	public:
		ImGuiConsole() = default;
		~ImGuiConsole() = default;

		static void AddDebugLog(const char* format...);
		static void AddTaskLog(const char* format...);
		static void AddInfoLog(const char* format...);
		static void AddWarnLog(const char* format...);
		static void AddErrLog(const char* format...);

		static void OnGui();

		static void Clear();
	private:
		static ExampleAppConsole _console;

		static const char* CONSOLE_TITLE;
	};
};

#define DENTY_DEBUG_CONSOLE_LOG(...) ImGuiConsole::AddDebugLog(__VA_ARGS__)
#define DENTY_TASK_CONSOLE_LOG(...)  ImGuiConsole::AddTaskLog(__VA_ARGS__)
#define DENTY_INFO_CONSOLE_LOG(...)  ImGuiConsole::AddInfoLog(__VA_ARGS__)
#define DENTY_WARN_CONSOLE_LOG(...)  ImGuiConsole::AddWarnLog(__VA_ARGS__)
#define DENTY_ERR_CONSOLE_LOG(...)   ImGuiConsole::AddErrLog(__VA_ARGS__)