#include "StdAfx.h"
#include "Misc.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR CmdLine, int CmdShow)
{
#if defined(_MSC_VER) && defined(_DEBUG)
	// ¼ì²é Memory Leak
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	if (Application == 0) {
		TRACE("CWinApp »¹Ã»µÇÂ¼×¢²á\n");
		return -1;
	}

	Application->SetWinMainArgs(hInstance, CmdLine, CmdShow);

	if (!Application->InitInstance())
		return -1;

	return Application->Run();
}
