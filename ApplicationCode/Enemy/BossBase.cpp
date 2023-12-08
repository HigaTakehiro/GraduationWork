#include "BossBase.h"

#include "Collision.h"

void BossBase::RecvDamage(Vector3 pos)
{

}
void BossBase::RecvDamageFlash()
{
	if (FlashF) {
		if (++val > 60) {
			val = 0.f;
			FlashF = FALSE;
		} else {
			color_rgb.y = sinf(val);
			color_rgb.z = sinf(val);
		}
	}
	else
	{
		val = 0;
		color_rgb = XMFLOAT4(1, 1, 1, 1);
	}
}

void BossBase::ArmDamageFlash(bool& f, int& t,XMFLOAT4&c)
{
	if (!f)return;

	if (++t > 90) {
		t = 0.f;
		f = FALSE;
	} else {
		c.y = sinf(t);
		c.z = sinf(t);
	}
}