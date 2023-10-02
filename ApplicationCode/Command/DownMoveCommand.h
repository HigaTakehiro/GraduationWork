#pragma once
#include "Player.h"
#include "PlayerCommand.h"

class DownMoveCommand : public PlayerCommand
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~DownMoveCommand() = default;

public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="player"></param>
	void Execute(Player* player);
};

