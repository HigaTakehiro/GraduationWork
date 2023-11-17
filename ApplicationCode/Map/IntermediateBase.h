#pragma once
#include"Object3d.h"
#include<memory.h>
#include<DirectXMath.h>
#include <fstream>
using namespace std;
class IntermediateBase
{
public:
	void Initialize();
	void Update();
	void Draw();
	/// <summary>
	/// �Z�[�u
	/// </summary>
	/// <param name="floor"></param>
	void FloorSave(int floor);
	/// <summary>
	/// ���[�h
	/// </summary>
	void LoadFloor();

	int GetBaseNo() { return baseNo; }
private:
	unique_ptr<Object3d> stage_;
	int baseNo = 0;
};

