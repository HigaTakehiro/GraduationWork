#include "RightMoveCommand.h"
#include "Vector2.h"

void RightMoveCommand::Execute(Player* player)
{
	//右方向への移動ベクトル
	const Vector2 moveVec = { 1.0f * player->GetSpeed(), 0.0f};
	player->AddPlayerVector(moveVec);
}
