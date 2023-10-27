#include"ObbAndObb.h"

#include "Collision.h"

bool ObbAndObb::HitTest(Object3d* obj1, Object3d* obj2)
{
    Vector3 objPos1 = obj1->GetMatWorld().r[3];
    Vector3 objPos2 = obj2->GetMatWorld().r[3];

    OBB obb1, obb2;

    //
    obb1.SetParam_Pos(objPos1);
    obb1.SetParam_Rot(obj1->GetMatRot());
    obb1.SetParam_Scl(obj1->GetObbScl());

    //
    obb2.SetParam_Pos(objPos2);
    obb2.SetParam_Rot(obj2->GetMatRot());
    obb2.SetParam_Scl(obj2->GetObbScl());

	return Collision::OBBCollision(obb1, obb2);
}
