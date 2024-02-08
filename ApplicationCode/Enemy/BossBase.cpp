#include "BossBase.h"

#include <algorithm>
#include"ExternalFileLoader.h"
#include "Collision.h"
#include "Easing.h"
#include "Helper.h"
#include "ImageManager.h"

void BossBase::RecvDamage(Vector3 pos)
{

}
void BossBase::RecvDamageFlash()
{
	if (FlashF) {
		if (++val > 40) {
			val = 0.f;
			FlashF = FALSE;
		} else {
			color_rgb.y = sinf(val)*2;
			color_rgb.z = sinf(val)*2;
		}
		color_rgb.y = std::clamp(color_rgb.y, 0.4f, 2.f);
		color_rgb.z = std::clamp(color_rgb.z,0.4f,2.f);
	}
	else
	{
		val = 0;
		color_rgb = XMFLOAT4(1, 1, 1, color_rgb.w);
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

void BossBase::HPUiInit()
{
	m_HpTex = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 0, 0 });
	if(names=="Dogom")
	m_HpTex_Frame = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::BossBar, { 0, 0 });
	else if(names=="Togemaru")
		m_HpTex_Frame = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::BossBar_Toge, { 0, 0 });
	else if (names == "LastBoss")
		m_HpTex_Frame = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::BossBar_last, { 0, 0 });

	m_HpTex_Inner = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 0, 0 });

	//m_HpTex_Frame->SetColor(XMFLOAT3(0, 0, 0));
	m_HpTex_Inner->SetColor(XMFLOAT3(1, 1, 0));
	
}

void BossBase::HPUiUpda()
{
	if (m_BodyAlpha <= 0.f)return;
	float px = 850.f, py = 30.f;
	float sx, sy;
	//0~400‚ÌŠÔ‚Å‚Ì•âŠ®Žæ‚é
	sx = Helper::SmoothStep_Deb(0, BossMaxHP, m_HP) * 270.f;
	sy = 40.f;

	NowHP = sx;
	if (BodyRecvDam)
		bravegaugeF = TRUE;

	if (bravegaugeF)
	{
		if (++InnerSclingT <= 60.f)
			m_hpInnerSizeX = Easing::easeIn(InnerSclingT, 60.f, BeforeHP, NowHP - 1.f);
		else {
			bravegaugeF = FALSE;
		}
	} else
	{
		InnerSclingT = 0.f;
		BeforeHP = sx;
		m_hpInnerSizeX = sx;
	}

	InnerSclingT = std::clamp(InnerSclingT, 0.f, 60.f);

	m_HpTex_Frame->SetPosition(XMFLOAT2(px - 10.f, py - 20.f));
	m_HpTex_Frame->SetSize(XMFLOAT2(430.f, 70.f));

	m_HpTex_Inner->SetPosition(XMFLOAT2(px - 5.f, py - 15.f));
	m_HpTex_Inner->SetSize(XMFLOAT2(m_hpInnerSizeX, sy + 20.f));

	m_HpTex->SetColor(XMFLOAT3(0.8f, 0, 0));
	m_HpTex->SetPosition(XMFLOAT2(px - 5.f, py - 15.f));
	m_HpTex->SetSize(XMFLOAT2(sx, sy + 20.f));

}

void BossBase::HPUiDraw()
{
	
	m_HpTex_Inner->Draw();
	m_HpTex->Draw();m_HpTex_Frame->Draw();
}


