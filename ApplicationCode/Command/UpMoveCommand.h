#pragma once
#include "Player.h"
#include "PlayerCommand.h"

class UpMoveCommand : public PlayerCommand
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~UpMoveCommand() = default;

public:
	/// <summary>
	/// コマンド実行
	/// </summary>
	/// <param name="player"></param>
	void Execute(Player* player);
};

