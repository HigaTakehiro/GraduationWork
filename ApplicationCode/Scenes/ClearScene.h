#pragma once
#include "BaseScene.h"
class ClearScene :
    public BaseScene
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
private: //�����o�֐�
/// <summary>
/// �V�[���؂�ւ�����
/// </summary>
	void SceneChange();

	/// <summary>
	/// �J�����������ݒ�
	/// </summary>
	void CameraSetting();
};

