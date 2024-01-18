#pragma once
#include "Sprite.h"
#include "TextDraw.h"
#include "Vector2.h"

class SkillPanel
{
public: //メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkillPanel();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="skillName">スキル名</param>
	void Initialize(const std::wstring& skillName, const Vector2& pos);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Vector2 cursorPos);

	/// <summary>
	/// スプライト描画処理
	/// </summary>
	void SpriteDraw();

	/// <summary>
	/// テキスト描画処理
	/// </summary>
	void TextMessageDraw();

	/// <summary>
	/// スキル取得状態を取得
	/// </summary>
	/// <returns>スキル取得状態</returns>
	bool GetIsSkillGet() { return isSkillGet_; }

	/// <summary>
	/// スキル取得状態をセット
	/// </summary>
	void SetIsSkillGet(bool isSkillGet) { isSkillGet_ = isSkillGet; }

	/// <summary>
	/// 取得可能状態を取得
	/// </summary>
	/// <returns>取得可能状態</returns>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// 取得可能状態をセット
	/// </summary>
	void SetIsActive(bool isActive) { isActive_ = isActive; }

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	Vector2 GetPos() { return pos_; }

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const Vector2& pos) { pos_ = pos; }

	/// <summary>
	/// 説明パネル解放処理
	/// </summary>
	/// <param name="cursorPos">カーソル座標</param>
	void TextPanelOpen(Vector2 cursorPos);

private: //メンバ変数
	//スキル名
	std::wstring skillName_;
	//スキル取得フラグ
	bool isSkillGet_;
	//取得可能状態フラグ
	bool isActive_;

	//スキルパネルスプライト
	std::unique_ptr<Sprite> skillPanel_;
	//説明パネルスプライト
	std::unique_ptr<Sprite> textPanel_;
	//説明パネル開放時間
	float panelOpenTimer_;
	//テキスト描画クラス
	TextDraw* text_;
	//座標
	Vector2 pos_;
	//説明パネルサイズ
	Vector2 textPanelSize_;
};

