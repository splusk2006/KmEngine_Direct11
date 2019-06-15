#pragma once

// Forward declarations
class km::CInput;
class km::CGraphics;

namespace km{
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
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	std::string mAppName;
	HINSTANCE	mHinstance;
	HWND		mHwnd;
	
	CInput*		mpInput	= nullptr;
	CGraphics*	mpGraphics = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static CSystem* ApplicationHandle = 0;
}