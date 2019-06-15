#pragma once

class CInput
{
public:
	CInput();
	CInput(const CInput&);
	~CInput();

	void Initialize();

	void KeyDown(UINT);
	void KeyUp(UINT);

	bool IsKeyDown(UINT);

private:
	bool mKeys[256];
};