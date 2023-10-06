#pragma once
#include "Object3d.h"
#include "Model.h"

class Shapes
{
public:
	Shapes() = default;
	~Shapes() = default;
	//Shapes(const Shapes& obj) = delete;
	//Shapes& operator=(const Shapes& obj) = delete;

public: //静的メンバ関数

	/// <summary>
	/// 平面の三角形を生成
	/// </summary>
	/// <param name="vertices_1">頂点1</param>
	/// <param name="vertices_2">頂点2</param>
	/// <param name="vertices_3">頂点3</param>
	/// <returns>平面の三角形</returns>
	static Model* CreateTriangle(const DirectX::XMFLOAT2& vertices_1, const DirectX::XMFLOAT2& vertices_2, const DirectX::XMFLOAT2& vertices_3, const std::string& textureName = "white1x1.png");

	/// <summary>
	/// 平面の四角形を生成
	/// </summary>
	/// <param name="upperLeft">左上座標</param>
	/// <param name="lowerBottom">右下座標</param>
	/// <returns>平面の四角形</returns>
	static Model* CreateSquare(DirectX::XMFLOAT2 upperLeft, DirectX::XMFLOAT2 lowerBottom, const std::string& textureName = "white1x1.png", const DirectX::XMFLOAT2 imageSize = {0.0f, 0.0f});

private: //メンバ変数

};

