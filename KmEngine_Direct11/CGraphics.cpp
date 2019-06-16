#include "stdafx.h"
#include "CGraphics.h"
#include "CDirect3D.h"
#include "CCamera.h"
#include "CModel.h"
#include "CColorShader.h"

CGraphics::CGraphics()
{}

CGraphics::CGraphics(const CGraphics &)
{}

CGraphics::~CGraphics()
{}

bool CGraphics::Initialize(int scr_w, int scr_h, HWND hwnd)
{
/* Direct3D */
	// Allocate Direct3D object
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

/* Camera */
	// Allocate Camera object
	mpCamera = new CCamera;
	if (!mpCamera)
		return false;

	// Init Camera
	mpCamera->setPos(0.0f, 0.0f, -5.0f);

/* Model */
	// Allocate Model object
	mpModel = new CModel;
	if (!mpModel)
		return false;

	// Init Model
	if (!mpModel->Initialize(mpDirect3D->getDevice()))
	{
		MessageBox(hwnd, L"Could not initialize Model object.", L"Error", MB_OK);
		return false;
	}

/* Shader */
	// Allocate Shader object
	mpColorShader = new CColorShader;
	if (!mpColorShader)
		return false;

	// Init ColorShader
	if(!mpColorShader->Initialize(mpDirect3D->getDevice(),hwnd))
	{
		MessageBox(hwnd, L"Could not initialize color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void CGraphics::Shutdown()
{
	// Release Color shader object
	if (mpColorShader)
	{
		mpColorShader->Shutdown();
		delete mpColorShader;
		mpColorShader = nullptr;
	}
	// Release Model object
	if (mpModel)
	{
		mpModel->Shutdown();
		delete mpModel;
		mpModel = nullptr;
	}
	// Release Camera obejct
	if (mpCamera)
	{
		delete mpCamera;
		mpCamera = nullptr;
	}
	// Release Direct3D object
	if (mpDirect3D)
	{
		mpDirect3D->Shutdown();
		_aligned_free(mpDirect3D);
		mpDirect3D = nullptr;
	}
}

bool CGraphics::GameLoop()
{
	return Render();
}

bool CGraphics::Render()
{
	// Draw Scene 
	mpDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Create view matrix with camera postion
	mpCamera->Render();

	XMMATRIX worldMat, viewMat, projMat;
	mpDirect3D->getWorldMat(worldMat);
	mpCamera->getViewMat(viewMat);
	mpDirect3D->getProjectionMat(projMat);

	// Set model vertex and index vertex to graphic pipeline
	mpModel->Render(mpDirect3D->getDC());

	// Render model with color shader
	if (!mpColorShader->Render(mpDirect3D->getDC(), mpModel->getIndexCnt(),
		CColorShader::MatrixBufferType(worldMat, viewMat, projMat)))
		return false;

	// Show Scene
	mpDirect3D->EndScene();

	return true;
}