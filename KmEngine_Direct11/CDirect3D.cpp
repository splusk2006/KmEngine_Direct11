#include "stdafx.h"
#include "CDirect3D.h"

CDirect3D::CDirect3D()
{}

CDirect3D::CDirect3D(const CDirect3D &)
{}

CDirect3D::~CDirect3D()
{}

bool CDirect3D::InitAdapter(int scr_w, int scr_h, bool vsync, unsigned int& numerator, unsigned int& denominator)
{
	// Set vertical synchroniztion state
	mVsyncEnabled = vsync;

	// Create DirectX graphics interface factory
	IDXGIFactory* factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
		return false;

	// Create first video card interface adapter with factory object
	IDXGIAdapter* adapter = nullptr;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
		return false;

	// Select first adapter for output
	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		return false;

	// Get mode numbers for output
	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, NULL)))
		return false;

	// Create display mode list(monitor - video card combination list)
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	// Fill in display moode list
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList)))
		return false;

	// Search for display mode which is proper
	// If found, save monitor refresh rate
	for (unsigned int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)scr_w &&
			displayModeList[i].Height == (unsigned int)scr_h)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
			break;
		}
	}

	// Get video card description
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
		return false;

	// Save video card memory unit as MB
	mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Save video card name
	size_t strLen = 0;
	if (wcstombs_s(&strLen, mVideoCardDescription, 128, adapterDesc.Description, 128) != 0)
		return false;

	// Delete display mode list
	delete[] displayModeList;
	displayModeList = nullptr;

	// Release output adapter, adapter and factory object
	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	return true;
}

bool CDirect3D::InitSwapChain(int scr_w, int scr_h, HWND hwnd, bool full_scr, int numerator, int denominator)
{
	// Init swap-chain structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set back buffer count
	swapChainDesc.BufferCount = 1;
	// Set back buffer Size
	swapChainDesc.BufferDesc.Width = scr_w;
	swapChainDesc.BufferDesc.Height = scr_h;
	// Set 32bit surface
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set back buffer refresh rate
	if (mVsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;

	// Set multi- to OFF
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set screen mode
	if (full_scr)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	// Set scanline ordering and scaling to UNSPECIFIED
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Empty next backbuffer after being output
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Dont use more options
	swapChainDesc.Flags = 0;

	// Set feature level to DirectX 11
	D3D_FEATURE_LEVEL featureLv = D3D_FEATURE_LEVEL_11_0;

	// Create swapchain, D3Ddevice, D3Ddevice context
	if (FAILED(D3D11CreateDeviceAndSwapChain
	(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		&featureLv, 1, D3D11_SDK_VERSION, &swapChainDesc,
		&mpSwapChain, &mpDevice, NULL, &mpDC)))
		return false;

	return true;
}

bool CDirect3D::InitBackBuffer()
{
	// Get backbuffer pointer
	ID3D11Texture2D* pBackBuff = nullptr;
	if (FAILED(mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuff)))
		return false;

	// Create render target view with backbuffer pointer
	if (FAILED(mpDevice->CreateRenderTargetView(pBackBuff, NULL, &mpRenderTargetView)))
		return false;

	// Release backbuffter pointer
	pBackBuff->Release();
	pBackBuff = nullptr;

	return true;
}

bool CDirect3D::InitDepthBuffer(int scr_w, int scr_h)
{
	// Init depth buffter structure
	D3D11_TEXTURE2D_DESC depthBuffDesc;
	ZeroMemory(&depthBuffDesc, sizeof(depthBuffDesc));

	// Fill in depth buffter structure
	depthBuffDesc.Width = scr_w;
	depthBuffDesc.Height = scr_h;
	depthBuffDesc.MipLevels = 1;
	depthBuffDesc.ArraySize = 1;
	depthBuffDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBuffDesc.SampleDesc.Count = 1;
	depthBuffDesc.SampleDesc.Quality = 0;
	depthBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBuffDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBuffDesc.CPUAccessFlags = 0;
	depthBuffDesc.MiscFlags = 0;

	// Create depth buffer texture
	if (FAILED(mpDevice->CreateTexture2D(&depthBuffDesc, NULL, &mpDepthStencilBuff)))
		return false;

	return true;
}

bool CDirect3D::InitDepthStencil()
{
	// Init stencil structure
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Fill in stencil structure
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Set front pixel stencil
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Set back pixel stencil
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil
	if (FAILED(mpDevice->CreateDepthStencilState(&depthStencilDesc, &mpDepthStencilState)))
		return false;

	// Set depth stencil state
	mpDC->OMSetDepthStencilState(mpDepthStencilState, 1);

	return true;
}

bool CDirect3D::InitDepthStencilView()
{
	// Init
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Fill in
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create
	if (FAILED(mpDevice->CreateDepthStencilView(mpDepthStencilBuff,
		&depthStencilViewDesc, &mpDepthStencilView)))
		return false;

	// Bind rtv and dsv to render pipeline
	mpDC->OMSetRenderTargets(1, &mpRenderTargetView, mpDepthStencilView);

	return true;
}

bool CDirect3D::InitRasterizer()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(mpDevice->CreateRasterizerState(&rasterDesc, &mpRasterState)))
		return false;

	mpDC->RSSetState(mpRasterState);

	return true;
}

bool CDirect3D::Initialize(int scr_w, int scr_h, bool vsync, HWND hwnd, bool full_scr, float scr_depth, float scr_near)
{
	unsigned int numerator	 = 0;
	unsigned int denominator = 0;

	// Adapter
	if (!InitAdapter(scr_w, scr_h, vsync, numerator, denominator))
		return false;

	// Swap Chain
	if (!InitSwapChain(scr_w, scr_h, hwnd, full_scr, numerator, denominator))
		return false;

	// Back Buffer
	if (!InitBackBuffer())
		return false;

/* Depth buffer */
	if (!InitDepthBuffer(scr_w, scr_h))
		return false;

/* Depth stencil */
	if (!InitDepthStencil())
		return false;

/* Depth stencil view */
	if (!InitDepthStencilView())
		return false;

/* Rasterizer */
	if (!InitRasterizer())
		return false;

/* Viewport */
	D3D11_VIEWPORT viewport;
	viewport.Width		= (float)scr_w;
	viewport.Height		= (float)scr_h;
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;
	viewport.TopLeftX	= 0.0f;
	viewport.TopLeftY	= 0.0f;

	mpDC->RSSetViewports(1, &viewport);

/* Matrix */
	float fov = 3.141592654f / 2.0f;
	float scrAspect = (float)scr_w / (float)scr_h;

	mProjectionMat	= XMMatrixPerspectiveFovLH(fov, scrAspect, scr_near, scr_depth);
	mWorldMat		= XMMatrixIdentity();
	mOrthoMat		= XMMatrixOrthographicLH((float)scr_w, (float)scr_h, scr_near, scr_depth);

	return true;
}

void CDirect3D::Shutdown()
{
	if (mpSwapChain)
		mpSwapChain->SetFullscreenState(false, NULL);

	if (mpRasterState)
	{
		mpRasterState->Release();
		mpRasterState = nullptr;
	}

	if (mpDepthStencilView)
	{
		mpDepthStencilView->Release();
		mpDepthStencilView = nullptr;
	}
	if (mpDepthStencilState)
	{
		mpDepthStencilState->Release();
		mpDepthStencilState = nullptr;
	}
	if (mpDepthStencilBuff)
	{
		mpDepthStencilBuff->Release();
		mpDepthStencilBuff= nullptr;
	}
	if (mpRenderTargetView)
	{
		mpRenderTargetView->Release();
		mpRenderTargetView = nullptr;
	}
	if (mpDC)
	{
		mpDC->Release();
		mpDC= nullptr;
	}
	if (mpDevice)
	{
		mpDevice->Release();
		mpDevice= nullptr;
	}
	if (mpSwapChain)
	{
		mpSwapChain->Release();
		mpSwapChain= nullptr;
	}

	//ReleaseID3D11Obj<>(mpRasterState);
	//ReleaseID3D11Obj<>(mpDepthStencilView);
	//ReleaseID3D11Obj<>(mpDepthStencilState);
	//ReleaseID3D11Obj<>(mpDepthStencilBuff);
	//ReleaseID3D11Obj<>(mpRenderTargetView);
	//ReleaseID3D11Obj<>(mpDC);
	//ReleaseID3D11Obj<>(mpDevice);
	//ReleaseID3D11Obj<>(mpSwapChain);
}

void CDirect3D::BeginScene(float r, float g, float b, float a)
{
	float color[4] = { r,g,b,a };

	// Clear back buffer
	mpDC->ClearRenderTargetView(mpRenderTargetView, color);

	// Clear depth buffer
	mpDC->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDirect3D::EndScene()
{
	// Show back buffer in screen
	if (mVsyncEnabled)
		// Lock screen refrash rate
		mpSwapChain->Present(1, 0);
	else
		// Show as fast as posible
		mpSwapChain->Present(0, 0);
}

ID3D11Device* CDirect3D::getDevice()	{ return mpDevice; }
ID3D11DeviceContext* CDirect3D::getDC() { return mpDC; }

void CDirect3D::getProjectionMat(XMMATRIX &pmat) { pmat = mProjectionMat; }
void CDirect3D::getWorldMat(XMMATRIX &wmat)		 { wmat = mWorldMat; }
void CDirect3D::getOrthoMat(XMMATRIX &omat)		 { omat = mOrthoMat; }

void CDirect3D::getVideoCardInfo(std::string& name, int& memory)
{
	name	= mVideoCardDescription;
	memory	= mVideoCardMemory;
}