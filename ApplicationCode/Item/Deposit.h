#pragma once
#include "Object3d.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Vector3.h"

class Deposit
{
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Deposit();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 pos,bool isDetroy=FALSE,Camera*camera=nullptr);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const Vector3& playerPos);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 鉱石ドロップ方向
	/// </summary>
	/// <returns></returns>
	Vector3 OreDropVec();

	/// <summary>
	/// 鉱床ヒット判定取得
	/// </summary>
	/// <returns>ヒット判定</returns>
	bool GetIsHit(bool isHammerSwing);

	/// <summary>
	/// HP取得
	/// </summary>
	/// <returns>HP</returns>
	int32_t GetHP() { return hp_; }

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <returns>座標</returns>
	Vector3 GetPos() { return deposit_->GetPosition(); }

	/// <summary>
	/// マップの番号をセット
	/// </summary>
	/// <returns></returns>
	void SetMapNum(int Num) { this->mapnum_=Num; }

	/// <summary>
	/// マップの番号を取得
	/// </summary>
	/// <returns></returns>
	int GetMapNum() { return mapnum_; }

private: //メンバ関数
	/// <summary>
	/// アルファ値設定
	/// </summary>
	/// <param name="playerPos"></param>
	void AlphaTest(const Vector3& playerPos);

private: //メンバ変数
	//鉱床オブジェクト
	std::unique_ptr<Object3d> deposit_;
	//鉱床モデル
	Model* model_;
	//HP
	int32_t hp_;
	//無敵時間
	int32_t hitCoolTime_;
	//無敵時間タイマー
	int32_t hitCoolTimer_;
	//マップ番号
	int mapnum_;

private:
	//
	bool isDestroy = FALSE;
	//破壊フラグ
	bool destroyF_ = FALSE;
	//破壊エフェクト
	std::unique_ptr<ParticleManager>desParticle_;
	//パーティクル用
	int32_t particleCreateCool_;//ずる賢いがこうでもしないと参照あっちこっち行って複雑すぎる
	Camera* camera_ = nullptr;
	Vector3 parPos_ = {};
	bool BossDestroy;//鉱石の色
	DirectX::XMFLOAT4 color_={1,1,1,1};
public:

	void SetDestroyBoss(bool f) { BossDestroy = f; }
	void SetCameraIns(Camera* ins) { camera_ = ins; }
	void SetDestroyF(bool f) { /* if(particleCreateCool_ > 60)*/ { destroyF_ = f;/* particleCreateCool_ = 0;*/ } }
	void SetParPos(Vector3 pos) { parPos_ = pos; }
	float GetDepositAlpha()const { return color_.w; }
	void DestroyEffect();
	void ParticleDraw();
};

