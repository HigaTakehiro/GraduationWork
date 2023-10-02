#include "KeyInputHandler.h"
#include "SafeDelete.h"

KeyInputHandler::~KeyInputHandler()
{
	safe_delete(wKey_);
	safe_delete(aKey_);
	safe_delete(sKey_);
	safe_delete(dKey_);
	safe_delete(leftClick_);
	safe_delete(rightClick_);
	safe_delete(rKey_);
	safe_delete(leftShiftKey_);
	safe_delete(spaceKey_);
}

void KeyInputHandler::Initialize(Player* player)
{
	player_ = player;
	KeyBindChange_W(Commands::UpMove);
	KeyBindChange_A(Commands::LeftMove);
	KeyBindChange_S(Commands::DownMove);
	KeyBindChange_D(Commands::RightMove);	
	KeyBindChange_LShift(Commands::None);
	KeyBindChange_Space(Commands::None);
}

void KeyInputHandler::PlayerHandleInput()
{
	if (KeyInput::GetIns()->PushKey(DIK_W) && wKey_ != nullptr) wKey_->Execute(player_);
	if (KeyInput::GetIns()->PushKey(DIK_A) && aKey_ != nullptr) aKey_->Execute(player_);
	if (KeyInput::GetIns()->PushKey(DIK_S) && sKey_ != nullptr) sKey_->Execute(player_);
	if (KeyInput::GetIns()->PushKey(DIK_D) && dKey_ != nullptr) dKey_->Execute(player_);
	if (KeyInput::GetIns()->PushKey(DIK_R) && rKey_ != nullptr) rKey_->Execute(player_);
	if (KeyInput::GetIns()->TriggerKey(DIK_LSHIFT) && leftShiftKey_ != nullptr) leftShiftKey_->Execute(player_);
	if (KeyInput::GetIns()->TriggerKey(DIK_SPACE) && spaceKey_ != nullptr) spaceKey_->Execute(player_);
	if (MouseInput::GetIns()->PushClick(MouseInput::MouseState::LEFT_CLICK) && leftClick_ != nullptr) leftClick_->Execute(player_);
	if (MouseInput::GetIns()->TriggerClick(MouseInput::MouseState::RIGHT_CLICK) && rightClick_ != nullptr) rightClick_->Execute(player_);
}

PlayerCommand* KeyInputHandler::KeyBindChange(Commands command)
{
	switch (command) {
	case Commands::LeftMove:
		return new LeftMoveCommand();
		break;
	case Commands::RightMove:
		return new RightMoveCommand();
		break;
	case Commands::UpMove:
		return new UpMoveCommand();
		break;
	case Commands::DownMove:
		return new DownMoveCommand();
		break;
	case Commands::None:
		return nullptr;
		break;
	default:
		return nullptr;
		break;
	}
}

void KeyInputHandler::KeyBindChange_W(Commands command)
{
	wKey_ = KeyBindChange(command);
}

void KeyInputHandler::KeyBindChange_A(Commands command)
{
	aKey_ = KeyBindChange(command);
}

void KeyInputHandler::KeyBindChange_S(Commands command)
{
	sKey_ = KeyBindChange(command);
}

void KeyInputHandler::KeyBindChange_D(Commands command)
{
	dKey_ = KeyBindChange(command);
}

void KeyInputHandler::KeyBindChange_LeftClick(Commands command)
{
	leftClick_ = KeyBindChange(command);
}

void KeyInputHandler::KeyBindChange_RightClick(Commands command)
{
	rightClick_ = KeyBindChange(command);
}

void KeyInputHandler::KeyBindChange_R(Commands command)
{
	rKey_ = KeyBindChange(command);
}

void KeyInputHandler::KeyBindChange_Space(Commands command)
{
	spaceKey_ = KeyBindChange(command);
}

void KeyInputHandler::KeyBindChange_LShift(Commands command)
{
	leftShiftKey_ = KeyBindChange(command);
}
