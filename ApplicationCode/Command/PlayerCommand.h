#pragma once
#include "Player.h"

class PlayerCommand {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerCommand() {}

public: //メンバ関数
	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="player"></param>
	virtual void Execute(Player* player) = 0;
};