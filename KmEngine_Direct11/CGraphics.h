#pragma once

// GLOBALS
const bool	FULL_SCREEN		= false;
const bool	VSYNC_ENABLED	= true;
const float SCREEN_DEPTH	= 1000.0f;
const float SCREEN_NEAR		= 0.1f;

// Forward declarations
class CDirect3D;

class CGraphics
{
public:
	CGraphics();
	CGraphics(const CGraphics&);
	~CGraphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	CDirect3D* mpDirect3D = nullptr;
};