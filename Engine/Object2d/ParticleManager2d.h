#pragma once
#include "Sprite.h"
#include "ImageManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include <forward_list>
#include <string>

class ParticleManager2d
{
public: //構造体
	struct Particle2d {
		//座標
		Vector2 pos_ = { 0, 0 };
		//大きさ
		Vector2 size_ = { 1, 1 };
		//回転
		float rot_ = 0.0f;
		//速度ベクトル
		Vector2 velocity_ = { 0, 0 };
		//加速度
		Vector2 accel_ = { 0, 0 };
		//色
		Vector3 color_ = {1, 1, 1};
		//アルファ値
		float alpha_ = 1.0f;
		//初期パラメータ
		Vector3 start_Color_ = {1, 1, 1};
		Vector2 start_Size_ = {1, 1};
		float start_Rot_ = 0.0f;
		float start_Alpha_ = 1.0f;
		//最終パラメータ
		Vector3 end_Color_ = {1, 1, 1};
		Vector2 end_Size_ = {0, 0};
		float end_Rot_ = 0.0f;
		float end_Alpha_ = 0.0f;
		//現在フレーム
		int32_t nowFrame_;
		//最終フレーム
		int32_t endFrame_;
		//パーティクル画像
		std::unique_ptr<Sprite> particle_;
	};

public: //静的メンバ関数
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static ParticleManager2d* Create();
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<ParticleManager2d> UniquePtrCreate();
	/// <summary>
	/// パーティクル生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<Particle2d> ParticleCreate();

public: //メンバ関数
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクル追加
	/// </summary>
	/// <param name="particle">パーティクル</param>
	void Add(int32_t life, Vector2 position, Vector2 velocity, Vector2 accel, Vector2 start_scale = { 1.0f, 1.0f }, Vector2 end_scale = {1.0f, 1.0f}, Vector3 start_color = { 1.0f, 1.0f, 1.0f }, Vector3 end_color = { 0.0f, 0.0f, 0.0f }, float start_alpha = 1.0f, float end_alpha = 0.0f, ImageManager::ImageName imageName = ImageManager::ImageName::bar);

private: //メンバ変数
	//パーティクルリスト
	std::forward_list<Particle2d> particleList_;
};