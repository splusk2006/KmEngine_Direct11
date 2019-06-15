#include "stdafx.h"
#include "CGraphics.h"
#include "CDirect3D.h"

CGraphics::CGraphics()
{}

CGraphics::CGraphics(const CGraphics &)
{}

CGraphics::~CGraphics()
{}

bool CGraphics::Initialize(int scr_w, int scr_h, HWND hwnd)
{
	// Create Direct3D object
	mpDirect3D = (CDirect3D*)_aligned_malloc(sizeof(CDirect3D), 16);
	if (!mpDirect3D)
		return false;

	// Init Direct3D object
	if (!(mpDirect3D->Initialize(scr_w, scr_h, VSYNC_ENABLED,
		hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void CGraphics::Shutdown()
{
	// Release Direct3D object
	if (mpDirect3D)
	{
		mpDirect3D->Shutdown();
		_aligned_free(mpDirect3D);
		mpDirect3D = nullptr;
	}
}

bool CGraphics::Frame()
{
	return Render();
}

bool CGraphics::Render()
{
	// Draw Scene 
	mpDirect3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Show Scene
	mpDirect3D->EndScene();

	return true;
}