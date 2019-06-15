#include "stdafx.h"
#include "CInput.h"

using namespace km;

CInput::CInput()
{
}

CInput::CInput(const CInput &)
{
}


CInput::~CInput()
{
}

void CInput::Initialize()
{
	// Initialize key array
	for (auto& keys : mKeys)
	{
		keys = false;
	}
}

void CInput::KeyDown(UINT input)
{
	mKeys[input] = true;
}

void CInput::KeyUp(UINT input)
{
	mKeys[input] = false;
}

bool CInput::IsKeyDown(UINT input)
{
	return mKeys[input];
}
