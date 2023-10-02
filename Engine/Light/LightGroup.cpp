#include "LightGroup.h"

using namespace DirectX;

ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInitialize()
{
	assert(!LightGroup::device);
	LightGroup::device = DirectXSetting::GetIns()->GetDev();
}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();
	instance->Initialize();
	return instance;
}

std::unique_ptr<LightGroup> LightGroup::UniquePtrCreate()
{
	std::unique_ptr<LightGroup> instance = std::make_unique<LightGroup>();
	instance->Initialize();
	return instance;
}

void LightGroup::Initialize() 
{
	assert(device);

	DefaultLightSetting();

	HRESULT result;
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	if (FAILED(result)) {
		assert(0);
	}

	TransferConstBuffer();
}

void LightGroup::Update() {
	if (isDirty) {
		TransferConstBuffer();
		isDirty = false;
	}
}

void LightGroup::Draw(UINT rootParameterIndex) {
	DirectXSetting::GetIns()->GetCmdList()->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambientColor = ambientColor;
		for (int i = 0; i < DirLightNum; i++) {
			if (dirLights[i].GetIsActive()) {
				constMap->dirLights[i].isActive = 1;
				constMap->dirLights[i].lightVec = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightColor = dirLights[i].GetLightColor();
			}
			else {
				constMap->dirLights[i].isActive = 0;
			}
		}
		for (int i = 0; i < PointLightNum; i++) {
			if (pointLights[i].GetIsActive()) {
				constMap->pointLights[i].isActive = 1;
				constMap->pointLights[i].lightPos = pointLights[i].GetLightPos();
				constMap->pointLights[i].lightColor = pointLights[i].GetLightColor();
				constMap->pointLights[i].lightAtten = pointLights[i].GetLightAtten();
			}
			else {
				constMap->pointLights[i].isActive = 0;
			}
		}
		for (int i = 0; i < SpotLightNum; i++) {
			if (spotLights[i].GetIsActive()) {
				constMap->spotLights[i].isActive = 1;
				constMap->spotLights[i].lightVec = -spotLights[i].GetLightVec();
				constMap->spotLights[i].lightPos = spotLights[i].GetLightPos();
				constMap->spotLights[i].lightColor = spotLights[i].GetLightColor();
				constMap->spotLights[i].lightAtten = spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightCosAngle = spotLights[i].GetLightCosAngle();
			}
			else {
				constMap->spotLights[i].isActive = 0;
			}
		}
		for (int i = 0; i < CircleShadowNum; i++) {
			if (circleShadows[i].GetIsActive()) {
				constMap->circleShadow[i].isActive = 1;
				constMap->circleShadow[i].dir = -circleShadows[i].GetDir();
				constMap->circleShadow[i].casterPos = circleShadows[i].GetCasterPos();
				constMap->circleShadow[i].distanceCasterLight = circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadow[i].atten = circleShadows[i].GetAtten();
				constMap->circleShadow[i].cosAngle = circleShadows[i].GetAngle();
			}
			else {
				constMap->circleShadow[i].isActive = 0;
			}
		}
		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSetting() {
	ambientColor = { 1.0f, 1.0f, 1.0f };

	dirLights[0].SetIsActive(true);
	dirLights[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[0].SetLightDir({ 0.0f, -1.0f, 0.0f, 0.0f });

	dirLights[1].SetIsActive(true);
	dirLights[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[1].SetLightDir({ 0.5f, 0.1f, 0.2f, 0.0f });

	dirLights[2].SetIsActive(true);
	dirLights[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dirLights[2].SetLightDir({ -0.5f, 0.1f, -0.2f, 0.0f });
}

void LightGroup::SetAmbientColor(const XMFLOAT3& ambientColor) {
	this->ambientColor = ambientColor;
	isDirty = true;
}

void LightGroup::SetDirLightActive(int index, bool isActive) {
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetIsActive(isActive);
}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3& lightColor) {
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightColor(lightColor);
	isDirty = true;
}

void LightGroup::SetDirLightDirection(int index, const XMFLOAT3& lightDir) {
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightDir({ lightDir.x, lightDir.y, lightDir.z });
	isDirty = true;
}

void LightGroup::SetPointLightActive(int index, bool isActive)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetActive(isActive);
}

void LightGroup::SetPointLightPos(int index, const XMFLOAT3& lightPos)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightPos(lightPos);
	isDirty = true;
}

void LightGroup::SetPointLightColor(int index, const XMFLOAT3& lightColor)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightColor(lightColor);
	isDirty = true;
}

void LightGroup::SetPointLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < PointLightNum);

	pointLights[index].SetLightAtten(lightAtten);
	isDirty = true;
}

void LightGroup::SetSpotLightActive(int index, bool isActive)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetIsActive(isActive);
}

void LightGroup::SetSpotLightDirection(int index, const XMVECTOR& lightDir)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightVec(lightDir);
	isDirty = true;
}

void LightGroup::SetSpotLightPos(int index, const XMFLOAT3& lightPos)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightPos(lightPos);
	isDirty = true;
}

void LightGroup::SetSpotLightColor(int index, const XMFLOAT3& lightColor)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightColor(lightColor);
	isDirty = true;
}

void LightGroup::SetSpotLightAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < SpotLightNum);
	
	spotLights[index].SetLightAtten(lightAtten);
	isDirty = true;
}

void LightGroup::SetSpotLightAngle(int index, const XMFLOAT2& lightAngle)
{
	assert(0 <= index && index < SpotLightNum);

	spotLights[index].SetLightCosAngle(lightAngle);
	isDirty = true;
}

void LightGroup::SetCircleShadowActive(int index, bool isActive)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetIsActive(isActive);
}

void LightGroup::SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetCasterPos(casterPos);
	isDirty = true;
}

void LightGroup::SetCircleShadowDir(int index, const XMVECTOR& lightDir)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetDir(lightDir);
	isDirty = true;
}

void LightGroup::SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetDistanceCasterLight(distanceCasterLight);
	isDirty = true;
}

void LightGroup::SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetAtten(lightAtten);
	isDirty = true;
}

void LightGroup::SetCircleShadowAngle(int index, const XMFLOAT2& angle)
{
	assert(0 <= index && index < CircleShadowNum);

	circleShadows[index].SetAngle(angle);
	isDirty = true;
}