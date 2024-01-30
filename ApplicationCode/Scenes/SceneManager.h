#pragma once
#include "BaseScene.h"
#include "TitleScene.h"
#include"TutorialScene.h"
#include "GameScene.h"
#include"IBScene.h"
#include"SkillScene.h"
#include "BossScene.h"
#include "ResultScene.h"
#include "DirectXSetting.h"
#include "Sound.h"
#include "KeyInput.h"
#include "SafeDelete.h"
#include "TextDraw.h"
#include "CollisionManager.h"
#include "SkillManager.h"

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

public: //�V�[���Ǘ��ԍ�
	enum class SceneName {
		Title,
		Tutorial,
		Game,
		IB,
		SKILL,
		Boss,
		Boss2,
		Boss3,
		Result,
		None
	};

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

public: //�ÓI�����o�֐�

	/// <summary>
	/// �X�V
	/// </summary>
	static void Update();

	/// <summary>
	/// �`��
	/// </summary>
	static void Draw();

	/// <summary>
	/// �V�[���؂�ւ�
	/// </summary>
	static void SceneChange(SceneName scene);

	/// <summary>
	/// �X�R�A�����Z
	/// </summary>
	/// <param name="score">�X�R�A</param>
	static void AddScore(const int32_t score);

	/// <summary>
	/// �X�R�A���Z�b�g
	/// </summary>
	/// <param name="score"></param>
	static void SetScore(const int32_t score);

	/// <summary>
	/// �X�e�[�W�ԍ����Z�b�g
	/// </summary>
	static void SetStageNo(const int32_t stageNo);

	/// <summary>
	/// �����蔻��}�l�[�W���[���Z�b�g
	/// </summary>
	/// <param name="colManager">�����蔻��N���X</param>
	static void SetCollsionManager(CollisionManager* colManager) { colManager_ = colManager; }

	/// <summary>
	/// HP���Z�b�g
	/// </summary>
	/// <param name="hp">HP</param>
	static void SetHP(int32_t hp) { hp_ = hp; }

	/// <summary>
	/// HP���擾
	/// </summary>
	/// <returns>HP</returns>
	static int32_t GetHP() { return hp_; }

	/// <summary>
	/// �ő�HP���Z�b�g
	/// </summary>
	/// <param name="maxHp">�ő�HP</param>
	static void SetMaxHP(int32_t maxHp) { maxHp_ = maxHp; }

	/// <summary>
	/// �ő�HP���擾
	/// </summary>
	/// <returns>�ő�HP</returns>
	static int32_t GetMaxHP() { return maxHp_; }

	/// <summary>
	/// �U���͂��Z�b�g
	/// </summary>
	/// <param name="atk">�U����</param>
	static void SetATK(int32_t atk) { atk_ = atk; }

	/// <summary>
	/// �U���͂��擾
	/// </summary>
	/// <returns>�U����</returns>
	static int32_t GetATK() { return atk_; }

	/// <summary>
	/// ����͂��Z�b�g
	/// </summary>
	/// <param name="def">�����</param>
	static void SetDEF(int32_t def) { def_ = def; }

	/// <summary>
	/// ����͂��擾
	/// </summary>
	/// <returns>�����</returns>
	static int32_t GetDEF() { return def_; }

	/// <summary>
	/// �f�������Z�b�g
	/// </summary>
	/// <param name="spd">�f����</param>
	static void SetSPD(int32_t spd) { spd_ = spd; }

	/// <summary>
	/// �f�������擾
	/// </summary>
	/// <returns>�f����</returns>
	static int32_t GetSPD() { return spd_; }

	/// <summary>
	/// ���x�����Z�b�g
	/// </summary>
	/// <param name="level">���x��</param>
	static void SetLevel(int32_t level) { level_ = level; }

	/// <summary>
	/// ���x�����擾
	/// </summary>
	/// <returns>���x��</returns>
	static int32_t GetLevel() { return level_; }

	/// <summary>
	/// �o���l���Z�b�g
	/// </summary>
	/// <param name="ep">�o���l</param>
	static void SetEP(int32_t ep) { ep_ = ep; }

	/// <summary>
	/// �o���l���擾
	/// </summary>
	/// <returns>�o���l</returns>
	static int32_t GetEP() { return ep_; }

	/// <summary>
	/// �X�L���|�C���g���Z�b�g
	/// </summary>
	/// <param name="skillPoint">�X�L���|�C���g</param>
	static void SetSkillPoint(int32_t skillPoint) { skillPoint_ = skillPoint; }

	/// <summary>
	/// �X�L���|�C���g���擾
	/// </summary>
	/// <returns>�X�L���|�C���g</returns>
	static int32_t GetSkillPoint() { return skillPoint_; }

	/// <summary>
	/// �X�e�[�W�ԍ��擾
	/// </summary>
	/// <returns>�X�e�[�W�ԍ�</returns>
	static int32_t GetStageNo() { return stageNo_; }

	/// <summary>
	/// �X�R�A�擾
	/// </summary>
	/// <returns>�X�R�A</returns>
	static int32_t GetScore() { return score; }

private: //�ÓI�����o�֐�
	/// <summary>
	/// �V�[������������
	/// </summary>
	static void NowSceneInitialize();

private: //�ÓI�����o�ϐ�
	static BaseScene* nowScene;
	static BaseScene* skillScene_;
	static int32_t stageNo_;
	static int32_t score;
	static CollisionManager* colManager_;
	static SkillManager* skillManager_;
	//�v���C���[���x��
	static int32_t level_;
	//�o���l
	static int32_t ep_;
	//HP
	static int32_t hp_;
	//�ő�HP
	static int32_t maxHp_;
	//�U����
	static int32_t atk_;
	//�h���
	static int32_t def_;
	//�f����
	static int32_t spd_;
	//�X�L���|�C���g
	static int32_t skillPoint_;
	//�X�L���V�[���t���O
	static bool isSkillScene_;

private: //�����o�ϐ�
	std::unique_ptr<TextDraw> textDraw;
};