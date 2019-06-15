#include "stdafx.h"
#include "CSystem.h"

int APIENTRY wWinMain
	(_In_	 HINSTANCE	hInstance,
	_In_opt_ HINSTANCE	hPrevInstance,
	_In_	 LPWSTR		lpCmdLine,
	_In_	 int		nCmdShow)
{
	// Allocate system object
	CSystem* system = new CSystem;
	if (!system)
		return -1;

	// Initialize system object and run.
	if (system->Initialize())
		system->Run();

	// Shutdown system object and return memory
	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}