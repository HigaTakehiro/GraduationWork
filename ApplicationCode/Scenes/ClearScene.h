#pragma once
#include "BaseScene.h"
class ClearScene :
    public BaseScene
{
public: //メンバ関数
/// <summary>
/// 初期化関数
/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();
private: //メンバ関数
/// <summary>
/// シーン切り替え処理
/// </summary>
	void SceneChange();

	/// <summary>
	/// カメラ初期化設定
	/// </summary>
	void CameraSetting();
};

