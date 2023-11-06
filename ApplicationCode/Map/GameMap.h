#pragma once
#include"Object3d.h"
#include"Player.h"
#include"Stairs.h"

#include<memory.h>
#include<DirectXMath.h>
using namespace std;
using namespace DirectX;

class Player;
class GameMap
{
private:
	enum Map {
		None = 0,
		Normal,
		Forest,
		Enemy,
		Kaidan,
		Boss,
		Start,
	};

	enum Direction {
		Vertical = 0,
		Beside,
	};

	struct Stage
	{
		unique_ptr<Object3d> stage_;
		XMFLOAT3 stagePos_;
		int num;
		Map state_;
	};

	struct Bridge
	{
		unique_ptr<Object3d> bridge_;
		int num;
		Direction state_;
	};

public:

	void LoadCsv(Player* player);

	void CreateBridge();

	/// <summary>
	/// ������
	/// </summary>
	void Initalize(Player* player);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, float OldCameraPos);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	void CheckHitTest(Player* player);

	void CheckHitBridge(const XMFLOAT3& pos, int& Direction);

	int NextCount(const XMFLOAT3& pos, int& Direction);

	void SetStop(bool flag) { this->stopCount_ = flag; }

	XMFLOAT3 GetStartPos() { return startpos_; }

	XMFLOAT3 GetNowMapPos();

	void NextMap(Player* player,XMFLOAT3& CameraPos,XMFLOAT3& TargetPos,float OldCameraPos);

private:

	list<unique_ptr<Stage>> maps_;

	list<unique_ptr<Bridge>> bridge;

	unique_ptr<Stairs> stairs_;
	//�}�b�v�̔ԍ�
	int count_ = 0;
	//�Â���Ԃ̃}�v�ԍ�
	int oldcount_ = 0;
	//
	bool stopCount_ = false;

	bool nothit_ = false;

	int nextval_ = 0;
	//�C�[�W���O�p�̎���
	float time_ = 0;
	//���̊p�x���玟�̍��W�̈ʒu�����߂�
	int direction_ = 0;
	//�v���C���[�̊J�n�ʒu
	XMFLOAT3 startpos_{};

};


