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
class GameScene : public BaseScene
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

	void NextMap();
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
	//�A�C�e��
	std::unique_ptr<Ore> ore_;
	//�z�΃A�C�e��
	std::list<std::unique_ptr<Ore>> oreItems_;

	std::array<BaseEnemy*, 3> enemys_;
	OBB* _hummmerObb;

	std::unique_ptr<BossBase>boss_;
	//�e�L�X�g�`��
	TextDraw* text_;
	//���n��
	std::unique_ptr<GameMap> map_;
	//�J�������W
	Vector3 cameraPos_;
	//�����_
	Vector3 targetPos_;
	int count_ = 0;

	Shake* shake_;
	int oldcount_ = 0;
	float oldcamerapos_ = 0;
	float time_ = 0;
	XMFLOAT3 nextPos_{};
	int direction = 0;
};