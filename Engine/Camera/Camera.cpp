#include "Camera.h"

using namespace DirectX;

XMMATRIX Camera::matView{};
XMMATRIX Camera::matProjection{};
XMMATRIX Camera::matWorld{};
XMMATRIX Camera::matViewPort{};
XMFLOAT3 Camera::eye = { 0, 200.0f, -200.0f };
XMFLOAT3 Camera::target = { 0, 0, 0 };
XMFLOAT3 Camera::up = { 0, 1, 0 };
XMMATRIX Camera::matBillboard = XMMatrixIdentity();
XMMATRIX Camera::matBillboardY = XMMatrixIdentity();
bool Camera::isMatWorldCalc = false;

void Camera::InitializeCamera(int32_t window_width, int32_t window_height)
{
	// ビュー行列の生成
	UpdateViewMatrix();

	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 10000.0f
	);

	//ワールド行列の生成
	matWorld = XMMatrixTranslation(eye.x, eye.y, eye.z);

	//ビューポート行列の生成
	matViewPort = XMMatrixIdentity();
	matViewPort.r[0].m128_f32[0] = (float)window_width / 2.0f;
	matViewPort.r[1].m128_f32[1] = -1.0f * (float)(window_height / 2.0f);
	matViewPort.r[2].m128_f32[2] = 1.0f;
	matViewPort.r[3].m128_f32[0] = (float)window_width / 2.0f;
	matViewPort.r[3].m128_f32[1] = (float)window_height / 2.0f;
	matViewPort.r[3].m128_f32[3] = 1.0f;
}

void Camera::SetEye(XMFLOAT3 eye)
{
	Camera::eye = eye;

	UpdateViewMatrix();

	if (isMatWorldCalc) {
		UpdateWorldMatrix();
	}
}

void Camera::SetTarget(XMFLOAT3 target)
{
	Camera::target = target;

	UpdateViewMatrix();

	if (isMatWorldCalc) {
		UpdateWorldMatrix();
	}
}

void Camera::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::CameraMoveEyeVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void Camera::CameraMoveTargetVector(XMFLOAT3 move) {
	XMFLOAT3  target_moved = GetTarget();

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetTarget(target_moved);
}

void Camera::UpdateViewMatrix() {

	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	XMVECTOR upVector = XMLoadFloat3(&up);

	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	XMVECTOR  cameraAxisX;
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	XMMATRIX matCameraRot;
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
	ybillCameraAxisX = cameraAxisX;
	ybillCameraAxisY = XMVector3Normalize(upVector);
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);

	matView = XMMatrixTranspose(matCameraRot);

	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);

	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	matView.r[3] = translation;
}

void Camera::UpdateWorldMatrix()
{
	float xRot, yRot, zRot;
	XMFLOAT3 distance;
	XMMATRIX matRot, matTrans;

	distance = XMFLOAT3(eye.x - target.x, eye.y - target.y, eye.z - target.z);
	xRot = -(atan2(distance.y, sqrtf((float)pow(distance.z, 2)) + (float)pow(distance.x, 2)) * 180.0f / 3.14f);
	yRot = (atan2(distance.x, distance.z) * 180.0f / 3.14f);
	zRot = 0;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(zRot));
	matRot *= XMMatrixRotationX(XMConvertToRadians(xRot));
	matRot *= XMMatrixRotationY(XMConvertToRadians(yRot));
	matTrans = XMMatrixTranslation(eye.x, eye.y, eye.z);

	up = XMFLOAT3{ matRot.r[1].m128_f32[0], matRot.r[1].m128_f32[1], matRot.r[1].m128_f32[2] };

	matWorld = XMMatrixIdentity();
	matWorld *= matRot;
	matWorld *= matTrans;

}

void Camera::SetUp(XMFLOAT3 up) {
	Camera::up = up;

	UpdateViewMatrix();
}

void Camera::SetMatWorld(XMMATRIX matWorld) {
	Camera::matWorld = matWorld;
	Camera::isMatWorldCalc = true;
}