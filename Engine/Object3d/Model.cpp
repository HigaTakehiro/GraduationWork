#include "Model.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* Model::device = nullptr;
UINT Model::descriptorHandleIncrementSize = 0;

void Model::StaticInitialize(ID3D12Device* device) {
	//nullptrチェック
	assert(device);

	Model::device = device;
}

Model* Model::CreateModel(const std::string& modelname, bool isSmoothing) {
	Model* instance = new Model;
	instance->InitializeDescriptorHeap();
	instance->InitializeModel(modelname, isSmoothing);

	return instance;
}

Model* Model::CreateShapeModel(const std::vector<VertexPosNormalUv>& vertices_, const std::vector<unsigned short> indices_, const std::string& textureName)
{
	Model* shape = new Model;
	shape->InitializeDescriptorHeap();
	shape->InitializeShapesModel(vertices_, indices_, textureName);
	return shape;
}

void Model::InitializeShapesModel(const std::vector<VertexPosNormalUv>& vertices_, const std::vector<unsigned short> indices_, const std::string& textureName)
{
	HRESULT result = S_FALSE;

	std::vector<VertexPosNormalUv> realVertices;

	vertices = vertices_;
	indices = indices_;

	material.name = "square";
	material.ambient = { 1.0f, 1.0f, 1.0f };
	material.textureFilename = textureName;
	LoadTexture("Engine/Resources/Images/", material.textureFilename);

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {

		// 全インデックスに対して
		std::copy(indices.begin(), indices.end(), indexMap);

		indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//板ポリ判定フラグを立てる
	isBoardPolygon_ = true;
}

void Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr) {
		std::vector<unsigned short>& v = itr->second;
		XMVECTOR normal = {};
		for (unsigned short index : v) {
			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		for (unsigned short index : v) {
			vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}

void Model::Initialize() {
	HRESULT result;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));
}

void Model::Update(Material materials) {
	HRESULT result;

	ConstBufferDataB1* constMap = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap);
	constMap->ambient = material.ambient;
	constMap->diffuse = material.diffuse;
	constMap->specular = material.specular;
	constMap->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);

	if (isBoardPolygon_) {
		TransferVertex();
	}
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList) {
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void Model::SetAnchorpoint(const DirectX::XMFLOAT2& anchorpoint)
{
	anchorpoint_ = anchorpoint;
}

void Model::SetSize(const DirectX::XMFLOAT2& size)
{
	size_ = size;
	if (texSize_.x == 0.0f && texSize_.y == 0.0f) {
		texSize_ = size;
	}
}

void Model::SetTexRect(const DirectX::XMFLOAT2& texBase, const DirectX::XMFLOAT2& texSize)
{
	texBase_ = texBase;
	texSize_ = texSize;
}

std::wstring Model::SeparatedExtension(const std::wstring& filePath)
{
	size_t pos1;
	std::wstring fileExt;

	//区切り文字'.'が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	if (pos1 != std::wstring::npos) {
		//区切り文字の後ろをファイル拡張子として保存
		fileExt = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
	}
	else {
		fileExt = L"";
	}

	return fileExt;
}

void Model::TransferVertex()
{
	HRESULT result = S_FALSE;

	//   左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	//左右反転
	float left = (0.0f - anchorpoint_.x);
	float right = (1.0f - anchorpoint_.x);
	float top = (0.0f - anchorpoint_.y) * size_.y;
	float bottom = (1.0f - anchorpoint_.y) * size_.y;

	//頂点データ
	VertexPosNormalUv vertex[4];

	vertex[LB].pos = { -left, -bottom, 0.0f }; //左下
	vertex[LT].pos = { -left, -top, 0.0f }; //左上
	vertex[RB].pos = { -right, -bottom, 0.0f }; //右下
	vertex[RT].pos = { -right, -top, 0.0f }; //右上

	//テクスチャ情報取得
	if (texbuff) {
		D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

		float tex_left = texBase_.x / resDesc.Width;
		float tex_right = (texBase_.x + texSize_.x) / resDesc.Width;
		float tex_top = texBase_.y / resDesc.Height;
		float tex_bottom = (texBase_.y + texSize_.y) / resDesc.Height;

		vertex[LB].uv = { tex_left, tex_bottom };
		vertex[LT].uv = { tex_left, tex_top };
		vertex[RB].uv = { tex_right, tex_bottom };
		vertex[RT].uv = { tex_right, tex_top };
	}

	for (int32_t i = 0; i < 4; i++) {
		vertex[i].normal = { 0, 0, 1 };
		vertices[i] = vertex[i];
	}

	//頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	vertMap;
	if (SUCCEEDED(result)) {
		//std::copy(vertices.begin(), vertices.end(), vertMap);
		memcpy(vertMap, vertex, sizeof(vertex));
		vertBuff->Unmap(0, nullptr);
	}
}

void Model::InitializeDescriptorHeap() {
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::InitializeModel(const std::string& modelname, bool isSmoothing) {

	HRESULT result = S_FALSE;

	//ファイルストリーム
	std::ifstream file;
	//objファイルを開く
	//const string modelname = "armor";
	const string filename = modelname + ".obj";
	const string directoryPath = "Engine/Resources/3DModels/obj/" + modelname + "/";
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	vector<XMFLOAT3> positions;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> texcoords;
	//一行ずつ読み込む
	string line;
	while (getline(file, line)) {
		//一行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib") {
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		//先頭文字列がvなら頂点座標
		if (key == "v") {
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt") {
			//UV成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			//XYZ成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がfならポリゴン
		if (key == "f") {
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//頂点インデックス一個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexTexcoord, indexNormal;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;
				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				if (isSmoothing) {
					AddSmoothData(indexPosition, (unsigned short)vertices.size() - 1);
				}
				//頂点インデックスに追加
				indices.emplace_back((unsigned short)indices.size());
			}
		}
	}

	file.close();

	if (isSmoothing) {
		CalculateSmoothedVertexNormals();
	}

	std::vector<VertexPosNormalUv> realVertices;

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {

		// 全インデックスに対して
		std::copy(indices.begin(), indices.end(), indexMap);

		indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Model::LoadTexture(const std::string& directoryPath, const std::string& filename) {
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//ファイルパスを結合
	string filepath = directoryPath + filename;

	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	const std::wstring fileExt = SeparatedExtension(wfilepath);

	if (fileExt == L"dds") {
		result = LoadFromDDSFile(
			wfilepath, DDS_FLAGS_NONE,
			&metadata, scratchImg);

		if (FAILED(result)) {
			assert(0);
		}
	}
	else {
		result = LoadFromWICFile(
			wfilepath, WIC_FLAGS_NONE,
			&metadata, scratchImg);

		if (FAILED(result)) {
			assert(0);
		}
	}

	//ScratchImage mipChain{};

	//ミップマップ生成
	//result = GenerateMipMaps(
	//	scratchImg.GetImages(), scratchImg.GetImageCount(),
	//	scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
	//if (SUCCEEDED(result)) {
	//	scratchImg = std::move(mipChain);
	//	metadata = scratchImg.GetMetadata();
	//}
	
	//読み込んだテクスチャをSRGBとして扱う
	//metadata.format = MakeSRGB(metadata.format);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		assert(0);
	}

	// テクスチャバッファにデータ転送
	for (int32_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出

		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr, // 全領域へコピー
			img->pixels,    // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインサイズ
			(UINT)img->slicePitch // 1枚サイズ
		);
		if (FAILED(result)) {
			assert(0);
		}
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = texresDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = texresDesc.MipLevels;

	device->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename) {
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//一行ずつ読み込む
	string line;
	while (getline(file, line)) {
		//一行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {
			//マテリアル名読み込み
			line_stream >> material.name;
		}

		//先頭文字列がKaならアンビエント色
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}

		//先頭文字列がKdならディフューズ色
		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}

		//先頭文字列がKsならスペキュラー色
		if (key == "Ks") {
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}

		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {
			//テクスチャのファイル名の読み込み
			line_stream >> material.textureFilename;
			//テクスチャ読み込み
			LoadTexture(directoryPath, material.textureFilename);
		}

	}

	file.close();
}