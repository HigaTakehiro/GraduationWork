#include "BaseEnemy.h"


/***                    GETTER                      ***/
/******************************************************/
unsigned int BaseEnemy::GetHP() const { return _status.HP; }
unsigned int BaseEnemy::GetAttackVal() const { return _status.DamageValue; }

XMFLOAT3 BaseEnemy::GetPos() const { return _status.Pos; }
XMFLOAT3 BaseEnemy::GetRot() const { return _status.Rot; }
XMFLOAT3 BaseEnemy::GetScl() const { return  _status.Scl; }
/*******************************************************/




bool BaseEnemy::DeathJudg()
{
	//‘Ì—Í0‚ÅŽ€‚Ê
	bool l_judg = _status.HP <= 0;

	if (l_judg)return true;
	return false;
}
