#pragma once
#include"BaseScene.h"
#include"GameMap.h"

class StageOneFloor : public BaseScene
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
protected: //静的メンバ関数

private: //メンバ関数
	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	void SceneChange();

	/// <summary>
	/// カメラ初期化設定
	/// </summary>
	void CameraSetting();
private: //メンバ変数

		//仮地面
	std::unique_ptr<GameMap> map_;
};