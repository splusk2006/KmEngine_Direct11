#pragma once

class CModel : public AlignedAllocationPolicy<16>
{
private:
	struct VertexType
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

public:
	CModel();
	CModel(const CModel&);
	~CModel();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int	 getIndexCnt();

private:
	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	int mVertexCnt	= 0;
	int mIndexCnt	= 0;
	ID3D11Buffer* mpVertexBuff	= nullptr;
	ID3D11Buffer* mpIndexBuff	= nullptr;
};

