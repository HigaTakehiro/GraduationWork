#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <unordered_map>

class Model
{
private: // エイリアス
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};
	//定数バッファ用データ構造体
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};
	//マテリアル
	struct Material
	{
		std::string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
		float alpha;
		std::string textureFilename;

		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

private: //メンバ変数
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
	//マテリアル
	Material material;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB1;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;

	//板ポリ変数
	//板ポリフラグ
	bool isBoardPolygon_;
	//アンカーポイント
	DirectX::XMFLOAT2 anchorpoint_;
	//サイズ
	DirectX::XMFLOAT2 size_;
	//テクスチャ左上座標
	DirectX::XMFLOAT2 texBase_;
	//テクスチャ右下座標
	DirectX::XMFLOAT2 texSize_;
	//左右反転
	bool isFlip_;

public: //静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 生成
	/// </summary>
	static Model* CreateModel(const std::string& modelname, bool isSmoothing = false);

	/// <summary>
	/// 図形モデル生成
	/// </summary>
	/// <param name="vertices_">頂点</param>
	/// <param name="indices_">頂点インデックス</param>
	/// <param name="textureName">テクスチャ名</param>
	/// <returns></returns>
	static Model* CreateShapeModel(const std::vector<VertexPosNormalUv>& vertices_, const std::vector<unsigned short> indices_, const std::string& textureName);

public: //メンバ関数
	/// <summary>
	/// マテリアルの取得
	/// </summary>
	/// <returns></returns>
	Material GetMaterial() { return material; }

	/// <summary>
	/// アンビエントをセット
	/// </summary>
	/// <param name="ambient"></param>
	void SetAmbient(const XMFLOAT3& ambient) { material.ambient = ambient; }

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// マテリアルの読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// モデル初期化
	/// </summary>
	void InitializeModel(const std::string& modelname, bool isSmoothing);

	/// <summary>
	/// 図形モデル初期化
	/// </summary>
	/// <param name="vertices_">頂点データ</param>
	/// <param name="indices_">頂点インデックス</param>
	/// <param name="textureName">テクスチャ名</param>
	void InitializeShapesModel(const std::vector<VertexPosNormalUv>& vertices_, const std::vector<unsigned short> indices_, const std::string& textureName);

	/// <summary>
	/// エッジ平滑化用データを追加
	/// </summary>
	/// <param name="indexPosition">座標インデックス</param>
	/// <param name="indexVertex">頂点インデックス</param>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// 平滑化した頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(Material material);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// アンカーポイント(画像の中心)をセット
	/// </summary>
	/// <param name="anchorpoint">アンカーポイント</param>
	void SetAnchorpoint(const DirectX::XMFLOAT2& anchorpoint);

	/// <summary>
	/// サイズをセット
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const DirectX::XMFLOAT2& size);

	/// <summary>
	/// テクスチャ画像の切り抜き範囲をセット
	/// </summary>
	/// <param name="texBase">左上座標</param>
	/// <param name="texSize">右下座標</param>
	void SetTexRect(const DirectX::XMFLOAT2& texBase, const DirectX::XMFLOAT2& texSize);

	/// <summary>
	/// 反転フラグをセット
	/// </summary>
	/// <param name="isFlip">反転フラグ</param>
	void SetIsFlip(bool isFlip = true) { isFlip_ = isFlip; }

private: //メンバ関数
	/// <summary>
	/// 拡張子を分離する
	/// </summary>
	/// <param name="filePath">拡張子を分離したいファイルパス</param>
	/// <returns>拡張子</returns>
	std::wstring SeparatedExtension(const std::wstring& filePath);

	/// <summary>
	/// 頂点データを更新する
	/// </summary>
	void TransferVertex();

};