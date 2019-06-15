#include "stdafx.h"
#include "CInput.h"
#include "CGraphics.h"
#include "CSystem.h"

CSystem::CSystem()
{}

CSystem::CSystem(const CSystem &)
{}

CSystem::~CSystem()
{}

bool CSystem::Initialize()
{
	// Window width,height
	int scrW = 0;
	int scrH = 0;

	// Initialize window creation
	this->InitializeWindows(scrW, scrH);

	// Allocate Input object
	mpInput = new CInput;
	if (!mpInput)
		return false;

	mpInput->Initialize();

	// Allocate Graphics object
	mpGraphics = new CGraphics;
	if (!mpGraphics)
		return false;

	return mpGraphics->Initialize(scrW,scrH,mHwnd);
}

void CSystem::Shutdown()
{
	// Return Graphics object
	if (mpGraphics)
	{
		mpGraphics->Shutdown();
		delete mpGraphics;
		mpGraphics = nullptr;
	}

	// Return Input object
	if (mpInput)
	{
		delete mpInput;
		mpInput = nullptr;
	}

	// Windows shutdown
	this->ShutdownWindows();
}

void CSystem::Run()
{
	// Create and initialize message struct
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Message loop
	while (true)
	{
		// Process window messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Break this loop if shutdown message is received
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Process Frame function
			if (!Frame())
				break;
		}
	}
}

LRESULT CSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
		mpInput->KeyDown((UINT)wparam);
		return 0;

	case WM_KEYUP:
		mpInput->KeyUp((UINT)wparam);
		return 0;

	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	return LRESULT();
}

bool CSystem::Frame()
{
	// Always exit with esc
	if (mpInput->IsKeyDown(VK_ESCAPE))
		return false;

	// Process graphics frame
	return mpGraphics->Frame();
}

void CSystem::InitializeWindows(int& scr_w, int& scr_h)
{
	// Set external pointer to this obejct
	gpAppHandle = this;

	// Get instance of this program
	mHinstance = GetModuleHandle(NULL);

	// Set name of program
	mAppName = "KmEngine";
	LPCWSTR	lpcwAppName = L"KmEngine";//(LPCWSTR)mAppName.c_str();

	// Set windows class
	WNDCLASSEX wc;
	wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = mHinstance;
	wc.hIcon		 = LoadIcon(NULL,IDI_WINLOGO);
	wc.hIconSm		 = wc.hIcon;
	wc.hCursor		 = LoadCursor(NULL,IDC_ARROW);;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = lpcwAppName;
	wc.cbSize		 = sizeof(WNDCLASSEX);

	// Register windows class
	RegisterClassEx(&wc);

	// Read monitor resolution
	int monitorW = GetSystemMetrics(SM_CXSCREEN);
	int monitorH = GetSystemMetrics(SM_CYSCREEN);

	int posX = 0;
	int posY = 0;

	// Set as fullscreen mode
	if (FULL_SCREEN)
	{
		scr_w = monitorW;
		scr_h = monitorH;

		DEVMODE dmScreenSettings;
		ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));

		// Screen resolution -> Desktop resolution
		// Color -> 32bit
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= (ULONG)scr_w;
		dmScreenSettings.dmPelsHeight	= (ULONG)scr_h;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	// Set as window mode
	else
	{
		scr_w = 800;
		scr_h = 600;

		// Place window to mid screen 
		posX = (monitorW - scr_w) / 2;
		posY = (monitorH - scr_h) / 2;
	}

	// Generate window and get handle
	mHwnd = CreateWindowEx
		(WS_EX_APPWINDOW, lpcwAppName, lpcwAppName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, scr_w, scr_h, NULL, NULL, mHinstance, NULL);

	// Show window and set focus
	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);
}

void CSystem::ShutdownWindows()
{
	// Reset display setting if is fullscreen mode.
	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	// Destroy window
	DestroyWindow(mHwnd);
	mHwnd = NULL;

	// Unregister program instance
	UnregisterClass((LPCWSTR)mAppName.c_str(), mHinstance);
	mHinstance = NULL;

	// Reset external pointer
	gpAppHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check is window terminated
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// Check is window closed
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// Else, Call system class message process
	default:
		return gpAppHandle->MessageHandler(hwnd, umsg, wparam, lparam);
	}
}