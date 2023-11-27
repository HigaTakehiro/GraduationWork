#include "BossBase.h"

#include "Collision.h"

void BossBase::RecvDamage(Vector3 pos)
{

}
void BossBase::RecvDamageFlash()
{
	if (!FlashF)return;

	if (++val > 60) {
		val = 0.f;
		FlashF = FALSE;
	} else {
		color_rgb.y = sinf(val);
		color_rgb.z = sinf(val);
	}
}
