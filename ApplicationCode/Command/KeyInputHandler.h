#pragma once
#include "KeyInput.h"
#include "MouseInput.h"
#include "PlayerCommand.h"
#include "LeftMoveCommand.h"
#include "RightMoveCommand.h"
#include "UpMoveCommand.h"
#include "DownMoveCommand.h"
#include "Player.h"

class KeyInputHandler
{
public: //列挙型

	enum class Commands {
		LeftMove,
		RightMove,
		UpMove,
		DownMove,
		None
	};

public: //メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	KeyInputHandler() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeyInputHandler();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player);

	/// <summary>
	/// プレイヤー関連のキー入力受付
	/// </summary>
	void PlayerHandleInput();

	/// <summary>
	/// Wキー割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_W(Commands command);

	/// <summary>
	/// Aキー割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_A(Commands command);

	/// <summary>
	/// Sキー割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_S(Commands command);

	/// <summary>
	/// Dキー割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_D(Commands command);

	/// <summary>
	/// 左クリック割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_LeftClick(Commands command);

	/// <summary>
	/// 右クリック割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_RightClick(Commands command);

	/// <summary>
	/// Rキー割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_R(Commands command);

	/// <summary>
	/// Spaceキー割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_Space(Commands command);

	/// <summary>
	/// 左Shiftキー割り当て変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	void KeyBindChange_LShift(Commands command);

private: //メンバ関数
	/// <summary>
	/// キーバインド変更
	/// </summary>
	/// <param name="command">割り当てるコマンド</param>
	PlayerCommand* KeyBindChange (Commands command);

private: //メンバ変数
	//プレイヤー
	Player* player_;

	//Wキーに割り当てているコマンド
	PlayerCommand* wKey_;
	//Aキーに割り当てているコマンド
	PlayerCommand* aKey_;
	//Sキーに割り当てているコマンド
	PlayerCommand* sKey_;
	//Dキーに割り当てているコマンド
	PlayerCommand* dKey_;
	//左クリックに割り当てているコマンド
	PlayerCommand* leftClick_;
	//右クリックに割り当てているコマンド
	PlayerCommand* rightClick_;
	//左Shiftキーに割り当てているコマンド
	PlayerCommand* leftShiftKey_;
	//Rキーに割り当てているコマンド
	PlayerCommand* rKey_;
	//Spaceキーに割り当てているコマンド
	PlayerCommand* spaceKey_;

};
