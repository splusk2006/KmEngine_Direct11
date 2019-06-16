#include "stdafx.h"
#include "CColorShader.h"


CColorShader::CColorShader()
{}

CColorShader::CColorShader(const CColorShader &)
{}

CColorShader::~CColorShader()
{}

bool CColorShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	WCHAR vertexShaderFile[64] = L"../KmEngine_Direct11/color.vs";
	WCHAR pixelShaderFile[64] = L"../KmEngine_Direct11/color.ps";

	// Initialize vertex and pixel shader
	return this->InitShader(device, hwnd, vertexShaderFile, pixelShaderFile);
}

void CColorShader::Shutdown()
{
	ShutdownShader();
}

bool CColorShader::Render(ID3D11DeviceContext* dc, int index_cnt, MatrixBufferType mat)
{
	// Set shader parameters for rendering
	if (!SetShaderParam(dc, mat))
		return false;

	// Render buffer with shader
	RenderShader(dc, index_cnt);

	return true;
}


bool CColorShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vs_file, WCHAR *ps_file)
{
	ID3D10Blob* errorMsg = nullptr;

	// Compile vertex shader code
	ID3D10Blob* vertexShaderBuff = nullptr;
	LPCSTR vsEntryPoint = "ColorVertexShader";
	LPCSTR vsTarget = "vs_5_0";
	if (FAILED(D3DCompileFromFile(vs_file, NULL, NULL, vsEntryPoint, vsTarget,
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuff, &errorMsg)))
	{
		// If compile failed, output error message
		if (errorMsg)
			OutputShaderErrorMsg(errorMsg, hwnd, vs_file);
		// Else, cannot find shader file
		else
			MessageBox(hwnd, vs_file, L"Missing Shader File", MB_OK);

		return false;
	}

	// Compile pixel shader code
	ID3D10Blob* pixelShaderBuff = nullptr;
	LPCSTR psEntryPoint = "ColorPixelShader";
	LPCSTR psTarget = "ps_5_0";
	if (FAILED(D3DCompileFromFile(ps_file, NULL, NULL, psEntryPoint, psTarget,
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuff, &errorMsg)))
	{
		// If compile failed, output error message
		if (errorMsg)
			OutputShaderErrorMsg(errorMsg, hwnd, ps_file);
		// Else, cannot find shader file
		else
			MessageBox(hwnd, ps_file, L"Missing Shader File", MB_OK);

		return false;
	}

	// Create vertex shader from buffer
	if (FAILED(device->CreateVertexShader(vertexShaderBuff->GetBufferPointer(),
		vertexShaderBuff->GetBufferSize(), NULL, &mpVertexShader)))
		return false;

	// Create pixel shader from buffer
	if (FAILED(device->CreatePixelShader(pixelShaderBuff->GetBufferPointer(),
		pixelShaderBuff->GetBufferSize(), NULL, &mpPixelShader)))
		return false;


	// Vertex input rayout structure
	// This must be same with "VertexType" in "shader and CModel"
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName			= "POSITION";
	polygonLayout[0].SemanticIndex			= 0;
	polygonLayout[0].Format					= DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot				= 0;
	polygonLayout[0].AlignedByteOffset		= 0;
	polygonLayout[0].InputSlotClass			= D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate	= 0;

	polygonLayout[1].SemanticName			= "COLOR";
	polygonLayout[1].SemanticIndex			= 0;
	polygonLayout[1].Format					= DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot				= 0;
	polygonLayout[1].AlignedByteOffset		= D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass			= D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate	= 0;

	// Get layout elements number
	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create vertex input layout
	if (FAILED(device->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuff->GetBufferPointer(), vertexShaderBuff->GetBufferSize(),
		&mpLayout)))
		return false;

	// Release vs and ps
	vertexShaderBuff->Release();
	vertexShaderBuff = nullptr;
	pixelShaderBuff->Release();
	pixelShaderBuff = nullptr;

	// Fill in buffer description in vertex shader
	D3D11_BUFFER_DESC matBuffDesc;
	matBuffDesc.Usage				= D3D11_USAGE_DYNAMIC;
	matBuffDesc.ByteWidth			= sizeof(MatrixBufferType);
	matBuffDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	matBuffDesc.CPUAccessFlags		= D3D10_CPU_ACCESS_WRITE;
	matBuffDesc.MiscFlags			= 0;
	matBuffDesc.StructureByteStride = 0;

	// Create const buffer pointer to make this class to access buffer description
	if (FAILED(device->CreateBuffer(&matBuffDesc, NULL, &mpMatBuff)))
		return false;

	return true;
}

void CColorShader::ShutdownShader()
{
	ReleaseID3D11Obj<>(mpMatBuff);
	ReleaseID3D11Obj<>(mpLayout);
	ReleaseID3D11Obj<>(mpPixelShader);
	ReleaseID3D11Obj<>(mpVertexShader);
}

void CColorShader::OutputShaderErrorMsg(ID3D10Blob* error_msg, HWND hwnd, WCHAR* shader_file)
{
	// Output error message
	OutputDebugStringA(reinterpret_cast<const char*>(error_msg->GetBufferPointer()));

	// Release error message
	error_msg->Release();
	error_msg = nullptr;

	// If compile error exist, show popup message
	MessageBox(hwnd, L"Shader compile error.", shader_file, MB_OK);
}

bool CColorShader::SetShaderParam(ID3D11DeviceContext* dc, MatrixBufferType mat)
{
	// Transpos matrix so shader can use it
	mat.world		= XMMatrixTranspose(mat.world);
	mat.view		= XMMatrixTranspose(mat.view);
	mat.projection	= XMMatrixTranspose(mat.projection);

	// Lock const buffer to write data
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(dc->Map(mpMatBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	// Get Pointer
	MatrixBufferType* pData = (MatrixBufferType*)mappedResource.pData;

	// Copy matrix into const buffer
	pData->world = mat.world;
	pData->view = mat.view;
	pData->projection = mat.projection;

	// Unlock const buffer
	dc->Unmap(mpMatBuff, 0);

	// Set const buffer position in vertex shader
	unsigned numBuff = 0;

	// Change const buffer of vertex shader 
	dc->VSSetConstantBuffers(numBuff, 1, &mpMatBuff);

	return true;
}

void CColorShader::RenderShader(ID3D11DeviceContext* dc, int index_cnt)
{
	// Set layout
	dc->IASetInputLayout(mpLayout);

	// Set shader
	dc->VSSetShader(mpVertexShader, NULL, 0);
	dc->PSSetShader(mpPixelShader, NULL, 0);

	// Draw
	dc->DrawIndexed(index_cnt, 0, 0);
}