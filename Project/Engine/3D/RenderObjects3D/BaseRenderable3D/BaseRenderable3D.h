#pragma once

// C++
#include <memory>
#include <optional>
// 
// DirectX
#include <d3d12.h>

// MyHedder
#include "Structs/ObjectStruct.h"
#include "Enums/BlendModeEnum.h"
#include "Enums/Renderer3DEnum.h"

#include "DirectX/ComPtr/ComPtr.h"
#include "3D/Base3D/WorldEntity/WorldEntity.h"
#include "3D/Base3D/WorldTransform/WorldTransform.h"

/// <summary>
/// 3D描画用の基底クラス
/// </summary>
class BaseRenderable3D : public WorldEntity {
public:
	BaseRenderable3D(const std::string& objectName);
	virtual ~BaseRenderable3D() = default;
	virtual void AssignShape() = 0;
	virtual void Update();
	virtual void Draw() = 0;
	virtual void DrawInstanced(uint32_t instanceCount) = 0;

	WorldTransform* GetWorldTransform();

	Vector3& GetScale();
	Vector3& GetRotate();
	Vector3& GetTranslate();

	Material3D& GetMaterial();
	BlendMode& GetBlendMode();
	bool& GetIsShow();

	void SetRenderer3DType(Renderer3DType type);
	Renderer3DType GetType()const;
protected:
	void Initialize(const std::string& objectName);
	// 描画前の処理
	void PrepareForRendering();
	// WVP用のリソース作成
	void CreateWVPResource();
	// データを書き込む
	void MapWVPData();
	// マテリアルリソースの作成
	void CreateMaterialResource();
	// マテリアルデータの書き込み
	void MapMateiralData();

	// WVPデータの更新
	void UpdateWVPData();
	// マテリアルデータの更新
	void UpdateMaterialData();
private:
	const uint32_t cameraRootParamaterIndex_ = 2;
	const uint32_t lightRootParamaterIndex_ = 5;
private:
	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_{};
	// UVトランスフォーム
	UVTransform uvTransform_{};
	// ブレンドモード
	BlendMode blendMode_ = BlendMode::Normal;
	// 描画オブジェクトのタイプ
	std::optional<Renderer3DType> renderer3DType_ = std::nullopt;

	// 描画フラグ
	bool isShow_ = false;
private:
	// WVP用のリソース
	ComPtr<ID3D12Resource> transformationResource_ = nullptr;
	// シェーダーに送るWVPデータ
	TransformationMatrix* transformationData_ = nullptr;

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	// マテリアルデータ
	Material3DForGPU* materialData_ = nullptr;
	// マテリアル
	Material3D material_{};
};