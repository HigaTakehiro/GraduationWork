#include "PadInput.h"

WinApp* PadInput::winApp = nullptr;
LPDIRECTINPUTDEVICE8 PadInput::devPad = nullptr;
DIJOYSTATE PadInput::padState = {};
LPDIRECTINPUT8 PadInput::p_Interface = nullptr; 

PadInput* PadInput::GetIns()
{
	static PadInput instance;
	return &instance;
}

BOOL PadInput::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	// 既に発見しているなら終了
	if (parameter->FindCount >= 1)
	{
		return DIENUM_STOP;
	}

	// デバイス生成
	HRESULT hr = p_Interface->CreateDevice(
		lpddi->guidInstance,
		parameter->GamePadDevice,
		NULL);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// 入力フォーマットの指定
	device = *parameter->GamePadDevice;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// プロパティの設定
	if (SetUpGamePadProperty(device) == false)
	{
		return DIENUM_STOP;
	}

	// 協調レベルの設定
	if (SetUpCooperativeLevel(device) == false)
	{
		return DIENUM_STOP;
	}

	// 発見数をカウント
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}
BOOL PadInput::SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device)
{
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Z軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Z;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// RX軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RX;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// RY軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RY;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// RZ軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RZ;
	auto hr = device->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(hr))
	{
		return true;
	}

	return true;
}
BOOL PadInput::SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device)
{
	//協調モードの設定
	if (FAILED(device->SetCooperativeLevel(
		winApp->GetHwnd(),
		DISCL_EXCLUSIVE | DISCL_FOREGROUND)
	))
	{
		return false;
	}

	return true;
}
BOOL PadInput::StartGamePadControl()
{
	// デバイスが生成されてない
	if (devPad == nullptr)
	{
		return false;
	}

	// 制御開始
	if (FAILED(devPad->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	devPad->GetCapabilities(&cap);
	// ポーリング判定
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		/*
			PollはAcquireの前に行うとされていたが、
			Acquireの前で実行すると失敗したので
			後で実行するようにした
		*/
		if (FAILED(devPad->Poll()))
		{
			return false;
		}
	}

	return true;
}

float PadInput::leftStickX()
{
	if (padState.lX < -unresponsive_range)
	{
		return padState.lX / 1000.0f;
		//is_push[ButtonKind::LeftButton] = true;
	}
	else if (padState.lX > unresponsive_range)
	{
		return padState.lX / 1000.0f;
		//is_push[ButtonKind::RightButton] = true;
	}

	return 0.0f;
}
float PadInput::leftStickY()
{
	if (padState.lY < -unresponsive_range)
	{
		return padState.lY / 1000.0f;
		//is_push[ButtonKind::UpButton] = true;
	}
	else if (padState.lY > unresponsive_range)
	{
		return padState.lY / 1000.0f;
		//is_push[ButtonKind::DownButton] = true;
	}

	return 0.0f;
}
float PadInput::rightStickX()
{
	if (padState.lRx < -unresponsive_range)
	{
		return padState.lRx / 1000.0f;
		//is_push[ButtonKind::LeftButton] = true;
	}
	else if (padState.lRx > unresponsive_range)
	{
		return padState.lRx / 1000.0f;
		//is_push[ButtonKind::RightButton] = true;
	}

	return 0.0f;
}
float PadInput::rightStickY()
{
	if (padState.lRy < -unresponsive_range)
	{
		return padState.lRy / 1000.0f;
		//is_push[ButtonKind::UpButton] = true;
	}
	else if (padState.lRy > unresponsive_range)
	{
		return padState.lRy / 1000.0f;
		//is_push[ButtonKind::DownButton] = true;
	}

	return 0.0f;
}

float PadInput::getLeftX()
{
	return padState.lX / 1000.0f;
}
float PadInput::getLeftY()
{
	return padState.lY / 1000.0f;
}
float PadInput::getRightX()
{
	return padState.lRx / 1000.0f;
}
float PadInput::getRightY()
{
	return padState.lRy / 1000.0f;
}

void PadInput::Initialize(WinApp* winApp) {
	HRESULT result = S_FALSE;

	//WinAppのインスタンスを記録
	this->winApp = winApp;
	//DirectInputのインスタンス生成
	result = DirectInput8Create(winApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	//パッドデバイス生成
	result = dinput->CreateDevice(GUID_Joystick, &devPad, NULL);
	if (devPad != nullptr) {
		//入力データ形式のセット
		result = devPad->SetDataFormat(&c_dfDIJoystick);
		//排他制御レベルのセット
		result = devPad->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}

	// インターフェース作成
	result = DirectInput8Create(
		GetModuleHandle(nullptr),	// インスタンスハンドル
		DIRECTINPUT_VERSION,		// DirectInputのバージョン
		IID_IDirectInput8,			// 使用する機能
		(void**)&p_Interface,	// 作成されたインターフェース代入用
		NULL						// NULL固定
	);

	DeviceEnumParameter parameter;

	parameter.FindCount = 0;
	parameter.GamePadDevice = &devPad;

	// GAMEPADを調べる
	p_Interface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// 検索するデバイスの種類
		DeviceFindCallBack,			// 発見時に実行する関数
		&parameter,					// 関数に渡す値
		DIEDFL_ATTACHEDONLY			// 検索方法
	);

	// JOYSTICKを調べる
	p_Interface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// どちらも見つけることが出来なかったら失敗
	if (parameter.FindCount == 0)
	{
		return;
	}

	int count = 0;
	// 制御開始
	while (StartGamePadControl() == false)
	{
		Sleep(100);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

	// 入力情報の初期化
	for (int i = 0; i < PadButton::Button_Max; i++)
	{
		p_ButtonState[i] = ButtonState::ButtonStateNone;
	}
}
void PadInput::Update() {
	HRESULT result;

	if (devPad != nullptr) {
		//前回のパッドのボタン押下を保存
		prePadState = padState;
		//パッド情報の更新
		result = devPad->Acquire();
		//パッドの入力情報を取得する
		result = devPad->GetDeviceState(sizeof(DIJOYSTATE), &padState);

		if (FAILED(result)) {

			if (FAILED(devPad->Acquire())) {
				for (int i = 0; i <Button_Max; i++)
				{
					p_ButtonState[i] = ButtonState::ButtonStateNone();
				}
				devPad->Poll();
			}
			return;
		}
	}

	bool is_push[PadButton::Button_Max] = { false };
	// スティック判定
	//int unresponsive_range = 200;
	if (padState.lX < -unresponsive_range)
	{
		is_push[PadButton::Stick_Left] = true;
	}
	else if (padState.lX > unresponsive_range)
	{
		is_push[PadButton::Stick_Right] = true;
	}

	if (padState.lY < -unresponsive_range)
	{
		is_push[PadButton::Stick_Up] = true;
	}
	else if (padState.lY > unresponsive_range)
	{
		is_push[PadButton::Stick_Down] = true;
	}

}

bool PadInput::PushButton(BYTE buttonNumber) {
	//指定キーを押していればtrueを返す
	if (padState.rgbButtons[buttonNumber] & (0x80)) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}
bool PadInput::TriggerButton(BYTE buttonNumber) {
	//前回は押していなくて、今回は押していればtrueを返す
	if (!prePadState.rgbButtons[buttonNumber] && padState.rgbButtons[buttonNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}
bool PadInput::ReleaseButton(BYTE buttonNumber)
{
	//前回は押していて、今回は押していなければtrueを返す
	if (prePadState.rgbButtons[buttonNumber] && !padState.rgbButtons[buttonNumber]) return true;
	return false;
}
bool PadInput::PushCrossKey(LONG crossKeyNumber) {
	if (padState.rgdwPOV[0] != 0xFFFFFFFF) {
		//指定キーを押していればtrueを返す
		if (padState.rgdwPOV[0] == crossKeyNumber) {
			return true;
		}
	}
	//そうでないならfalseを返す
	return false;
}