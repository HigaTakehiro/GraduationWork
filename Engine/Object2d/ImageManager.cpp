#include "ImageManager.h"

#include "Texture.h"

ImageManager* ImageManager::GetIns()
{
	static ImageManager instance;
	return &instance;
}

void ImageManager::Initialize() {
	Sprite::LoadTexture(int32_t(ImageName::background),L"Engine/Resources/Images/Background.png");
	Sprite::LoadTexture(int32_t(ImageName::bar), L"Engine/Resources/Images/white1x1.png");
	Sprite::LoadTexture(int32_t(ImageName::skill), L"Engine/Resources/Images/skillBack.png");
	Sprite::LoadTexture(int32_t(ImageName::skillButton), L"Engine/Resources/Images/sukillB.png");
	Sprite::LoadTexture(int32_t(ImageName::susumuButton), L"Engine/Resources/Images/susumu.png");
	Sprite::LoadTexture(int32_t(ImageName::Arrow), L"Engine/Resources/Images/arrow2.png");
	Sprite::LoadTexture(int32_t(ImageName::filter), L"Engine/Resources/Images/titleFilter.png");
	Sprite::LoadTexture(int32_t(ImageName::title), L"Engine/Resources/Images/TitleLOGO.png");
	Sprite::LoadTexture(int32_t(ImageName::asist_), L"Engine/Resources/Images/susumuA.png");
	Sprite::LoadTexture(int32_t(ImageName::wake), L"Engine/Resources/Images/wakeUpA.png");
	Sprite::LoadTexture(int32_t(ImageName::TextWindow), L"Engine/Resources/Images/window.png");
	Sprite::LoadTexture(int32_t(ImageName::skillCursor), L"Engine/Resources/Images/Cursor_normalUI.png");
	Texture::LoadTexture(Image2DName::USA_1, L"Engine/Resources/Images/usa/usa_idle1.png");
	Texture::LoadTexture(Image2DName::USA_2, L"Engine/Resources/Images/usa/usa_idle2.png");
	Texture::LoadTexture(Image2DName::USA_3, L"Engine/Resources/Images/usa/usa_idle3.png");
	Sprite::LoadTexture(Image2DName::IdlePlayer, L"Engine/Resources/Images/tuyu_idle.png");
	Sprite::LoadTexture((UINT)ImageName::skillIcon_HammerReturn, L"Engine/Resources/Images/skillPanel_hammerReturn.png");
	Sprite::LoadTexture((UINT)ImageName::skillIcon_HPUp, L"Engine/Resources/Images/skillPanel_hpUP.png");
	Sprite::LoadTexture((UINT)ImageName::skillIcon_ATKUp, L"Engine/Resources/Images/skillPanel_powUP.png");
	Sprite::LoadTexture((UINT)ImageName::skillIcon_DEFUp, L"Engine/Resources/Images/skillPanel_defUP.png");
	Sprite::LoadTexture((UINT)ImageName::skillIcon_SPDUp, L"Engine/Resources/Images/skillPanel_speedUP.png");
	Sprite::LoadTexture((UINT)ImageName::BossBar, L"Engine/Resources/Images/dogomuHP-export.png");
	Sprite::LoadTexture((UINT)ImageName::BossBar_Toge, L"Engine/Resources/Images/togemaruHP-export.png");
	Sprite::LoadTexture((UINT)ImageName::Flash, L"Engine/Resources/Images/Flash.png");
	Sprite::LoadTexture((UINT)ImageName::PlayerUIBack, L"Engine/Resources/Images/playerHPBar.png");
	Sprite::LoadTexture((UINT)ImageName::skillIcon_FallHammer, L"Engine/Resources/Images/skillPanel_fallHammer.png");
	Sprite::LoadTexture((UINT)ImageName::skillIcon_HyperMode, L"Engine/Resources/Images/skillPanel_hyperMode.png");
	Sprite::LoadTexture((UINT)ImageName::skillIcon_Empty, L"Engine/Resources/Images/skillPanel_empty.png");
	Sprite::LoadTexture((UINT)ImageName::levelUp, L"Engine/Resources/Images/LEVELUP.png");

	Texture::LoadTexture(Image2DName::BUSA1, L"Engine/Resources/Images/usa_back/usa_back1.png");
	Texture::LoadTexture(Image2DName::BUSA2, L"Engine/Resources/Images/usa_back/usa_back2.png");
	Texture::LoadTexture(Image2DName::BUSA3, L"Engine/Resources/Images/usa_back/usa_back3.png");

	Texture::LoadTexture(Image2DName::RUSA1, L"Engine/Resources/Images/usa_right/usa_right1.png");
	Texture::LoadTexture(Image2DName::RUSA2, L"Engine/Resources/Images/usa_right/usa_right2.png");
	Texture::LoadTexture(Image2DName::RUSA3, L"Engine/Resources/Images/usa_right/usa_right3.png");


	Texture::LoadTexture(Image2DName::LUSA1, L"Engine/Resources/Images/usa_left/usa_left1.png");
	Texture::LoadTexture(Image2DName::LUSA2, L"Engine/Resources/Images/usa_left/usa_left2.png");
	Texture::LoadTexture(Image2DName::LUSA3, L"Engine/Resources/Images/usa_left/usa_left3.png");

	Sprite::LoadTexture(Image2DName::STONE, L"Engine/Resources/Images/frontRock1.png");
}
