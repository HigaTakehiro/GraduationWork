#pragma once
#include "Sprite.h"
#include "TextDraw.h"
#include "Vector2.h"

class SkillPanel
{
public: //enum
	enum SkillType {
		HammerReturn,
		HPUP,
		ATKUP,
		DEFUP,
		SPDUP,
		FallHammer,
		HyperMode,
		Empty,
		None
	};

public: //メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkillPanel();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="skillName">スキル名</param>
	void Initialize(const std::wstring& skillName, const Vector2& pos, SkillType type, int32_t num = 0);

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
	/// 説明文処理
	/// </summary>
	/// <param name="cursorPos">カーソル座標</param>
	void TextOpen(Vector2 cursorPos);

	/// <summary>
	/// パネルにカーソルが触れているかどうか
	/// </summary>
	/// <param name="cursorPos">カーソル座標</param>
	/// <returns>カーソルがパネルに触れているかどうか</returns>
	bool PanelToCursorHit(Vector2 cursorPos);

	/// <summary>
	/// スキルタイプ取得
	/// </summary>
	/// <returns>スキルタイプ</returns>
	SkillType GetSkillType() { return skillType_; }

	/// <summary>
	/// ステータス上昇数値取得
	/// </summary>
	/// <returns>ステータス上昇数値</returns>
	int32_t GetStatusUpNum() { return num_; }

	/// <summary>
	/// スキル説明文読み込み処理
	/// </summary>
	void SkillTextLoad();

	/// <summary>
	/// スキルタイプをセット
	/// </summary>
	/// <param name="skillType">スキルタイプ</param>
	void SetSkillType(SkillType type);

	/// <summary>
	/// スキルパネル名を取得
	/// </summary>
	/// <returns>スキルパネル名</returns>
	std::wstring GetSkillName() { return skillName_; }

private: //メンバ変数
	//スキル名
	std::wstring skillName_;
	//スキル説明文
	std::wstring skillText_;
	//描画スキル説明文
	std::wstring drawText_;
	//スキル取得フラグ
	bool isSkillGet_;
	//取得可能状態フラグ
	bool isActive_;
	//パネル選択フラグ
	bool isSelect_;

	//スキルパネルスプライト
	std::unique_ptr<Sprite> skillPanel_;
	//テキスト描画クラス
	TextDraw* text_;
	//座標
	Vector2 pos_;
	//ステータス上昇数値
	int32_t num_;
	//スキルタイプ
	SkillType skillType_;
	//説明文表示タイマー
	int32_t drawTimer_;
	int32_t drawTime_;
};

