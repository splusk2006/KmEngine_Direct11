#pragma once

class CCamera : public AlignedAllocationPolicy<16>
{
public:
	CCamera();
	CCamera(const CCamera&);
	~CCamera();

	void setPos(float, float, float);
	void setRot(float, float, float);

	XMFLOAT3 getPos();
	XMFLOAT3 getRot();
	void	 getViewMat(XMMATRIX&);

	void Render();

private:
	XMFLOAT3 mPos;
	XMFLOAT3 mRot;
	XMMATRIX mViewMat;
};

