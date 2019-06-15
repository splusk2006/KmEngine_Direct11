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

void CInput::KeyDown(UCHAR _input)
{
	mKeys[_input] = true;
}

void CInput::KeyUp(UCHAR _input)
{
	mKeys[_input] = false;
}

bool CInput::IsKeyDown(UCHAR _input)
{
	return mKeys[_input];
}
