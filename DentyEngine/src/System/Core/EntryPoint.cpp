#include "stdafx.h"

#include "Program.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
#if defined(DENTY_DEBUG) || defined(DENTY_RELEASE)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	{
		const auto program = DentyEngine::Program::Create();
		program->Launch("DentyEngine", hInstance, DentyEngine::Vector2(1900.0f, 1000.0f));
		program->Run();
		program->Terminate();

	    delete program;
	}

	return 0;
}