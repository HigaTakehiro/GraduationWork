#include "ImageManager.h"

#include "Texture.h"

ImageManager* ImageManager::GetIns()
{
	static ImageManager instance;
	return &instance;
}

void ImageManager::Initialize() {
	//Sprite::LoadTexture(int32_t(ImageName::solidTexNumber),L"Engine/Resources/Images/golemicon.png");
	Texture::LoadTexture(Image2DName::USA_1, L"Engine/Resources/Images/usa/usa_idle1.png");
	Texture::LoadTexture(Image2DName::USA_2, L"Engine/Resources/Images/usa/usa_idle2.png");
	Texture::LoadTexture(Image2DName::USA_3, L"Engine/Resources/Images/usa/usa_idle3.png");

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
