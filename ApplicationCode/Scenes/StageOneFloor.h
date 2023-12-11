#pragma once
#include "BaseScene.h"
#include "PostEffect.h"
#include "SafeDelete.h"
#include "MouseInput.h"
#include "Object3d.h"
#include "Vector3.h"
#include "Camera.h"
#include "MouseInput.h"
#include "Easing.h"
#include "Shapes.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "DebugText.h"
#include "MotionMath.h"
#include "SceneManager.h"
#include "TextDraw.h"
#include "JsonLoader.h"
#include "Player.h"
#include"GameMap.h"
#include "Ore.h"
#include"BossBase.h"
#include"BaseEnemy.h"
#include"Shake.h"
#include"IntermediateBase.h"
#include "Grass.h"

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

	/// <summary>
	/// �G�Ɋ֌W���鏈��
	/// </summary>
	void EnemyProcess();
private: //�����o�ϐ�
	//�|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect_;
	//�|�X�g�G�t�F�N�g�ԍ�
	PostEffect::PostEffectNo postEffectNo_;
	//���C�g
	std::unique_ptr<LightGroup> light_;
	//�J����
	std::unique_ptr<Camera> camera_;
	//�v���C���[
	Player* player_;
	//�z�΃A�C�e��
	std::list<std::unique_ptr<Ore>> oreItems_;

	std::vector<BaseEnemy*> enemys_;
	std::vector<Vector3> vec;
	OBB* _hummmerObb;

	std::unique_ptr<BossBase>boss_;
	//�e�L�X�g�`��
	TextDraw* text_;
	//���n��
	std::unique_ptr<GameMap> map_;
	//�w�i�摜
	std::unique_ptr<Sprite> background_;
	//�J�������W
	Vector3 cameraPos_;
	//�����_
	Vector3 targetPos_;

	Shake* shake_;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};
	Vector3 pos{};
	Vector3 target{};
	SceneChangeEffect* schange;
};