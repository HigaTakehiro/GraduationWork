#include "KeyInput.h"
#include "WinApp.h"
#include "DirectXSetting.h"
#include "Sound.h"
#include "PostEffect.h"
#include "MouseInput.h"
#include "PadInput.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "LightGroup.h"
#include "SoundManager.h"
#include "ExternalFileLoader.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t)
{
	const int32_t debugTextNumber = 0;

	//ポインタ置き場
	WinApp* winApp = nullptr;
	DirectXSetting* dxCommon = DirectXSetting::GetIns();
	SceneManager* scene = nullptr;
	Sound* sound = nullptr;
	ExternalFileLoader* externalFileLoader = nullptr;

	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->SetWindowStyle(WS_SYSMENU | WS_CAPTION | WS_EX_CONTEXTHELP | WS_MINIMIZEBOX);
	winApp->Initialize();

	//DirectXの初期化
	dxCommon->Initialize(winApp);

	//入力の初期化
	KeyInput::GetIns()->Initialize(winApp);
	MouseInput::GetIns()->Initialize(winApp);
	PadInput::GetIns()->Initialize(winApp);

	//Soundの初期化
	sound = new Sound();
	if (!sound->Initialize()) {
		assert(0);
		return 1;
	}
	SoundManager::GetIns()->Initialize(sound);

	//Sprite & DebugTextの初期化
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	Sprite::LoadTexture(debugTextNumber, L"Engine/Resources/Images/debugfont.png");
	DebugText debugText; //デバッグテキスト表示用クラス
	debugText.Initialize(debugTextNumber);

	ImageManager::GetIns()->Initialize();

	//FBXの初期化
	FbxLoader::GetInstance()->Initialize(DirectXSetting::GetIns()->GetDev());
	FBXObject3d::SetDevice(DirectXSetting::GetIns()->GetDev());
	FBXObject3d::CreateGraphicsPipeline();

	//Object3dの初期化
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	ModelManager::GetIns()->Initialize();
	LightGroup::StaticInitialize();

	scene = new SceneManager();
	scene->Initialize();

	// DirectX初期化処理　ここまで

	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		KeyInput::GetIns()->Update();
		MouseInput::GetIns()->Update();
		PadInput::GetIns()->Update();
		scene->Update();

		// DirectX毎フレーム処理　ここまで
		// ４．描画コマンドここから
		scene->Draw();
	}

	//scene解放
	scene->Finalize();
	safe_delete(scene);
	//WindowsAPI解放
	winApp->Finalize();
	safe_delete(winApp);
	return 0;
}