#include "ImageManager.h"

ImageManager* ImageManager::GetIns()
{
	static ImageManager instance;
	return &instance;
}

void ImageManager::Initialize() {
	//Sprite::LoadTexture(int32_t(ImageName::solidTexNumber),L"Engine/Resources/Images/solid.png");

}
