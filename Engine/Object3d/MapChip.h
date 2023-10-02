#pragma once
#include "Model.h"
#include "Object3d.h"
#include "SafeDelete.h"

class MapChip
{
public: //メンバ関数
	/// <summary>
	/// マップチップの初期化
	/// </summary>
	void MapChipInitialize();

	/// <summary>
	/// マップチップのロード
	/// </summary>
	/// <param name="mapname">csvファイル名</param>
	/// <param name="map_Width">横幅</param>
	/// <param name="map_Height">縦幅</param>
	/// <returns>マップチップ</returns>
	int32_t** MapLoad(const std::string& mapname, int32_t map_width, int32_t map_height);

	/// <summary>
	/// マップチップのセット
	/// </summary>
	/// <param name="map">読み込んだマップチップ</param>
	/// <param name="map_Width">横幅</param>
	/// <param name="map_Height">縦幅</param>
	/// <param name="height">マップの高さ(0なら地面)</param>
	/// <returns>マップチップをオブジェクトにセット</returns>
	std::vector<Object3d*> MapSet(int32_t** map, int32_t map_Width, int32_t map_Height, int32_t height);

	/// <summary>
	/// マップチップの解放
	/// </summary>
	void MapChipFinalize();

private: //メンバ変数
	//ブロック一個分の大きさ
	//const float LAND_SCALE = 30.0f;
	int32_t** map;

	Model* blockModel = nullptr;
};

