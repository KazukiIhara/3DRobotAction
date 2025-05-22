#pragma once

// C++
#include <string>

// DirectX
#include <d3d12.h>

// MyHedder
#include "DirectX/ComPtr/ComPtr.h"
#include "2D/WorldTransform2D/WorldTransform2D.h"
#include "Structs/ObjectStruct.h"
#include "Structs/ModelStruct.h"
#include "Enums/BlendModeEnum.h"

/// <summary>
/// 2Dオブジェクトクラス
/// </summary>
class Object2D {
public:
	Object2D(const std::string& objectName, const std::string& textureName);
	~Object2D();
	void Initialize(const std::string& objectName, const std::string& textureName);
	void Update();
	void Draw();

	// 
	// アクセッサ
	// 

	// テクスチャの名前をセット
	void SetTextureName(const std::string& textureName);

	// トランスフォーム
	Vector2& GetPosition();
	Vector2& GetSize();
	Vector2& GetAnchorPoint();
	float& GetRotate();
	float& GetDepth();

	// UVトランスフォーム
	Vector2& GetUVTranslate();
	Vector2& GetUVScale();
	float& GetUVRotate();

	// 生存フラグを取得
	bool& GetIsAlive();
	// 描画フラグを取得
	bool& GetIsShow();

	// マテリアルを取得
	Material2D& GetMaterial();

private:
	// 頂点リソースの作成
	void CreateVertexResource();
	// 頂点バッファビューの作成
	void CreateVretexBufferView();
	// 頂点データの書き込み
	void MapVertexData();

	// 描画用のインデックスリソースを作成
	void CreateIndexResource();
	// インデックスバッファビューの作成
	void CreateIndexBufferView();
	// インデックスリソースにデータを書き込む
	void MapIndexResource();

	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMaterialData();

	// wvp用のリソース作成
	void CreateWVPResource();
	// データを書き込む
	void MapWVPData();
	// テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();
private:
	// 名前
	std::string name_ = "";
	// TextureName
	std::string textureName_;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;

	// トランスフォーム
	WorldTransform2D worldTransform_{};
	// uvTransform
	UVTransform uvTransform_{};

	// アンカーポイント
	Vector2 anchorPoint_ = { 0.0f,0.0f };

	// テクスチャ左上座標
	Vector2 textureLeftTop_ = { 0.0f,0.0f };
	// テクスチャ切り出しサイズ
	Vector2	cutOutSize_ = { 100.0f,100.0f };

	// 左右フリップ
	bool isFlipX_ = false;
	// 上下フリップ
	bool isFlipY_ = false;

	// 生存フラグ
	bool isAlive_ = true;
	// 描画フラグ
	bool isShow_ = true;

private:
	// WVP用のリソース
	ComPtr<ID3D12Resource> transformationResource_ = nullptr;
	// WVPデータ
	Matrix4x4* wvpData_ = nullptr;

	// 頂点リソース
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	// VBV
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	// 頂点データ
	VertexData2D* vertexData_ = nullptr;

	// インデックスリソース
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	// インデックスデータ
	uint32_t* indexData_ = nullptr;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	Material2D* materialData_ = nullptr;
	// マテリアル情報を受け取る箱
	Material2D material_{};
};