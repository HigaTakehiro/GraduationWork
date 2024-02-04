#pragma once
#include "Sprite.h"
#include "TextDraw.h"
#include "Vector2.h"

class SkillPanel
{
public: //enum
	enum SkillType {
		HammerReturn,
		HPUP,
		ATKUP,
		DEFUP,
		SPDUP,
		FallHammer,
		HyperMode,
		Empty,
		None
	};

public: //�����o�֐�

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SkillPanel();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="skillName">�X�L����</param>
	void Initialize(const std::wstring& skillName, const Vector2& pos, SkillType type, int32_t num = 0);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Vector2 cursorPos);

	/// <summary>
	/// �X�v���C�g�`�揈��
	/// </summary>
	void SpriteDraw();

	/// <summary>
	/// �e�L�X�g�`�揈��
	/// </summary>
	void TextMessageDraw();

	/// <summary>
	/// �X�L���擾��Ԃ��擾
	/// </summary>
	/// <returns>�X�L���擾���</returns>
	bool GetIsSkillGet() { return isSkillGet_; }

	/// <summary>
	/// �X�L���擾��Ԃ��Z�b�g
	/// </summary>
	void SetIsSkillGet(bool isSkillGet) { isSkillGet_ = isSkillGet; }

	/// <summary>
	/// �擾�\��Ԃ��擾
	/// </summary>
	/// <returns>�擾�\���</returns>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// �擾�\��Ԃ��Z�b�g
	/// </summary>
	void SetIsActive(bool isActive) { isActive_ = isActive; }

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	Vector2 GetPos() { return pos_; }

	/// <summary>
	/// ���W���Z�b�g
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const Vector2& pos) { pos_ = pos; }

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="cursorPos">�J�[�\�����W</param>
	void TextOpen(Vector2 cursorPos);

	/// <summary>
	/// �p�l���ɃJ�[�\�����G��Ă��邩�ǂ���
	/// </summary>
	/// <param name="cursorPos">�J�[�\�����W</param>
	/// <returns>�J�[�\�����p�l���ɐG��Ă��邩�ǂ���</returns>
	bool PanelToCursorHit(Vector2 cursorPos);

	/// <summary>
	/// �X�L���^�C�v�擾
	/// </summary>
	/// <returns>�X�L���^�C�v</returns>
	SkillType GetSkillType() { return skillType_; }

	/// <summary>
	/// �X�e�[�^�X�㏸���l�擾
	/// </summary>
	/// <returns>�X�e�[�^�X�㏸���l</returns>
	int32_t GetStatusUpNum() { return num_; }

	/// <summary>
	/// �X�L���������ǂݍ��ݏ���
	/// </summary>
	void SkillTextLoad();

	/// <summary>
	/// �X�L���^�C�v���Z�b�g
	/// </summary>
	/// <param name="skillType">�X�L���^�C�v</param>
	void SetSkillType(SkillType type);

	/// <summary>
	/// �X�L���p�l�������擾
	/// </summary>
	/// <returns>�X�L���p�l����</returns>
	std::wstring GetSkillName() { return skillName_; }

private: //�����o�ϐ�
	//�X�L����
	std::wstring skillName_;
	//�X�L��������
	std::wstring skillText_;
	//�`��X�L��������
	std::wstring drawText_;
	//�X�L���擾�t���O
	bool isSkillGet_;
	//�擾�\��ԃt���O
	bool isActive_;
	//�p�l���I���t���O
	bool isSelect_;

	//�X�L���p�l���X�v���C�g
	std::unique_ptr<Sprite> skillPanel_;
	//�e�L�X�g�`��N���X
	TextDraw* text_;
	//���W
	Vector2 pos_;
	//�X�e�[�^�X�㏸���l
	int32_t num_;
	//�X�L���^�C�v
	SkillType skillType_;
	//�������\���^�C�}�[
	int32_t drawTimer_;
	int32_t drawTime_;
};

