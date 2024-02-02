#pragma once
#include "Player.h"
#include <string>

class IActiveSkill {
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IActiveSkill() {};

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	virtual void Use(Player* player) = 0;

	/// <summary>
	/// スキル有効化フラグをセット
	/// </summary>
	/// <param name="isActive">スキル有効化フラグ</param>
	virtual void SetIsActive(bool isActive) = 0;

	/// <summary>
	/// スキル名を取得
	/// </summary>
	/// <returns>スキル名</returns>
	std::string GetName() const { return name_; }

	/// <summary>
	/// スキル有効化フラグを取得
	/// </summary>
	/// <returns>スキル有効化フラグ</returns>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// クールタイム中かどうかを取得
	/// </summary>
	/// <returns>クールタイム</returns>
	bool GetCoolTime() { return coolTimer_ >= coolTime_; }

protected: //メンバ変数
	//スキル名
	std::string name_;
	//スキル有効化フラグ
	bool isActive_;
	//クールタイム
	int32_t coolTime_;
	int32_t coolTimer_;
};