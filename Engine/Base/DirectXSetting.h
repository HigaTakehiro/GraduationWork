#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dxgi1_6.h>
#include <dwrite_3.h>
#include <d2d1_1.h>
#include <d2d1_3.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include <DirectXMath.h>
#include <chrono>
#include <thread>
#include <map>
#include "WinApp.h"

class DirectXSetting final
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DirectXSetting() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~DirectXSetting() = default;
	/// <summary>
    /// コピーコンストラクタを禁止
    /// </summary>
	DirectXSetting(const DirectXSetting& obj) = delete;

	/// <summary>
	/// 代入演算子を禁止
	/// </summary>
	DirectXSetting& operator=(const DirectXSetting& obj) = delete;

public: //静的メンバ関数
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static DirectXSetting* GetIns();

private: //エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win);
	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="color">背景色</param>
	void PreDraw(XMFLOAT4 color);
	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();
	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDev() { return dev.Get(); }
	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
	/// <summary>
	/// D2Dデバイス環境の取得
	/// </summary>
	/// <returns>D2Dデバイス環境</returns>
	ID2D1DeviceContext* GetD2DDeviceContext() { return d2dDeviceContext.Get(); }
	/// <summary>
	/// directWriteFactoryの取得
	/// </summary>
	/// <returns>directWriteFactory</returns>
	IDWriteFactory* GetDWriteFactory() { return directWriteFactory.Get(); }
	/// <summary>
	/// SolidColorBrushの取得
	/// </summary>
	/// <returns>SolidColorBrush連想配列</returns>
	std::map<std::string, ComPtr<ID2D1SolidColorBrush>> GetColorBrushes() { return solidColorBlushes; }
	/// <summary>
	/// テキストフォーマットの取得
	/// </summary>
	/// <returns>テキストフォーマット連想配列</returns>
	std::map<std::string, ComPtr<IDWriteTextFormat>> GetTextFormats() { return textFormats; }
	/// <summary>
	/// SolidColorBlushを登録する
	/// </summary>
	/// <param name="key">連想配列のキー</param>
	/// <param name="color">ブラシ色</param>
	void registerSolidColorBrush(const std::string& key, const D2D1::ColorF color);
	/// <summary>
	/// テキストフォーマットを登録する
	/// </summary>
	/// <param name="key">連想配列のキー</param>
	/// <param name="fontName">フォント名</param>
	/// <param name="fontSize">フォントサイズ</param>
	void registerTextFormat(const std::string& key, const std::wstring& fontName, const float fontSize);
	/// <summary>
	/// 外部フォントファイルを登録する
	/// </summary>
	/// <param name="key">連想配列のキー</param>
	/// <param name="fontName">フォント名</param>
	/// <param name="fontPath">フォントのファイルパス</param>
	/// <param name="fontSize">フォントサイズ</param>
	void registerFontFormat(const std::string& key, const std::wstring& fontName, const std::wstring& fontPath, const float fontSize);
	/// <summary>
	/// Direct2D描画開始
	/// </summary>
	void beginDrawWithDirect2D();
	/// <summary>
	/// Direct2D描画終了
	/// </summary>
	void endDrawWithDirect2D();

private: //メンバ変数
	//ウィンドウズアプリケーション管理
	WinApp* winApp;

	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D11On12Device> id3d11On12Device;
	ComPtr<ID3D11DeviceContext> devContext11;
	ComPtr<ID2D1DeviceContext> d2dDeviceContext;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<IDWriteFactory5> directWriteFactory;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	std::vector<ComPtr<ID3D11Resource>> wrappedBackBuffers;
	std::vector<ComPtr<ID2D1Bitmap1>> d2dRenderTargets;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	//ソリッドカラーブラシ
	std::map<std::string, ComPtr<ID2D1SolidColorBrush>> solidColorBlushes;
	//テキストフォーマット
	std::map<std::string, ComPtr<IDWriteTextFormat>> textFormats;
	//FPS固定用記録時間
	std::chrono::steady_clock::time_point reference;

private: //メンバ関数
	/// <summary>
	/// デバイスの初期化
	/// </summary>
	void InitializeDev();
	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void InitializeCmd();
	/// <summary>
	/// DirectX11のデバイスの初期化
	/// </summary>
	void InitializeDev11();
	/// <summary>
	/// ID2Dデバイスコンテキストの初期化
	/// </summary>
	void InitializeID2DDeviceContext();
	/// <summary>
	/// DirectWriteの描画先を生成
	/// </summary>
	void CreateD2DRenderdTarget();
	/// <summary>
	/// スワップチェーンの初期化
	/// </summary>
	void InitializeSwapChain();
	/// <summary>
	/// レンダーターゲットの初期化
	/// </summary>
	void InitializeRenderTarget();
	/// <summary>
	/// 深度バッファの初期化
	/// </summary>
	void InitializeDepthBuffer();
	/// <summary>
	/// フェンスの初期化
	/// </summary>
	void InitializeFence();
	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();
	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();
};

