#pragma once

// Linking
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

// For Warning C4316 (Memory alignment)
#include "AlignedAllocationPolicy.h"

// For safe release
template<typename T>
inline void ReleaseID3D11Obj(T*& obj)
{
	if (obj != nullptr)
	{
		obj->Release();
		obj = nullptr;
	}
}