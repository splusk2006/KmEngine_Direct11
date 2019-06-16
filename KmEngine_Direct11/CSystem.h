#pragma once

// Forward declarations
class CInput;
class CGraphics;

class CSystem
{
public:
	CSystem();
	CSystem(const CSystem&);
	~CSystem();

	bool Initialize();
	void Shutdown();
	void Run();

	// Process windows system messages
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool GameLoop();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR		mAppName;
	HINSTANCE	mHinstance;
	HWND		mHwnd;
	
	CInput*		mpInput	= nullptr;
	CGraphics*	mpGraphics = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static CSystem* gpAppHandle = nullptr;