#pragma once

#include "3D/Shapes3D/Model/Model.h"
#include "3D/Shapes3D/SkinningMesh/SkinningMesh.h"

class SkinningModel:public Model {
public:
	SkinningModel(const ModelData& modeldata);
	~SkinningModel()override;

	void Initialize()override;

	void Update()override;

	// アニメーションの適用
	void ApplyAnimation(const AnimationData& animation, float animationTime);

private:
	// メッシュ生成
	void CreateMeshes()override;

	// パレットのリソース作成
	void CreateSkinPaletteResource();
	// InverseBindPoseMatrixの作成
	void CreateInverseBindPoseMatrix();
	// 全メッシュにスキニング影響度を設定
	void SettingInfluenceAllMeshes();
	// パレットの更新
	void SkinPaletteUpdate();

private:
	// スケルトン
	std::unique_ptr<Skeleton> skeleton_ = nullptr;

	// パレットのリソース
	ComPtr<ID3D12Resource> paletteResource_ = nullptr;
	std::span<WellForGPU> mappedPalette_;
	uint32_t paletteSrvIndex_ = 0;

	// 逆バインドポーズ
	std::vector<Matrix4x4> inverseBindPoseMatrices_;
};
