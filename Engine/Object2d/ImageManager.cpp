#include "ImageManager.h"

#include "Texture.h"

ImageManager* ImageManager::GetIns()
{
	static ImageManager instance;
	return &instance;
}

void ImageManager::Initialize() {
	//Sprite::LoadTexture(int32_t(ImageName::solidTexNumber),L"Engine/Resources/Images/golemicon.png");
	Texture::LoadTexture(Image2DName::AL, L"Engine/Resources/Images/golemicon.png");
}
