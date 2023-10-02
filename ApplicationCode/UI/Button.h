#pragma once
#include "Sprite.h"
#include "ImageManager.h"
#include "MouseInput.h"
#include "Vector2.h"

class Button
{
public: //静的メンバ関数
	/// <summary>
	/// ボタンのインスタンス生成
	/// </summary>
	/// <param name="imageName">画像名</param>
	/// <param name="pos">座標</param>
	/// <param name="size">サイズ</param>
	/// <param name="rot">回転</param>
	/// <returns>インスタンス</returns>
	static Button* CreateButton(ImageManager::ImageName imageName, const Vector2& pos, const Vector2& size, const float rot);

	/// <summary>
	/// ボタンのインスタンス生成
	/// </summary>
	/// <param name="imageName">画像名</param>
	/// <returns>ボタン</returns>
	static std::unique_ptr<Button> CreateUniqueButton(ImageManager::ImageName imageName, const Vector2& pos, const Vector2& size, const float rot);

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Button() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Button() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ImageManager::ImageName imageName, const Vector2& pos, const Vector2& size, const float rot);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Vector2& pos) { pos_ = pos; }

	/// <summary>
	/// サイズをセット
	/// </summary>
	/// <param name="size">サイズ</param>
	void SetSize(const Vector2& size) { size_ = size; }

	/// <summary>
	/// 回転をセット
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRot(const float rot) { rot_ = rot; }

	/// <summary>
	/// アルファ値をセット
	/// </summary>
	/// <param name="alpha">アルファ値</param>
	void SetAlpha(const float alpha) { alpha_ = alpha; }

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetPos() { return pos_; }

	/// <summary>
	/// サイズを取得
	/// </summary>
	/// <returns></returns>
	Vector2 GetSize() { return size_; }

	/// <summary>
	/// 回転を取得
	/// </summary>
	/// <returns></returns>
	float GetRot() { return rot_; }

	/// <summary>
	/// クリック検知フラグを取得
	/// </summary>
	/// <returns>クリック検知フラグ</returns>
	bool GetIsClick() { return isClick_; }

private: //メンバ関数
	/// <summary>
	/// マウスが当たっているかチェック
	/// </summary>
	bool MouseHitCheck();

private: //メンバ変数
	//ボタンスプライト
	std::unique_ptr<Sprite> button_;
	//座標
	Vector2 pos_;
	//サイズ
	Vector2 size_;
	//元サイズ
	Vector2 initSize_;
	//回転
	float rot_;
	//アルファ値
	float alpha_;
	//元アルファ値
	float initAlpha_;
	//クリック検知フラグ
	bool isClick_;
};