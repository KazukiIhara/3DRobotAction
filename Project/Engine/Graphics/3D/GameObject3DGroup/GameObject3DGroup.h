#pragma once

// C++
#include <list>
#include <unordered_map>

// MyHedder
#include "Structs/GameObject3DStruct.h"
#include "3D/GameObject3D/GameObject3D.h"

/// <summary>
/// ゲームオブジェクトのグループクラス
/// </summary>
class GameObject3DGroup {
public:
	GameObject3DGroup(const std::string& groupName);
	~GameObject3DGroup() = default;

	// オブジェクトを追加
	void AddObject(std::unique_ptr<GameObject3D> newObject);

	// 描画用のレンダラーを追加
	void AddRenderer(std::unique_ptr<BaseRenderable3D> newRenderer);

	// 更新
	void UpdateData();
	// 描画
	void Draw();

	// 寿命切れのオブジェクトを削除
	void DeleteGarbage();

	// 名前を取得
	std::string& GetName();

protected:
	// 描画前処理
	void PrepareForRendering();

private:
	// instancingリソース作成
	void CreateInstancingResource();
	// instancingリソース書き込み
	void MapInstancingData();
	// instancingデータ更新
	void UpdateInstancingData();
	// 描画用データ更新
	void UpdateDrawData();

	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMaterialData();
	// マテリアルデータの更新
	void UpdateMaterialData();

private:
	// 名前
	std::string name_ = "";
	// instance最大数
	const uint32_t kMaxInstance_ = 32768;
	// 受け取るレンダラー3D
	std::unique_ptr<BaseRenderable3D> thisGameObjectRenderer_ = nullptr;
	// 実際に更新するゲームオブジェクトたち
	std::unordered_map<std::string, std::unique_ptr<GameObject3D>> gameObjects_;
	// 描画用のデータリスト
	std::list<GameObject3DData> gameObjectsData_;
	// instance描画する際に使う変数
	uint32_t instanceCount_ = 0;
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;
private:
	// カメラのルートパラメタインデックス番号
	const uint32_t cameraRootParamaterIndex_ = 2;
	// ライトのルートパラメータイデックス番号
	const uint32_t lightRootParamaterIndex_ = 5;
	// instancing描画用リソース
	ComPtr<ID3D12Resource> instancingResource_ = nullptr;
	// instancing描画用データ
	GameObject3DDataForGPU* instancingData_ = nullptr;
	// SrvIndex
	uint32_t srvIndex_ = 0;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	Material3DForGPU* materialData_ = nullptr;
	// マテリアル
	Material3D material_{};
};