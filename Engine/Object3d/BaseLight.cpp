#include "BaseLight.h"

using namespace DirectX;

ID3D12Device* BaseLight::device = nullptr;

void BaseLight::StaticInitialize()
{
	BaseLight::device = DirectXSetting::GetIns()->GetDev();
	assert(BaseLight::device);
	assert(device);
}
