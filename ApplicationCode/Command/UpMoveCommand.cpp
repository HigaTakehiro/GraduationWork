#include "UpMoveCommand.h"
#include "Vector2.h"

void UpMoveCommand::Execute(Player* player)
{
	//ã•ûŒü‚Ö‚ÌˆÚ“®ƒxƒNƒgƒ‹
	const Vector2 moveVec = { 0.0f, -1.0f*player->GetSpeed()};
	player->AddPlayerVector(moveVec);
}
