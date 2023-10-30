#include "Stairs.h"
#include"Modelmanager.h"

void Stairs::Initialize(const XMFLOAT3& Pos)
{
	pos_ = Pos;
	stairs_ = make_unique<Object3d>();
	stairs_= Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("Kaidan"));
	stairs_->SetPosition(pos_);
}

void Stairs::Update()
{
	stairs_->Update();
}

void Stairs::Draw()
{
	stairs_->Draw();
}
