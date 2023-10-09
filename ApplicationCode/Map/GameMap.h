#pragma once
#include"Object3d.h"
#include<memory.h>
#include<DirectXMath.h>
using namespace std;
using namespace DirectX;
class GameMap
{
private:
	enum Map {
		None = 0,
		Normal,
		Forest,
		Enemy,
	};

	struct Stage
	{
		unique_ptr<Object3d> stage_;
		XMFLOAT3 stagePos_;
		int num;
		Map state_;
	};


public:
	
	void LoadCsv();
	
	/// <summary>
	/// ������
	/// </summary>
	void Initalize();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	void CheckNowNumber(const XMFLOAT3& pos);
	
	int GetCount(const XMFLOAT3& pos);

	XMFLOAT3 GetNowMapPos();
	
private:
	
	unique_ptr<Object3d> map_[3][3];

	list<unique_ptr<Stage>> maps_;

	Stage* sta[3][3];

	XMFLOAT3 pos_[3][3];

	int count_ = 0;
};

