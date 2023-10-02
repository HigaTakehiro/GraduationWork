#pragma once
#include "Player.h"
#include "PlayerCommand.h"

class LeftMoveCommand : public PlayerCommand
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LeftMoveCommand() = default;

public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="player"></param>
	void Execute(Player* player);
};

