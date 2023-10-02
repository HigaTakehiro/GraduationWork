#include "WinApp.h"

#pragma comment(lib, "winmm.lib")

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//メッセージで分岐
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); //標準の処理を行う
}

void WinApp::Initialize() {
	//ウィンドウクラスの設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = L"AngelicIron"; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, windowStyle, false); // 自動でサイズ補正

	int display_width = GetSystemMetrics(SM_CXSCREEN);
	int display_height = GetSystemMetrics(SM_CYSCREEN);

	if (display_width == 0) {
		display_width = GetSystemMetrics(CW_USEDEFAULT);
	}
	if (display_height == 0) {
		display_height = GetSystemMetrics(CW_USEDEFAULT);
	}

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName, // クラス名
		L"転血",         // タイトルバーの文字
		windowStyle,        // 標準的なウィンドウスタイル
		display_width / 6,              // 表示X座標（OSに任せる）
		display_height / 6,              // 表示Y座標（OSに任せる）
		wrc.right - wrc.left,       // ウィンドウ横幅
		wrc.bottom - wrc.top,   // ウィンドウ縦幅
		nullptr,                // 親ウィンドウハンドル
		nullptr,                // メニューハンドル
		w.hInstance,            // 呼び出しアプリケーションハンドル
		nullptr);               // オプション

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

	timeBeginPeriod(1);
}

bool WinApp::ProcessMessage() {
	MSG msg{};  // メッセージ

	// メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}

	// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

void WinApp::Finalize() {
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

void WinApp::SetWindowStyle(LONG windowStyle) {
	this->windowStyle |= windowStyle;
}