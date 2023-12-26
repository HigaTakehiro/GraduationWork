#include "DirectXSetting.h"
#include <cassert>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace Microsoft::WRL;

DirectXSetting* DirectXSetting::GetIns() {
	static DirectXSetting instance;
	return &instance;
}

void DirectXSetting::Initialize(WinApp* winApp) {
	//nullチェック
	assert(winApp);
	//メンバ変数に保存
	this->winApp = winApp;

	//FPS固定初期化処理
	InitializeFixFPS();
	//デバイスの生成
	InitializeDev();
	//コマンド関連の初期化
	InitializeCmd();
	//D3D11デバイスの生成
	InitializeDev11();
	//DirectX2DDeviceContextの生成
	InitializeID2DDeviceContext();
	//スワップチェーンの初期化
	InitializeSwapChain();
	//レンダーターゲットビューの初期化
	InitializeRenderTarget();
	//DirectWriteの描画先の生成
	CreateD2DRenderdTarget();
	//深度バッファの初期化
	InitializeDepthBuffer();
	//フェンスの初期化
	InitializeFence();
}

void DirectXSetting::PreDraw(XMFLOAT4 color) {
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// １．リソースバリアで書き込み可能に変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// ２．描画先指定
	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// ３．画面クリア           R     G     B    A
	float clearColor[] = { color.x, color.y, color.z, color.w }; // 青っぽい色
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポート領域の設定
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));
	// シザー矩形の設定
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));
}

void DirectXSetting::PostDraw() {
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// 命令のクローズ
	cmdList->Close();
	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList.Get() }; // コマンドリストの配列
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ（裏表の入替え）
	//swapchain->Present(1, 0);

	// コマンドリストの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//FPS固定
	UpdateFixFPS();

	cmdAllocator->Reset(); // キューをクリア
	cmdList->Reset(cmdAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
}

void DirectXSetting::InitializeDev() {
	HRESULT result = S_FALSE;

	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;
	for (int32_t i = 0;
		dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter); // 動的配列に追加する
	}

	for (int32_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);  // アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		std::wstring strDesc = adesc.Description;   // アダプター名
		// Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];   // 採用
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int32_t i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
}

void DirectXSetting::InitializeCmd() {
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));

	// コマンドリストを生成
	result = dev->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&cmdList));

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

void DirectXSetting::InitializeDev11()
{
	HRESULT result = S_FALSE;
	//DirectWriteFactoryの生成
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, _uuidof(IDWriteFactory), &directWriteFactory);
	//D3D11デバイスの生成
	ComPtr<ID3D11Device> d3d11Device;
	UINT d3d11DeviceFlags = 0U;

#ifdef _DEBUG
	d3d11DeviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#else
	d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	result = D3D11On12CreateDevice(
		dev.Get(), d3d11DeviceFlags, nullptr, 0,
		reinterpret_cast<IUnknown**>(cmdQueue.GetAddressOf()),
		1, 0, &d3d11Device,
		&devContext11, nullptr
	);

	d3d11Device.As(&id3d11On12Device);
}

void DirectXSetting::InitializeID2DDeviceContext()
{
	HRESULT result = S_FALSE;
	ComPtr<ID2D1Factory3> d2dFactory = nullptr;
	constexpr D2D1_FACTORY_OPTIONS factoryOption{};

	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &factoryOption, &d2dFactory);

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	id3d11On12Device.As(&dxgiDevice);

	ComPtr<ID2D1Device> d2dDevice = nullptr;
	d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.ReleaseAndGetAddressOf());

	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2dDeviceContext.ReleaseAndGetAddressOf());
}

void DirectXSetting::CreateD2DRenderdTarget()
{
	HRESULT result;

	D3D11_RESOURCE_FLAGS flags = { D3D11_BIND_RENDER_TARGET };
	const UINT dpi = GetDpiForWindow(winApp->GetHwnd());
	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			static_cast<float>(dpi));

	for (int32_t i = 0; i < 2; i++) {
		ComPtr<ID3D11Resource> wrappedBackBuffer = nullptr;
		result = id3d11On12Device->CreateWrappedResource(
			backBuffers[i].Get(),
			&flags,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT,
			IID_PPV_ARGS(wrappedBackBuffer.ReleaseAndGetAddressOf()));

		ComPtr<IDXGISurface> dxgiSurface = nullptr;
		wrappedBackBuffer.As(&dxgiSurface);

		ComPtr<ID2D1Bitmap1> d2dRenderTarget = nullptr;
		result = d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProperties, &d2dRenderTarget);

		wrappedBackBuffers.emplace_back(wrappedBackBuffer);
		d2dRenderTargets.emplace_back(d2dRenderTarget);

	}

}

void DirectXSetting::InitializeSwapChain() {
	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 色情報の書式
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapchainDesc.BufferCount = 2;  // バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;

	// スワップチェーンの生成
	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		winApp->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);

	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapchain1.As(&swapchain);
}

void DirectXSetting::InitializeRenderTarget() {
	HRESULT result = S_FALSE;

	// 各種設定をしてデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;    // 裏表の２つ
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	// 裏表の２つ分について
	backBuffers.resize(2);
	for (int32_t i = 0; i < 2; i++)
	{
		// スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		// レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i,
				dev->GetDescriptorHandleIncrementSize(heapDesc.Type)
			)
		);
	}
}

void DirectXSetting::InitializeDepthBuffer() {
	HRESULT result = S_FALSE;

	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WinApp::window_width,
		WinApp::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	// 深度バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	//ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXSetting::InitializeFence() {
	HRESULT result = S_FALSE;

	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void DirectXSetting::InitializeFixFPS() {
	reference = std::chrono::steady_clock::now();
}

void DirectXSetting::UpdateFixFPS() {
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//現在時間の取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	//経過時間の取得
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference);

	if (elapsed < kMinCheckTime) {
		while (std::chrono::steady_clock::now() - reference < kMinTime) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	reference = std::chrono::steady_clock::now();
}

void DirectXSetting::registerFontFormat(const std::string& key, const std::wstring& fontName, const std::wstring& fontPath, const float fontSize)
{
	//テキストフォーマット
	ComPtr<IDWriteTextFormat> textFormat = nullptr;
	//フォントファイル
	ComPtr<IDWriteFontFile> fontFile = nullptr;
	//フォントコレクション
	ComPtr<IDWriteFontCollection1> fontCollection = nullptr;
	//フォントセットビルダー
	ComPtr<IDWriteFontSetBuilder1> fontSetBuilder = nullptr;

	//フォントセットビルダーの生成
	directWriteFactory->CreateFontSetBuilder(fontSetBuilder.GetAddressOf());

	//外部フォントファイル読み込み
	directWriteFactory->CreateFontFileReference(fontPath.c_str(), nullptr, fontFile.GetAddressOf());

	//フォントコレクションの生成
	ComPtr<IDWriteFontSet> fontSet = nullptr;

	fontSetBuilder->AddFontFile(fontFile.Get());
	fontSetBuilder->CreateFontSet(fontSet.GetAddressOf());
	directWriteFactory->CreateFontCollectionFromFontSet(fontSet.Get(), fontCollection.GetAddressOf());

	//テキストフォーマットの作成
	directWriteFactory->CreateTextFormat(
		fontName.c_str(),
		fontCollection.Get(),
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"ja-JP", textFormat.GetAddressOf());

	textFormats[key] = textFormat;
}

void DirectXSetting::beginDrawWithDirect2D()
{
	const auto backBufferIndex = swapchain->GetCurrentBackBufferIndex();
	const auto wrappedBackBuffer = wrappedBackBuffers[backBufferIndex];
	const auto backBufferForD2D = d2dRenderTargets[backBufferIndex];

	id3d11On12Device->AcquireWrappedResources(wrappedBackBuffer.GetAddressOf(), 1);
	d2dDeviceContext->SetTarget(backBufferForD2D.Get());
	d2dDeviceContext->BeginDraw();
	d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

void DirectXSetting::endDrawWithDirect2D()
{
	const auto backBufferIndex = swapchain->GetCurrentBackBufferIndex();
	const auto wrappedBackBuffer = wrappedBackBuffers[backBufferIndex];

	d2dDeviceContext->EndDraw();
	id3d11On12Device->ReleaseWrappedResources(wrappedBackBuffer.GetAddressOf(), 1);
	devContext11->Flush();

	swapchain->Present(1, 0);
}

void DirectXSetting::registerSolidColorBrush(const std::string& key, const D2D1::ColorF color)
{
	ComPtr<ID2D1SolidColorBrush> brush = nullptr;
	d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(color), brush.GetAddressOf());
	solidColorBlushes[key] = brush;
}

void DirectXSetting::registerTextFormat(const std::string& key, const std::wstring& fontName, const float fontSize)
{
	ComPtr<IDWriteTextFormat> textFormat = nullptr;
	directWriteFactory->CreateTextFormat(
		fontName.c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"ja-JP", textFormat.GetAddressOf());

	textFormats[key] = textFormat;
}
