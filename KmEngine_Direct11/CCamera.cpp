#include "stdafx.h"
#include "CCamera.h"


CCamera::CCamera()
{
	mPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRot = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CCamera::CCamera(const CCamera &)
{}

CCamera::~CCamera()
{}

void CCamera::setPos(float x, float y, float z)
{
	mPos.x = x;
	mPos.y = y;
	mPos.z = z;
}

void CCamera::setRot(float x, float y, float z)
{
	mRot.x = x;
	mRot.y = y;
	mRot.z = z;
}

XMFLOAT3 CCamera::getPos() { return mPos; }
XMFLOAT3 CCamera::getRot() { return mRot; }
void CCamera::getViewMat(XMMATRIX& view_mat) { view_mat = mViewMat; }

void CCamera::Render()
{
	XMFLOAT3	up, pos, lookAt;
	XMVECTOR	upVec, posVec, lookAtVec;
	float		pitch, yaw, roll;
	XMMATRIX	rotMat;

	// Set up vector
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	upVec = XMLoadFloat3(&up);

	// Set 3D world camera position
	pos = mPos;
	posVec = XMLoadFloat3(&pos);

	// Set camera look at point
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	lookAtVec = XMLoadFloat3(&lookAt);

	// Set camera rotation
	pitch	= mRot.x * 0.0174532925f;
	yaw		= mRot.y * 0.0174532925f;
	roll	= mRot.z * 0.0174532925f;
	rotMat = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform look at and up vector to rotation matrix
	// So view could rotate proper
	lookAtVec = XMVector3TransformCoord(lookAtVec, rotMat);
	upVec = XMVector3TransformCoord(upVec, rotMat);

	// Transform rotated camera position to viewer postion
	lookAtVec = XMVectorAdd(posVec, lookAtVec);

	// Create updated view matrix
	mViewMat = XMMatrixLookAtLH(posVec, lookAtVec, upVec);
}