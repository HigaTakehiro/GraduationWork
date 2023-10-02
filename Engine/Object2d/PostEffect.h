#pragma once
#include "Sprite.h"
#include "Vector3.h"

class PostEffect : public Sprite
{
public: //構造体
	struct ConstBuffDataB0 {
		float time;
		float maxTime;
		float mask;
		float blurCenterX;
		float blurCenterY;
	};

	enum PostEffectNo {
		NONE,
		FADEOUT,
		NORMAL,
		DAMAGE,
		SLOW,
		DASH,
		SHOUT,
		POSTSHOUT,
	};

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="LT">左上</param>
	/// <param name="LB">左下</param>
	/// <param name="RT">右上</param>
	/// <param name="RB">右下</param>
	void Initialize(const Vector3& LT, const Vector3& LB, const Vector3& RT, const Vector3& RB);

	/// <summary>
	/// 描画コマンド
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, const float maxTime, PostEffectNo pipelineNo = NONE, bool isRoop = false);

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState();

	/// <summary>
	/// ピクセルシェーダー読み込み
	/// </summary>
	void LoadPS(const wchar_t* psName, ComPtr<ID3DBlob>& psBlob);

	/// <summary>
	/// ブラーが適用されない範囲セット
	/// </summary>
	/// <param name="mask">ブラーが適用されない範囲</param>
	void SetMask(float mask) { this->mask = mask; }

	/// <summary>
	///	ブラー発生中心位置をセット
	/// </summary>
	/// <param name="blurCenter">ブラー発生中心位置</param>
	void SetBlurCenter(DirectX::XMFLOAT2 blurCenter) { this->blurCenter = blurCenter; }

private: //静的メンバ変数
	static const int32_t texSize = 8;

public: //メンバ変数
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff[texSize];
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> pipelineState[texSize];
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

private: //静的メンバ変数
	//画面クリアカラー
	static const float clearColor[4];

private: //メンバ変数
	int32_t nowPipelineNo = 0;
	float timer = 0.0f;
	//ブラーが適用されない範囲
	float mask;
	//ブラーの中心
	DirectX::XMFLOAT2 blurCenter;
	//定数バッファ転送用
	ComPtr<ID3D12Resource> constBuffB0;

private: //メンバ関数
	/// <summary>
	/// テクスチャ作成
	/// </summary>
	void TexCreate();

	/// <summary>
	/// SRV作成
	/// </summary>
	void SRVCreate();

	/// <summary>
	/// RTV作成
	/// </summary>
	void RTVCreate();

	/// <summary>
	/// 深度バッファ作成
	/// </summary>
	void DepthCreate();

	/// <summary>
	/// DSV作成
	/// </summary>
	void DSVCreate();


};

