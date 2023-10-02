#pragma once
#include <DirectXMath.h>
#include "FbxLoader.h"
#include "FBXObject3d.h"
#include "SoundManager.h"
#include "ImageManager.h"
#include "Sprite.h"
#include "Easing.h"
#include "SafeDelete.h"

class BaseScene
{
public:
	//デストラクタ
	virtual ~BaseScene() {}

public: //仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

protected: //仮想メンバ関数
	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	virtual void SceneChange() = 0;

public: //メンバ関数

protected: //静的メンバ変数

};