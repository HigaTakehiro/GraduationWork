#include "CollisionManager.h"
#include "SphereAndSphere.h"
#include "ObbAndObb.h"
#include "SafeDelete.h"

CollisionManager::CollisionManager()
{
	collision_[(long long)Object3d::CollisionType::Sphere][(long long)Object3d::CollisionType::Sphere] = new SphereAndSphere();

	//OBB
	collision_[(long long)Object3d::CollisionType::Obb][(long long)Object3d::CollisionType::Obb] = new ObbAndObb();
}

CollisionManager::~CollisionManager()
{
	for (int32_t i = 0; i < (int32_t)Object3d::CollisionType::None; i++) {
		for (int32_t j = 0; j < (int32_t)Object3d::CollisionType::None; j++) {
			safe_delete(collision_[i][j]);
		}
	}
}

void CollisionManager::Update()
{
	const int32_t noObjType = -1;
	auto itr = std::remove_if(objList_.begin(), objList_.end(), [&](Object3d* obj)->bool {return obj->GetObjType() <= noObjType; });
	objList_.erase(itr, objList_.end());

	for (auto obj1 : objList_) {
		for (auto obj2 : objList_) {
			CollisionCheck(obj1, obj2);
		}
	}
}

void CollisionManager::AddObj(Object3d& obj)
{
	objList_.push_back(&obj);
}

void CollisionManager::CollisionCheck(Object3d* obj1, Object3d* obj2)
{
	const int32_t hammer = 0x04;
	const int32_t enemy = 0x02;
	const int32_t player = 0x01;
	const int32_t object = 0x08;
	const int32_t item = 0x0a;
	const int32_t playerAndItem = 0x0b;
	const int32_t objectAndEnemy = 0x0a;
	const int32_t playerAndObject = 0x09;

	int32_t type;
	int32_t type1 = obj1->GetObjType();
	int32_t type2 = obj2->GetObjType();

	type = type1 | type2;

	if (obj1 == obj2) {
		return;
	}

	if (type == playerAndItem) {
		HitTest(obj1, obj2);
		return;
	}
	if (type == playerAndObject) {
		HitTest(obj1, obj2);
		if (type1 == object) {
			obj1->SetIsHit(false);
		}
		else if (type2 == object) {
			obj2->SetIsHit(false);
		}
		return;
	}

	if (type == player) return;
	if (type == hammer) return;
	if (type == enemy) return;
	if (type == object) return;
	if (type == objectAndEnemy) return;
	if (type1 == 0x0a || type2 == 0x0a) return;

	HitTest(obj1, obj2);
}

void CollisionManager::HitTest(Object3d* obj1, Object3d* obj2)
{
	if (obj1->GetColType() == Object3d::CollisionType::Sphere && obj2->GetColType() == Object3d::CollisionType::Sphere) {
		if (collision_[(long long)Object3d::CollisionType::Sphere][(long long)Object3d::CollisionType::Sphere]->HitTest(obj1, obj2)) {
			obj1->OnCollision();
			obj2->OnCollision();
		}
	}

	//OBB
	if (obj1->GetColType() == Object3d::CollisionType::Obb && obj2->GetColType() == Object3d::CollisionType::Obb) {
		if (collision_[(long long)Object3d::CollisionType::Obb][(long long)Object3d::CollisionType::Obb]->HitTest(obj1, obj2)) {
			obj1->OnCollision();
			obj2->OnCollision();
		}
	}
}

void CollisionManager::Finalize()
{
	if (objList_.size() != 0) {
		objList_.clear();
	}
}
