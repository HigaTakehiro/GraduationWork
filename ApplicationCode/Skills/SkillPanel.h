#pragma once
#include "Sprite.h"
#include "TextDraw.h"
#include "Vector2.h"

class SkillPanel
{
public: //�����o�֐�

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SkillPanel();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="skillName">�X�L����</param>
	void Initialize(const std::wstring& skillName, const Vector2& pos);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

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

private: //�����o�ϐ�
	//�X�L����
	std::wstring skillName_;
	//�X�L���擾�t���O
	bool isSkillGet_;
	//�擾�\��ԃt���O
	bool isActive_;

	//�X�L���p�l���X�v���C�g
	std::unique_ptr<Sprite> skillPanel_;
	//�e�L�X�g�`��N���X
	TextDraw* text_;
	//���W
	Vector2 pos_;
};

