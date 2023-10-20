#include "BossBase.h"

#include "Collision.h"

void BossBase::RecvDamage(Vector3 pos)
{
	if(!m_DamF)
	{
		if (Collision::GetLength(m_Hummmer, pos) < 3.f) {
			m_HP -= 10;
			DamCool = 10;
			m_DamF = TRUE;
		}
		else
			DamCool = 0;
	}

	if(m_DamF)
	{
		if (--DamCool <= 0)m_DamF = FALSE;
	}
}
