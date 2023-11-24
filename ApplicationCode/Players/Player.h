#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include "Vector3.h"
#include "TextDraw.h"

class Player
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 回転角取得
	/// </summary>
	/// <returns>回転角</returns>
	Vector3 GetRot() { return rot_; }

	/// <summary>
	/// 鉱石取得処理
	/// </summary>
	void AddOreCount() { oreCount_++; }

	/// <summary>
	/// 反発設定
	/// </summary>
	/// <param name="vec"></param>
	void HitHammerToEnemy(Vector3 vec, float dis = 1.f);

	void SetPos(Vector3 pos) { this->pos_ = pos; }

	void SetStop(bool stop) { this->stop_ = stop; }

	bool GetStop() { return stop_; }

	/// <summary>
	/// ハンマー投げフラグを取得
	/// </summary>
	/// <returns>ハンマー投げフラグ</returns>
	bool GetIsHammerRelease() { return isHammerRelease_; }

	/// <summary>
	/// 攻撃フラグを取得
	/// </summary>
	/// <returns>攻撃フラグ</returns>
	bool GetIsAttack() { return isAttack_; }

	/// <summary>
	/// ハンマー回転攻撃フラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsHammerSwing() { return isHammerSwing_; }

	bool GetNotNext() { return notnext_; }

	/// <summary>
	/// 鉱石取得割合取得
	/// </summary>
	/// <returns>鉱石取得割合</returns>
	float GetOreCountRate() { return (float)oreCount_ / (float)maxOreCount_; }

	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns>HP</returns>
	int32_t GetHP() { return hp_; }

	/// <summary>
	/// HPをセット
	/// </summary>
	/// <param name="hp">HP</param>
	void SetHP(int32_t hp) { hp_ = hp; }

	/// <summary>
	/// レベルを取得
	/// </summary>
	/// <returns>レベル</returns>
	int32_t GetLevel() { return level_; }

	/// <summary>
	/// レベルをセット
	/// </summary>
	/// <param name="level">レベル</param>
	void SetLevel(int32_t level) { level_ = level; }

	/// <summary>
	/// 経験値を取得
	/// </summary>
	/// <returns>経験値</returns>
	int32_t GetEP() { return ep_; }

	/// <summary>
	/// HPを加算
	/// </summary>
	/// <param name="addHP">加算するHP</param>
	void AddHP(int32_t addHP) { hp_ += addHP; }

	/// <summary>
	/// 経験値を加算
	/// </summary>
	/// <param name="addEP">加算経験値</param>
	void AddEP(int32_t addEP) { ep_ += addEP; }

	/// <summary>
	/// 経験値をセット
	/// </summary>
	/// <param name="ep">経験値</param>
	void SetEP(int32_t ep) { ep_ = ep; }

	/// <summary>
	/// スプライト描画
	/// </summary>
	void SpriteDraw();

	/// <summary>
	/// HPを減算
	/// </summary>
	/// <param name="subHP">減算するHP</param>
	void SubHP(int32_t subHP) { hp_ -= subHP; }

	void SetNextFlor(bool flag) { nextflor_ = flag; }

	bool GetNextFlor() { return nextflor_; }

	bool GetNext() { return next_; }

	/// <summary>
	/// ハンマー反射フラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsHammerReflect() { return isHammerReflect_; }

	/// <summary>
	/// ハンマー反射フラグをセット
	/// </summary>
	/// <param name="isReflect"></param>
	void SetIsHammerReflect(bool isReflect) { isHammerReflect_ = isReflect; }

	/// <summary>
	/// 文字描画
	/// </summary>
	void TextUIDraw();

	/// <summary>
	/// チュートリアル用アプデ
	/// </summary>
	void TutorialUpdate(bool Stop, bool NotAttack);

	/// <summary>
	/// 鉱石カウントが最大取得量以上かどうかを取得
	/// </summary>
	/// <returns></returns>
	bool OreCountOverMaxCount();


private: //メンバ関数

	/// <summary>
	/// プレイヤーステータスの設定
	/// </summary>
	void PlayerStatusSetting();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// ハンマー投げ
	/// </summary>
	void HammerThrow();

	/// <summary>
	/// ハンマー入手
	/// </summary>
	void HammerGet();

	/// <summary>
	/// ハンマー戻り処理
	/// </summary>
	void HammerReturn();

	/// <summary>
	/// ハンマー強化処理
	/// </summary>
	void HammerPowerUp();

	/// <summary>
	/// 反発処理
	/// </summary>
	void Repulsion();

	/// <summary>
	/// アニメーション処理
	/// </summary>
	void Animation();

	/// <summary>
	/// UI処理
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// レベルアップ処理
	/// </summary>
	void LevelUp();

private: //メンバ変数
	//座標
	Vector3 pos_;
	//大きさ
	Vector3 scale_;
	//回転角
	Vector3 rot_;
	//HP
	int32_t hp_;
	int32_t maxHp_;
	//レベル
	int32_t level_ = 0;
	//経験値
	int32_t ep_;
	//レベルアップ必要経験値
	int32_t levelUpEp_;
	//経験値倍率
	float magEp_;
	//鉱石取得数
	int32_t oreCount_;
	//鉱石最大取得数
	int32_t maxOreCount_;
	//攻撃力
	int32_t attackPoint_;
	//鉱石取得ごとに大きくするサイズ
	Vector3 hammerSizeUp_;
	//鉱石取得ごとの回転速度係数
	float hammerRotCoeff_;

	//移動速度
	float moveSpeed_;
	//回転速度
	float rotSpeed_;
	//回転リセットタイマー
	float rotResetTimer_;
	//回転リセット時間
	float rotResetTime_;
	//初期回転角
	Vector3 initRot_;
	//初期回転速度
	float initRotSpeed_;
	//初期攻撃力
	int32_t initAtkPoint_;
	//最高移動速度
	float maxMoveSpeed_;
	//加速度
	Vector3 acc_;
	//ハンマー攻撃時加速度
	float hammerAcc_;

	//プレイヤーオブジェクト
	std::unique_ptr<Object3d> player_;
	//回転攻撃時プレイヤーオブジェクト
	std::unique_ptr<Object3d> rotAttackPlayer_;
	//プレイヤーモデル
	//通常時
	Model* playerModel_[4];
	//移動モデル
	//前
	Model* frontMoveModel_[4];
	//後
	Model* backMoveModel_[4];
	//右
	Model* rightMoveModel_[4];
	//左
	Model* leftMoveModel_[4];
	//回転攻撃モデル
	Model* rotAttackModel_[4];
	//影オブジェクト
	std::unique_ptr<Object3d> shadow_;
	//影
	Model* shadowModel_;

	//ハンマーオブジェクト
	std::unique_ptr<Object3d> hammer_;
	//ハンマーモデル
	Model* hammerModel_;
	//ハンマー投げフラグ
	bool isHammerRelease_;
	//攻撃フラグ
	bool isAttack_;
	//ハンマー回転フラグ
	bool isHammerSwing_;
	//ハンマー反射フラグ
	bool isHammerReflect_;
	//ハンマー座標
	Vector3 hammerPos_;
	//ハンマーサイズ
	Vector3 hammerSize_;
	//ハンマー投げ速度
	float throwSpeed_;
	//ハンマー投げベクトル
	Vector3 hammerVec_;
	//ハンマー初期値
	Vector3 initHammerPos_;
	Vector3 initHammerScale_;
	Vector3 initHammerRot_;
	//アニメーションカウント
	int32_t animeCount_;
	//アニメーションスピード
	int32_t animeSpeed_;
	//アニメーションタイマー
	int32_t animeTimer_;
	//前フレームアニメーションカウント
	int32_t preAnimeCount_;

	//矢印オブジェクト
	std::unique_ptr<Object3d> arrow_;
	//矢印モデル
	Model* arrowModel_;

	//ハンマー投げ時角度
	float hammerThrowRot_;

	//反発攻撃ベクトル
	Vector3 repulsionVec_;
	//反発攻撃速度
	float repulsionSpeed_;
	//反発係数
	float repulsionPower_;
	//操作を止める
	bool stop_ = false;
	//攻撃中にステージ移動しない様に
	bool notnext_ = false;
	//階段のチェック
	bool nextflor_ = false;

	bool next_ = false;

	//UI
	//HPバー
	std::unique_ptr<Sprite> hpBar_;
	std::unique_ptr<Sprite> hpBarBack_;
	float hpBarSize_;
	//経験値バー
	std::unique_ptr<Sprite> epBar_;
	std::unique_ptr<Sprite> epBarBack_;
	float epBarSize_;
	//テキスト
	TextDraw* text_;

public:
	Object3d* GetHammer() { return hammer_.get(); }
	DirectX::XMFLOAT3 GetPos() { return player_->GetPosition(); }
	DirectX::XMFLOAT3 Get() { return pos_; }
	bool GetisHit() { return player_->GetIsHit(); }
	bool getisHammerActive() {
		if (isHammerRelease_ || isHammerSwing_)return true; return false;
	}
};

