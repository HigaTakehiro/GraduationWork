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
}
