#pragma once

class CColorShader : public AlignedAllocationPolicy<16>
{
public:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

		MatrixBufferType(XMMATRIX w, XMMATRIX v, XMMATRIX p)
		{
			world = w;
			view = v;
			projection = p;
		}
	};

public:
	CColorShader();
	CColorShader(const CColorShader&);
	~CColorShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, MatrixBufferType);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMsg(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParam(ID3D11DeviceContext*, MatrixBufferType);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader*	mpVertexShader	= nullptr;
	ID3D11PixelShader*	mpPixelShader	= nullptr;
	ID3D11InputLayout*	mpLayout		= nullptr;
	ID3D11Buffer*		mpMatBuff		= nullptr;
};

