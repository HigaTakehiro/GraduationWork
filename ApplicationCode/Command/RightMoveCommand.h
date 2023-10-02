#pragma once
#include "Player.h"
#include "PlayerCommand.h"

class RightMoveCommand : public PlayerCommand
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~RightMoveCommand() = default;

public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="player"></param>
	void Execute(Player* player);
};

