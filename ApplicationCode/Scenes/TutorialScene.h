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
#include "SceneChange.h"
#include "MessageWindow.h"

class TutorialScene :
	public BaseScene
{
private:
    enum Phase {
		Title,
        Description,
        Move,
        Spown,
        Fight,
        Defeat,
        Free,
    }phase_;

private:
    static void (TutorialScene::* FuncTable[])();

public:
    void Initialize();

    void Update();

    void Draw();

    void Finalize();

private:
	/// <summary>
	/// �V�[���؂�ւ�����
	/// </summary>
	void SceneChange();
    /// <summary>
    /// �J�����������ݒ�
    /// </summary>
    void CameraSetting();

/// <summary>
/// �G�̏���
/// </summary>
	void EnemyProcess();

public:

	void TitlePhase();

    void DescriptionPhase();

    void MovePhase();

    void SpownPhase();

    void FightPhase();

    void DefeatPhase();

    void FreePhase();
private:
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
	//�^�C�g��
	unique_ptr<Sprite> titlefilter_;
	unique_ptr<Sprite> title_[9];
	unique_ptr<Sprite> asist_;
	unique_ptr<Object3d> sleep_;
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
	//���f��
	Model* sleepModel_[4];

	Model* titleModel_[9];

	//�J�������W
	Vector3 cameraPos_;
	//�����_
	Vector3 targetPos_;

	Shake* shake_;
	//�e�L�X�g�E�B���h�E
	MessageWindow* textWindow_;
	MessageWindow* fighttextwindow_;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};

	bool action_ = false;

	float timer_ = 0;

	int description_ = 0;

	bool notlook_ = false;

	XMFLOAT3 startpos_ = {};

	bool titlepos_ = true;

	XMFLOAT2 size_ = { 1280.f,720.f };
	XMFLOAT2 titleposition_{};
	SceneChangeEffect* scange;

	//�A�j���[�V�����J�E���g
	int32_t animeCount_;
	//�A�j���[�V�����X�s�[�h
	int32_t animeSpeed_=8;
	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�O�t���[���A�j���[�V�����J�E���g
	int32_t preAnimeCount_;


	//�A�j���[�V�����J�E���g
	int32_t titleanimeCount_=0;
	//�A�j���[�V�����X�s�[�h
	int32_t titleanimeSpeed_ = 8;
	//�A�j���[�V�����^�C�}�[
	int32_t titleanimeTimer_;
	//�O�t���[���A�j���[�V�����J�E���g
	int32_t titlepreAnimeCount_;


	bool stop_ = true;

	bool notattack_ = true;

	float movetimer_ = 0;
};
