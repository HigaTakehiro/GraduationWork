#pragma once
#include "IActiveSkill.h"
class HyperModeSkill : public IActiveSkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">スキル名</param>
	/// <param name="coolTime">クールタイム</param>
	/// <param name="activeTime">有効時間</param>
	HyperModeSkill(const std::string& name, int32_t coolTime, int32_t activeTime);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HyperModeSkill() override = default;

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Use(Player* player) override;

	/// <summary>
	/// スキル有効化
	/// </summary>
	/// <param name="isActive">有効化フラグ</param>
	void SetIsActive(bool isActive) override;

private: //メンバ変数
	//有効時間タイマー
	int32_t activeTimer_;
	//有効時間
	int32_t activeTime_;
};

