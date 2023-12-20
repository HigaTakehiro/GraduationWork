#pragma once
#include"Object3d.h"
#include"Player.h"
#include"Stairs.h"
#include"Grass.h"
#include "Deposit.h"
#include "Ore.h"
#include"BaseEnemy.h"
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
		bool stop = false;
	};

	struct Bridge
	{
		unique_ptr<Object3d> bridge_;
		int num;
		Direction state_;
	};

	struct Grassland
	{
		unique_ptr<Grass> grass_;
		int num;
	};


public:

	void LoadCsv(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, int StageNum);

	void CreateBridge();

	void CreateGrass(const XMFLOAT3& MapPos,int Count);

	void CreateDeposits(const XMFLOAT3& MapPos, int MapNum);

	void CreateEnemy(Player* player,const XMFLOAT3& MapPos, int Enemy);

	/// <summary>
	/// ������
	/// </summary>
	void Initalize(Player* player,XMFLOAT3& CameraPos,XMFLOAT3& TargetPos,int StageNum);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, float OldCameraPos,bool flag=true);

	/// <summary>
	/// �`��
	/// </summary>
	void MapDraw();

	void BridgeDraw(bool flag = true);

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

	Deposit* GetDePosit();
	bool DepositIsHit(bool flag) { return deposit_->GetIsHit(flag); }

	int GetCount() { return count_; }

	void NextMap(Player* player,XMFLOAT3& CameraPos,XMFLOAT3& TargetPos,float OldCameraPos);

	void DrawingMap(int StageNum,std::stringstream& stream);

	bool EnemyAllKill();

	void CreateRock();

	bool CheckRockToMap(const XMFLOAT3& RockPos);

	bool ReflectHammer(XMFLOAT3& Pos, bool isHammerRelease);

	/// <summary>
	/// �z�����X�g���擾
	/// </summary>
	/// <returns>�z��</returns>
	unique_ptr<Deposit>& GetDeposit(int32_t number) { return deposits_[number]; }
	/// <summary>
	/// �z�����X�g�T�C�Y���擾
	/// </summary>
	/// <returns>�z�����X�g�T�C�Y</returns>
	int32_t GetDepositsSize() { return deposits_.size(); }

	/// <summary>
	/// �G���X�g���擾
	/// </summary>
	/// <returns>�z��</returns>
	unique_ptr<BaseEnemy>& GetEnemy(int32_t number) { return enemys_[number]; }

	/// <summary>
	/// �G���X�g�T�C�Y���擾
	/// </summary>
	/// <returns>�z�����X�g�T�C�Y</returns>
	int32_t GetEnemySize() { return enemys_.size(); }

private:

	list<unique_ptr<Stage>> maps_;

	list<unique_ptr<Bridge>> bridge;

	unique_ptr<Stairs> stairs_;

	list<unique_ptr<Object3d>> rock_;
	
	list<unique_ptr<Grassland>> grass_;

	//�z�΃A�C�e��
	std::list<std::unique_ptr<Ore>> oreItems_;

	Deposit* deposit_;
	std::vector<std::unique_ptr<Deposit>> deposits_;

	vector<unique_ptr<BaseEnemy>> enemys_;

	//�}�b�v�̔ԍ�
	int count_ = 0;
	//�Â���Ԃ̃}�v�ԍ�
	int oldcount_ = 0;
	
	int enemyscount_ = 0;

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

	int nowstate_ = 0;
	//��̍��W
	XMFLOAT3 rockPos_{};

	XMFLOAT4 limit_ = { 10.f,10.f,8.8f,12.f };
};


