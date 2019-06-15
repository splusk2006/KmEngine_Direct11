#include "stdafx.h"
#include "CInput.h"
#include "CGraphics.h"
#include "CSystem.h"

using namespace km;

CSystem::CSystem()
{}

CSystem::CSystem(const CSystem &)
{}

CSystem::~CSystem()
{}

bool CSystem::Initialize()
{
	// Window width,height
	int screenW = 0;
	int screenH = 0;

	// Initialize window creation
	this->InitializeWindows(screenW, screenH);

	// Allocate Input object
	mpInput = new CInput;
	if (!mpInput)
		return false;

	mpInput->Initialize();

	// Allocate Graphics object
	mpGraphics = new CGraphics;
	if (!mpGraphics)
		return false;

	return mpGraphics->Initialize(screenW,screenH,mHwnd);
}

void CSystem::Shutdown()
{
	if (mpGraphics)
	{
		mpGraphics->Shutdown();
		delete mpGraphics;
		mpGraphics = 0;
	}
}

void CSystem::Run()
{
}

LRESULT CSystem::MessageHandler(HWND, UINT, WPARAM, LPARAM)
{
	return LRESULT();
}

bool CSystem::Frame()
{
	return false;
}

void CSystem::InitializeWindows(int &, int &)
{
}

void CSystem::ShutdownWindows()
{
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
{
	return LRESULT();
}
