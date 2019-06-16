#include "stdafx.h"
#include "CModel.h"


CModel::CModel()
{}

CModel::CModel(const CModel &)
{}

CModel::~CModel()
{}

bool CModel::Initialize(ID3D11Device* device)
{
	return InitBuffers(device);
}

void CModel::Shutdown()
{
	ShutdownBuffers();
}

void CModel::Render(ID3D11DeviceContext* dc)
{
	RenderBuffers(dc);
}

int CModel::getIndexCnt()
{
	return mIndexCnt;
}

bool CModel::InitBuffers(ID3D11Device* device)
{
	// Make Vertex
	mVertexCnt = 3;
	VertexType* vertex = new VertexType[mVertexCnt];
	if (!vertex)
		return false;

	// Make Index
	mIndexCnt = 3;
	unsigned long* index = new unsigned long[mIndexCnt];
	if (!index)
		return false;

	// Bottom Left
	vertex[0].pos	= XMFLOAT3(-1.0f, -1.0f, 0.0f);	
	vertex[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// Top Mid
	vertex[1].pos	= XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	// Bottom Right
	vertex[2].pos	= XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertex[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	index[0] = 0; 	// Bottom Left
	index[1] = 1;	// Top Mid
	index[2] = 2;	// Bottom Right

	// Set vertex buffer description
	D3D11_BUFFER_DESC vertexBuffDesc;
	vertexBuffDesc.Usage				= D3D11_USAGE_DEFAULT;
	vertexBuffDesc.ByteWidth			= sizeof(VertexType) * mVertexCnt;
	vertexBuffDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.CPUAccessFlags		= 0;
	vertexBuffDesc.MiscFlags			= 0;
	vertexBuffDesc.StructureByteStride	= 0;

	// Set vertex subresource structure
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem			= vertex;
	vertexData.SysMemPitch		= 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	if (FAILED(device->CreateBuffer(&vertexBuffDesc, &vertexData, &mpVertexBuff)))
		return false;

	// Set index buffer description
	D3D11_BUFFER_DESC indexBuffDesc;
	indexBuffDesc.Usage					= D3D11_USAGE_DEFAULT;
	indexBuffDesc.ByteWidth				= sizeof(unsigned long) * mIndexCnt;
	indexBuffDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	indexBuffDesc.CPUAccessFlags		= 0;
	indexBuffDesc.MiscFlags				= 0;
	indexBuffDesc.StructureByteStride	= 0;

	// Set index subresource structure
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem			= index;
	indexData.SysMemPitch		= 0;
	indexData.SysMemSlicePitch	= 0;

	// Create Index buffer
	if (FAILED(device->CreateBuffer(&indexBuffDesc, &indexData, &mpIndexBuff)))
		return false;

	// Release vertex and index buffer
	delete[] vertex;
	vertex = nullptr;
	delete[] index;
	index = nullptr;

	return true;
}

void CModel::ShutdownBuffers()
{
	ReleaseID3D11Obj<>(mpIndexBuff);
	ReleaseID3D11Obj<>(mpVertexBuff);
}

void CModel::RenderBuffers(ID3D11DeviceContext* dc)
{
	// Set vertex buffer unit and offset
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// For rendering, activate vertex buffer in assembly
	dc->IASetVertexBuffers(0, 1, &mpVertexBuff, &stride, &offset);

	// For rendering, activate index buffer in assembly
	dc->IASetIndexBuffer(mpIndexBuff, DXGI_FORMAT_R32_UINT, 0);

	// Set Basic shape. In here, triangle.
	dc->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
