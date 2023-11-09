#pragma once
#include "Sprite.h"

class ImageManager final
{
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager& obj) = delete;
	ImageManager& operator=(const ImageManager& obj) = delete;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static ImageManager* GetIns();

public: //�摜��
	enum class ImageName {
		background = 1,
	};

	enum Image2DName {
		USA_1,
		USA_2,
		USA_3,
		//
		BUSA1,
		BUSA2,
		BUSA3,
		//
		//
		RUSA1,
		RUSA2,
		RUSA3,
		//
		LUSA1,
		LUSA2,
		LUSA3,
		//
		STONE
	};
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

private:

};

