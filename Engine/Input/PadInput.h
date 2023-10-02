#pragma once

#include <Windows.h>
#include <wrl.h>
#include "WinApp.h"

#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class PadInput final
{
public: //enum
	enum PadButton {
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_LB,
		Button_RB,
		Button_View,
		Button_Menu,
		Button_LS,
		Button_RS,
		Stick_Up,
		Stick_Down,
		Stick_Left,
		Stick_Right,
		Button_Max,
	};

	enum PadCrossKey {
		CrossKey_Neutral = -1,
		CrossKey_Up = 0,
		CrossKey_UpperRight = 4500,
		CrossKey_Right = 9000,
		CrossKey_LowerRight = 13500,
		CrossKey_Lower = 18000,
		CrossKey_LowerLeft = 22500,
		CrossKey_Left = 27000,
		CrossKey_UpperLeft = 31500
	};

	enum ButtonState {
		ButtonStateNone,
		ButtonStateDown,
		ButtonStatePush,
		ButtonStateUp,
		ButtonStateMax,
	};

	//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
	struct DeviceEnumParameter
	{
		LPDIRECTINPUTDEVICE8* GamePadDevice;
		int FindCount;
	};

private: //エイリアス
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PadInput() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PadInput() = default;

	/// <summary>
	/// コピーコンストラクタを禁止
	/// </summary>
	PadInput(const PadInput& obj) = delete;

	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	static BOOL SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device);

	static BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device);

	static BOOL StartGamePadControl();

	/// <summary>
	/// 代入演算子を禁止
	/// </summary>
	PadInput& operator=(const PadInput& obj) = delete;

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ボタン押下検知
	/// </summary>
	bool PushButton(BYTE buttonNumber);

	/// <summary>
	/// ボタンを押した瞬間のみ検知
	/// </summary>
	bool TriggerButton(BYTE buttonNumber);

	/// <summary>
	/// 十字キー押下検知
	/// </summary>
	bool PushCrossKey(LONG crossKeyNumber);

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static PadInput* GetIns();

	float leftStickX();
	float leftStickY();

	float rightStickX();
	float rightStickY();

	static float getLeftX();
	static float getLeftY();

	static float getRightX();
	static float getRightY();

private: //メンバ変数
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput;
	//コントローラのデバイス
	static LPDIRECTINPUTDEVICE8 devPad;
	//コントローラの情報
	static DIJOYSTATE padState;
	DIJOYSTATE prePadState;
	ButtonState p_ButtonState[Button_Max];

	static LPDIRECTINPUT8 p_Interface;

	const int unresponsive_range = 200;

	//WindowsAPI
	static WinApp* winApp;
};


