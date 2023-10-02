#pragma once
#include "Sprite.h"
#include "Vector2.h"

class Collision2d final
{
private:
	Collision2d() = default;
	~Collision2d() = default;
	Collision2d(const Collision2d& obj) = delete;
	Collision2d& operator=(const Collision2d& obj) = delete;

public: //静的メンバ関数
	/// <summary>
	/// インスタンス取得用関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static Collision2d* GetIns();

public: //メンバ関数
	/// <summary>
	/// 円と円の当たり判定
	/// </summary>
	/// <param name="obj1">判定したいスプライト1</param>
	/// <param name="obj2">判定したいスプライト2</param>
	/// <param name="radius1">判定したいスプライト半径1</param>
	/// <param name="radius2">判定したいスプライト半径2</param>
	/// <returns>当たっているかどうか</returns>
	bool CircleAndCircle(Sprite* obj1, Sprite* obj2, float radius1, float radius2);
	/// <summary>
	/// 円と円の当たり判定
	/// </summary>
	/// <param name="pos1">判定したいスプライト座標1</param>
	/// <param name="pos2">判定したいスプライト座標2</param>
	/// <param name="radius1">判定したいスプライト半径1</param>
	/// <param name="radius2">判定したいスプライト半径2</param>
	/// <returns>当たっているかどうか</returns>
	bool CircleAndCircle(Vector2 pos1, Vector2 pos2, float radius1, float radius2);
};

