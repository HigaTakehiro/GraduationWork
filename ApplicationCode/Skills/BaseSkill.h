#pragma once
#include "Player.h"
#include <stdint.h>
#include <string>

class BaseSkill {
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseSkill() {};

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player"></param>
	virtual void UseSkill(Player* player) = 0;

public: //メンバ関数
	/// <summary>
	/// スキルがアクティブ状態かどうかを取得
	/// </summary>
	bool GetIsActive() { return isActive_; }
	/// <summary>
	/// スキル名を取得
	/// </summary>
	/// <returns></returns>
	std::string GetSkillName() { return skillName_; }
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="skillName">スキル名</param>
	/// <param name="activeTime">スキル使用時間</param>
	/// <param name="recastTime">スキル再使用時間</param>
	void Initialize(std::string skillName, int32_t activeTime = 0, int32_t recastTime = 0);

protected: //メンバ変数
	//スキル名
	std::string skillName_;
	//アクティブフラグ
	bool isActive_;

	//スキル使用時間
	int32_t activeTime_;
	//スキル使用タイマー
	int32_t activeTimer_;
	//スキル再使用タイム
	int32_t recastTime_;
	//スキル再使用タイマー
	int32_t recastTimer_;
};
