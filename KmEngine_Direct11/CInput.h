#pragma once

namespace km{
class CInput
{
public:
	CInput();
	CInput(const CInput&);
	~CInput();

	void Initialize();

	void KeyDown(UCHAR);
	void KeyUp(UCHAR);

	bool IsKeyDown(UCHAR);

private:
	bool mKeys[256];
};
}