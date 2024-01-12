#pragma once
#include <DirectXMath.h>
#include "FbxLoader.h"
#include "FBXObject3d.h"
#include "SoundManager.h"
#include "ImageManager.h"
#include "Sprite.h"
#include "Easing.h"
#include "SafeDelete.h"
#include "CollisionManager.h"
#include "SkillManager.h"

class BaseScene
{
public:
	//�f�X�g���N�^
	virtual ~BaseScene() {}

public: //���z�֐�
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;

protected: //���z�����o�֐�
	/// <summary>
	/// �V�[���؂�ւ�����
	/// </summary>
	virtual void SceneChange() = 0;

public: //�����o�֐�
	/// <summary>
	/// �����蔻��N���X���Z�b�g
	/// </summary>
	/// <param name="colManager">�����蔻��N���X</param>
	void SetCollisionManager(CollisionManager* colManager) { colManager_ = colManager; }
	/// <summary>
	/// �X�L���Ǘ��N���X���Z�b�g
	/// </summary>
	/// <param name="skillManager">�X�L���Ǘ��N���X</param>
	void SetSkillManager(SkillManager* skillManager) { skillManager_ = skillManager; }

protected: //�����o�ϐ�
	//�����蔻��N���X
	CollisionManager* colManager_;
	//�X�L���Ǘ��N���X
	SkillManager* skillManager_;
};