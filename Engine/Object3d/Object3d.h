#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include <memory>
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;	// ビュー変換行列
		XMMATRIX world; //ワールド行列
		XMFLOAT4 color; // 色
		XMFLOAT3 cameraPos; //カメラ座標
		float time; //時間
	};

	// 頂点シェーダ番号
	enum VSPipelineNo {
		Normal,
		Wave,
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, int32_t window_width, int32_t window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d* Create(Model* model);

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns></returns>
	static std::unique_ptr<Object3d> UniquePtrCreate(Model* model);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	//頂点シェーダー数
	static const int32_t vsSize = 2;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate[vsSize];
	//ライト
	static LightGroup* light;

private:// 静的メンバ関数
	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	/// <summary>
	/// 頂点シェーダー読み込み
	/// </summary>
	/// <param name="vsName">頂点シェーダー名</param>
	/// <param name="vsBlob">頂点シェーダーオブジェクト</param>
	static void LoadVS(const wchar_t* vsName, ComPtr<ID3DBlob>& vsBlob);

	/// <summary>
	/// ピクセルシェーダー読み込み
	/// </summary>
	/// <param name="psName">ピクセルシェーダー名</param>
	/// <param name="psBlob">ピクセルシェーダーオブジェクト</param>
	static void LoadPS(const wchar_t* psName, ComPtr<ID3DBlob>& psBlob);

public: //静的メンバ関数
	/// <summary>
	/// ライトをセット
	/// </summary>
	/// <param name="light">ライト</param>
	static void SetLight(LightGroup* light) { Object3d::light = light; }

public: // メンバ関数
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	/// <param name="maxTime">定数バッファに送る最大時間</param>
	void Update(const float maxTime = 0.0f);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// 大きさの取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetScale() { return scale; }

	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// 回転角の取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	//モデルのセット
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// ワールド変換行列の取得
	/// </summary>
	/// <returns>ワールド変換行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// 親オブジェクト設定(カメラ)
	/// </summary>
	void SetCameraParent(Camera* cameraParent) { this->cameraParent = cameraParent; }

	/// <summary>
	/// 親オブジェクト設定(obj)
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(Object3d* parent) { this->parent = parent; }

	/// <summary>
	/// アンビエントをセット
	/// </summary>
	/// <param name="ambient">アンビエント</param>
	void SetAmbient(const XMFLOAT3& ambient) { model->SetAmbient(ambient); }

	/// <summary>
	/// 色をセット
	/// </summary>
	/// <param name="color"></param>
	void SetColor(XMFLOAT4 color) { this->color = color; }

	/// <summary>
	/// ビルボード行列を掛ける
	/// </summary>
	/// <param name="isBillboard">ビルボードフラグ</param>
	void SetIsBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	/// <summary>
	/// Y軸ビルボード行列を掛ける
	/// </summary>
	/// <param name="isBillboardY">Y軸ビルボードフラグ</param>
	void SetIsBillboardY(bool isBillboardY) { this->isBillboardY = isBillboardY; }

	/// <summary>
	/// 波用シェーダー適用フラグをセット
	/// </summary>
	void SetIsWave(bool isWave) { this->isWave = isWave; }

	/// <summary>
	/// 親オブジェクト取得
	/// </summary>
	/// <returns>親オブジェクト</returns>
	Object3d* GetParent() { return parent; }

	/// <summary>
	/// 親オブジェクト(カメラ)取得
	/// </summary>
	/// <returns>親オブジェクト(カメラ)</returns>
	Camera* GetCameraParent() { return cameraParent; }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
	//親オブジェクト(カメラ)
	Camera* cameraParent = nullptr;
	//モデルデータ
	Model* model = nullptr;
	//カメラ
	Camera* camera = nullptr;
	//ビルボード行列フラグ
	bool isBillboard;
	//Y軸ビルボード行列フラグ
	bool isBillboardY;
	//波用シェーダー適用フラグ
	bool isWave;
	//シェーダー用タイマー
	float timer = 0.0f;
};

