#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include "Vector3.h"
#include "TextDraw.h"
#include "ISkill.h"

class Player
{
public: //�����o�֐�
	/// <summary>
	/// ������
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

	/// <summary>
	/// ��]�p�擾
	/// </summary>
	/// <returns>��]�p</returns>
	Vector3 GetRot() { return rot_; }

	/// <summary>
	/// �z�Ύ擾����
	/// </summary>
	void AddOreCount() { oreCount_++; }

	/// <summary>
	/// �n���}�[�z�����܂��
	/// </summary>
	void HammeronHole();

	/// <summary>
	/// �z�Ύ擾�����Z�b�g
	/// </summary>
	void ResetOreCount() { oreCount_ = 0; }

	/// <summary>
	/// �����ݒ�
	/// </summary>
	/// <param name="vec"></param>
	void HitHammerToEnemy(Vector3 vec, float dis = 1.f);

	void SetPos(Vector3 pos) { this->pos_ = pos; }

	void SetStop(bool stop) { this->stop_ = stop; }

	bool GetStop() { return stop_; }

	/// <summary>
	/// �n���}�[�����t���O���擾
	/// </summary>
	/// <returns>�n���}�[�����t���O</returns>
	bool GetIsHammerRelease() { return isHammerRelease_; }

	/// <summary>
	/// �U���t���O���擾
	/// </summary>
	/// <returns>�U���t���O</returns>
	bool GetIsAttack() { return isAttack_; }

	/// <summary>
	/// �n���}�[��]�U���t���O���擾
	/// </summary>
	/// <returns></returns>
	bool GetIsHammerSwing() { return isHammerSwing_; }

	/// <summary>
	/// ���G��ԃt���O���擾
	/// </summary>
	/// <returns>���G��ԃt���O</returns>
	bool GetIsInvincible() { return isInvincible_; }

	bool GetNotNext() { return notnext_; }

	/// <summary>
	/// �z�Ύ擾�����擾
	/// </summary>
	/// <returns>�z�Ύ擾����</returns>
	float GetOreCountRate() { return (float)oreCount_ / (float)maxOreCount_; }

	/// <summary>
	/// HP���擾
	/// </summary>
	/// <returns>HP</returns>
	int32_t GetHP() { return hp_; }

	/// <summary>
	/// HP���Z�b�g
	/// </summary>
	/// <param name="hp">HP</param>
	void SetHP(int32_t hp) { hp_ = hp; }

	/// <summary>
	/// �̗͂̍ő�l���擾
	/// </summary>
	/// <returns></returns>
	int32_t GetMaxHP() { return maxHp_; }

	/// <summary>
	/// HP�̍ő�l�����Z
	/// </summary>
	/// <param name="addNum">���Z���鐔�l</param>
	void AddMaxHP(int32_t addNum) { maxHp_ += addNum; }

	/// <summary>
	/// HP�̍ő�l���Z�b�g
	/// </summary>
	/// <param name="maxHp">�ő�HP</param>
	void SetMaxHP(int32_t maxHp) { maxHp_ = maxHp; }

	/// <summary>
	/// ���x�����擾
	/// </summary>
	/// <returns>���x��</returns>
	int32_t GetLevel() { return level_; }

	/// <summary>
	/// ���x�����Z�b�g
	/// </summary>
	/// <param name="level">���x��</param>
	void SetLevel(int32_t level) { level_ = level; }

	/// <summary>
	/// �o���l���擾
	/// </summary>
	/// <returns>�o���l</returns>
	int32_t GetEP() { return ep_; }

	/// <summary>
	/// HP�����Z
	/// </summary>
	/// <param name="addHP">���Z����HP</param>
	void AddHP(int32_t addHP) { hp_ += addHP; }

	/// <summary>
	/// �o���l�����Z
	/// </summary>
	/// <param name="addEP">���Z�o���l</param>
	void AddEP(int32_t addEP) { ep_ += addEP; }

	/// <summary>
	/// �o���l���Z�b�g
	/// </summary>
	/// <param name="ep">�o���l</param>
	void SetEP(int32_t ep) { ep_ = ep; }

	/// <summary>
	/// �X�v���C�g�`��
	/// </summary>
	void SpriteDraw();

	/// <summary>
	/// HP�����Z
	/// </summary>
	/// <param name="subHP">���Z����HP</param>
	void SubHP(int32_t subHP);

	void SetNextFlor(bool flag) { nextflor_ = flag; }

	bool GetNextFlor() { return nextflor_; }

	bool GetNext() { return next_; }

	/// <summary>
	/// �n���}�[���˃t���O���擾
	/// </summary>
	/// <returns></returns>
	bool GetIsHammerReflect() { return isHammerReflect_; }

	/// <summary>
	/// �n���}�[���˃t���O���Z�b�g
	/// </summary>
	/// <param name="isReflect"></param>
	void SetIsHammerReflect(bool isReflect) { isHammerReflect_ = isReflect; }

	/// <summary>
	/// �����`��
	/// </summary>
	void TextUIDraw();

	/// <summary>
	/// �`���[�g���A���p�A�v�f
	/// </summary>
	void TutorialUpdate(bool Stop, bool NotAttack);

	/// <summary>
	/// �z�΃J�E���g���ő�擾�ʈȏォ�ǂ������擾
	/// </summary>
	/// <returns></returns>
	bool OreCountOverMaxCount();

	/// <summary>
	/// ���S�t���O���擾
	/// </summary>
	/// <returns>���S�t���O</returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// �U���͂��擾
	/// </summary>
	/// <returns>�U����</returns>
	int32_t GetATK() { return atk_; }

	/// <summary>
	/// �_���[�W�v�Z�Ɏg�p����U���͂��擾
	/// </summary>
	/// <returns>�U����</returns>
	int32_t GetDamageATK();

	/// <summary>
	/// �h��͂��擾
	/// </summary>
	/// <returns>�h���</returns>
	int32_t GetDef() { return def_; }

	/// <summary>
	/// �f�������擾
	/// </summary>
	/// <returns>�f����</returns>
	int32_t GetSPD() { return spd_; }

	/// <summary>
	/// �U���͂��Z�b�g
	/// </summary>
	/// <param name="atk">�U����</param>
	void SetATK(int32_t atk) { atk_ = atk; }

	/// <summary>
	/// �h��͂��Z�b�g
	/// </summary>
	/// <param name="def">�h���</param>
	void SetDEF(int32_t def) { def_ = def; }

	/// <summary>
	/// �f�������Z�b�g
	/// </summary>
	/// <param name="spd">�f����</param>
	void SetSPD(int32_t spd) { spd_ = spd; }

	/// <summary>
	/// ���x���A�b�v�K�v�o���l���Z�b�g
	/// </summary>
	/// <param name="lvUpEp">���x���A�b�v�K�v�o���l</param>
	void SetLevelUpEP(int32_t lvUpEp) { levelUpEp_ = lvUpEp; }

	/// <summary>
	/// ���x���A�b�v�K�v�o���l���擾
	/// </summary>
	/// <returns>���x���A�b�v�K�v�o���l</returns>
	int32_t GetLevelUpEP() { return levelUpEp_; }

	/// <summary>
	/// �U���͂����Z
	/// </summary>
	/// <param name="num">�㏸���l</param>
	void AddATK(int32_t num) { atk_ += num; }

	/// <summary>
	/// ����͂����Z
	/// </summary>
	/// <param name="num">�㏸���l</param>
	void AddDEF(int32_t num) { def_ += num; }

	/// <summary>
	/// �f���������Z
	/// </summary>
	/// <param name="num">�㏸���l</param>
	void AddSPD(int32_t num) { spd_ += num; }

	/// <summary>
	/// �n���}�[�߂菈��
	/// </summary>
	void HammerReturn();

	/// <summary>
	/// �X�L���|�C���g���擾
	/// </summary>
	/// <returns>�X�L���|�C���g</returns>
	int32_t GetSkillPoint() { return skillPoint_; }

	/// <summary>
	/// �X�L���|�C���g�����Z
	/// </summary>
	/// <param name="num">�������l</param>
	void SubSkillPoint(int32_t num) { skillPoint_ -= num; }

	/// <summary>
	/// �X�L���|�C���g�����Z
	/// </summary>
	/// <param name="num">�㏸���l</param>
	void AddSkillPoint(int32_t num) { skillPoint_ += num; }

	/// <summary>
	/// �X�L���|�C���g���Z�b�g
	/// </summary>
	/// <param name="skillPoint">�X�L���|�C���g</param>
	void SetSkillPoint(int32_t skillPoint) { skillPoint_ = skillPoint; }

	/// <summary>
	/// ���G��ԃt���O���Z�b�g
	/// </summary>
	/// <param name="isInvicible">���G��ԃt���O</param>
	void SetIsInvicible(bool isInvicible) { isInvincible_ = isInvicible; }

	/// <summary>
	/// �t�H�[���n���}�[�U���L����
	/// </summary>
	void ActiveFallHammer();

	void StatusReset();

private: //�����o�֐�

	/// <summary>
	/// �v���C���[�X�e�[�^�X�̐ݒ�
	/// </summary>
	void PlayerStatusSetting();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �n���}�[����
	/// </summary>
	void HammerThrow();

	/// <summary>
	/// �n���}�[����
	/// </summary>
	void HammerGet();

	/// <summary>
	/// �n���}�[��������
	/// </summary>
	void HammerPowerUp();

	/// <summary>
	/// ��������
	/// </summary>
	void Repulsion();

	/// <summary>
	/// �A�j���[�V��������
	/// </summary>
	void Animation();

	/// <summary>
	/// UI����
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// ���x���A�b�v����
	/// </summary>
	void LevelUp();

	/// <summary>
	/// ���G���ԏ���
	/// </summary>
	void HitCoolTime();

	/// <summary>
	/// ���S���o
	/// </summary>
	void DeadAction();

	//�����U������
	/// <summary>
	/// �t�H�[���n���}�[�U��
	/// </summary>
	void FallHammerAttack();



private: //�����o�ϐ�
	//���W
	Vector3 pos_;
	//�傫��
	Vector3 scale_;
	//��]�p
	Vector3 rot_;
	//HP
	int32_t hp_;
	int32_t preHp_;
	int32_t maxHp_;
	int32_t initHP_;
	//�U����
	int32_t atk_ = 1;
	//�h���
	int32_t def_ = 1;
	//�f����
	int32_t spd_ = 1;
	int32_t oldSpd_ = 1;
	//�X�L���|�C���g
	int32_t skillPoint_;
	//���x���A�b�v���o�^�C�}�[
	int32_t lvUpTimer_;
	//���x���A�b�v���o�X�v���C�g
	std::unique_ptr<Sprite> lvUpSprite_;

	//���x��
	int32_t level_ = 0;
	//�o���l
	int32_t ep_;
	//���x���A�b�v�K�v�o���l
	int32_t levelUpEp_;
	//�o���l�{��
	float magEp_;
	//�z�Ύ擾��
	int32_t oreCount_;
	//�z�΍ő�擾��
	int32_t maxOreCount_;
	//�U����
	int32_t attackPoint_;
	//�z�Ύ擾���ɉ������U���͔{��
	float oreCountMag_[6];
	//�z�Ύ擾���Ƃɑ傫������T�C�Y
	Vector3 hammerSizeUp_;
	//�z�Ύ擾���Ƃ̉�]���x�W��
	float hammerRotCoeff_;
	//���G����
	int32_t hitCoolTime_;
	//���G���ԃ^�C�}�[
	int32_t hitCoolTimer_;
	//���S�t���O
	bool isDead_;
	//���S���o�^�C�}�[
	int32_t deadTimer_;
	//���S���o����
	int32_t deadTime_ = 2 * 60;
	//�z�����܂�t���O
	bool onHoleF;
	//
	bool isJudg_Hole;
	//����
	bool fallF;
	//�ړ����x
	float moveSpeed_;
	//��]���x
	float rotSpeed_;
	//��]���Z�b�g�^�C�}�[
	float rotResetTimer_;
	//��]���Z�b�g����
	float rotResetTime_;
	//������]�p
	Vector3 initRot_;
	//������]���x
	float initRotSpeed_;
	//�����U����
	int32_t initAtkPoint_;
	//�ō��ړ����x
	float maxMoveSpeed_;
	//�ō��n���}�[�ړ����x
	float maxHammerMoveSpeed_;
	//�����x
	Vector3 acc_;
	//�n���}�[�U���������x
	float hammerAcc_;

	//�v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> player_;
	//��]�U�����v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> rotAttackPlayer_;
	//�v���C���[���f��
	//�ʏ펞
	Model* playerModel_[4];
	//�ړ����f��
	//�O
	Model* frontMoveModel_[4];
	//��
	Model* backMoveModel_[4];
	//�E
	Model* rightMoveModel_[4];
	//��
	Model* leftMoveModel_[4];
	//��]�U�����f��
	Model* rotAttackModel_[4];
	//���S���f��
	Model* deadModel_;
	//�e�I�u�W�F�N�g
	std::unique_ptr<Object3d> shadow_;
	//�e
	Model* shadowModel_;

	//�n���}�[�I�u�W�F�N�g
	std::unique_ptr<Object3d> hammer_;
	//�n���}�[���f��
	Model* hammerModel_;
	//�n���}�[�����t���O
	bool isHammerRelease_;
	//�U���t���O
	bool isAttack_;
	//�s����~
	bool isStop_;
	//�n���}�[��]�t���O
	bool isHammerSwing_;
	//�n���}�[���˃t���O
	bool isHammerReflect_;
	//�n���}�[���W
	Vector3 hammerPos_;
	//�n���}�[�T�C�Y
	Vector3 hammerSize_;
	//�n���}�[�������x
	float throwSpeed_;
	//�n���}�[�����x�N�g��
	Vector3 hammerVec_;
	//�n���}�[�����l
	Vector3 initHammerPos_;
	Vector3 initHammerScale_;
	Vector3 initHammerRot_;
	//�A�j���[�V�����J�E���g
	int32_t animeCount_;
	//�A�j���[�V�����X�s�[�h
	int32_t animeSpeed_;
	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�O�t���[���A�j���[�V�����J�E���g
	int32_t preAnimeCount_;

	//���I�u�W�F�N�g
	std::unique_ptr<Object3d> arrow_;
	//��󃂃f��
	Model* arrowModel_;

	//�n���}�[�������p�x
	float hammerThrowRot_;

	//�����U���x�N�g��
	Vector3 repulsionVec_;
	//�����U�����x
	float repulsionSpeed_;
	//�����W��
	float repulsionPower_;
	//������~�߂�
	bool stop_ = false;
	//�U�����ɃX�e�[�W�ړ����Ȃ��l��
	bool notnext_ = false;
	//�K�i�̃`�F�b�N
	bool nextflor_ = false;

	bool next_ = false;
	//���G��ԃt���O
	bool isInvincible_ = false;
	//�t�H�[���n���}�[�U���t���O
	bool isFallHammer_ = false;
	bool isPreFallHammer_ = false;
	int32_t fallHammerTimer_ = 0;

	//UI
	//�X�e�[�^�X�o�[
	std::unique_ptr<Sprite> statusBack_;
	//HP�o�[
	std::unique_ptr<Sprite> hpBar_;
	float hpBarSize_;
	//�o���l�o�[
	std::unique_ptr<Sprite> epBar_;
	float epBarSize_;
	//�e�L�X�g
	TextDraw* text_;

	bool look_ = false;
	bool animationflag_ = false;
	bool sestop_ = true;



public:
	Object3d* GetHammer() { return hammer_.get(); }
	Vector3 GetPos() { return player_->GetMatWorld().r[3]; }

	DirectX::XMFLOAT3 Get() { return pos_; }
	bool GetisHit() { return player_->GetIsHit(); }
	bool getisHammerActive() {
		if (isHammerRelease_ || isHammerSwing_)return true; return false;
	}
	void SetStopF(bool flag) { isStop_ = flag; }

	Vector3 GetHammmerPos()const { return hammer_->GetMatWorld().r[3]; }

	void SetisJudgHole(bool f) { isJudg_Hole = f; }
};

