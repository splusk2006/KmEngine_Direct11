#pragma once

class CDirect3D
{
public:
	CDirect3D();
	CDirect3D(const CDirect3D&);
	~CDirect3D();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device*		 getDevice();
	ID3D11DeviceContext* getDC();

	void getProjectionMat(XMMATRIX&);
	void getWorldMat(XMMATRIX&);
	void getOrthoMat(XMMATRIX&);

	void getVideoCardInfo(std::string&, int&);

private:
	bool InitAdapter(int, int, bool, unsigned int&, unsigned int&);
	bool InitSwapChain(int, int, HWND, bool, int, int);
	bool InitBackBuffer();
	bool InitDepthBuffer(int, int);
	bool InitDepthStencil();
	bool InitDepthStencilView();
	bool InitRasterizer();

	template<typename T>
	inline void ReleaseID3D11Obj(T*& obj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}

private:
	bool		mVsyncEnabled				= false;
	int			mVideoCardMemory			= 0;
	char		mVideoCardDescription[128]	= { 0, };

	XMMATRIX	mProjectionMat;
	XMMATRIX	mWorldMat;
	XMMATRIX	mOrthoMat;

	IDXGISwapChain*				mpSwapChain			= nullptr;
	ID3D11Device*				mpDevice			= nullptr;
	ID3D11DeviceContext*		mpDC				= nullptr;
	ID3D11RenderTargetView*		mpRenderTargetView	= nullptr;
	ID3D11Texture2D*			mpDepthStencilBuff	= nullptr;
	ID3D11DepthStencilState*	mpDepthStencilState = nullptr;
	ID3D11DepthStencilView*		mpDepthStencilView	= nullptr;
	ID3D11RasterizerState*		mpRasterState		= nullptr;
};