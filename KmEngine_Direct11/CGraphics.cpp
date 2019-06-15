#include "stdafx.h"
#include "CGraphics.h"

using namespace km;

CGraphics::CGraphics()
{
}

CGraphics::CGraphics(const CGraphics &)
{
}


CGraphics::~CGraphics()
{
}

bool CGraphics::Initialize(int, int, HWND)
{
	return true;
}

void CGraphics::Shutdown()
{
}

bool CGraphics::Frame()
{
	return true;
}

bool CGraphics::Render()
{
	return true;
}
