#pragma once
#include"BaseScene.h"
#include"GameMap.h"

class StageOneFloor : public BaseScene
{
public: //�����o�֐�
/// <summary>
/// �������֐�
/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();
protected: //�ÓI�����o�֐�

private: //�����o�֐�
	/// <summary>
	/// �V�[���؂�ւ�����
	/// </summary>
	void SceneChange();

	/// <summary>
	/// �J�����������ݒ�
	/// </summary>
	void CameraSetting();
private: //�����o�ϐ�

		//���n��
	std::unique_ptr<GameMap> map_;
};