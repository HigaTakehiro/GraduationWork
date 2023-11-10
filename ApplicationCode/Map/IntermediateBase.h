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
	void FloorSave(int floor);
private:
	unique_ptr<Object3d> stage_;
};

