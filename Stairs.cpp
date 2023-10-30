#include "Stairs.h"
#include"Modelmanager.h"

void Stairs::Initialize(const XMFLOAT3& Pos)
{
	unique_ptr<Object3d> stairs_ = make_unique<Object3d>();
	stairs_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("Kaidan"));
	pos_ = Pos;
	stairs_->SetPosition(Pos);

}

void Stairs::Update()
{
	stairs_->Update();
}

void Stairs::Draw()
{
	stairs_->Draw();
}
