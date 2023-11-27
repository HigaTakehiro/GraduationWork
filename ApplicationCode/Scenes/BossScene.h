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
class BossScene :
    public BaseScene
{
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

private:
	//�|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect_;
	//�|�X�g�G�t�F�N�g�ԍ�
	PostEffect::PostEffectNo postEffectNo_;
	//���C�g
	std::unique_ptr<LightGroup> light_;
	//�J����
	std::unique_ptr<Camera> camera_;
	//
	std::unique_ptr<Stairs>m_Stairs;
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
	std::unique_ptr<Sprite>m_ClearTex = nullptr;
	XMFLOAT2 m_ClearTexScl;
	float ClearTexEaseT;
	bool NextClearF;
	bool touchFlor;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};
	SceneChangeEffect* schange;
};

