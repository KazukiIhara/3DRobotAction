#pragma once

#include "3D/Shapes3D/Mesh/Mesh.h"

class SkinningMesh:public Mesh {
public:
	SkinningMesh(const MeshData& meshData);
	~SkinningMesh()override = default;

	void Initialize()override;
	void Draw()override;
	void DrawInstancedForParticle(uint32_t instancedCount)override;

	// スキニング
	void Skinning(const uint32_t& paletteSrvIndex);
	// 影響度の参照を渡す
	std::span<VertexInfluenceJoints>& GetMappdInfluence();
private:
	// 頂点リソースを作成
	void CreateVertexResource()override;

	// スキニング用頂点リソースの作成
	void CreateSkinningVertexResources();
	// スキニング用頂点バッファビュー
	void CreateSkinningVertexBufferView();
	// スキニング情報用のリソース作成
	void CreateSkinningInformationResource();
	// スキニング情報用のデータ書き込み
	void MapSkinningInformationData();

	// スキニング影響度用のリソースを作成
	void CreateInfluenceResource();
private:
	// スキニング用頂点リソース
	ComPtr<ID3D12Resource> skinningVertexResource_;
	// スキニング用VBV
	D3D12_VERTEX_BUFFER_VIEW skinningVertexBufferView_{};

	// スキニング影響度のリソース
	ComPtr<ID3D12Resource> influenceResource_;
	// スキニング影響度
	std::span<VertexInfluenceJoints> mappedInfluence_;

	// スキニング用の情報リソース
	ComPtr<ID3D12Resource> skinningInformationResource_;
	// スキニング用の情報データ
	SkinningInformationForGPU* skiningInformationData_ = nullptr;


	// スキニングに必要なリソースインデックス
	uint32_t vertexSrvIndex_ = 0;
	uint32_t vertexUavIndex_ = 0;
	uint32_t influenceSrvIndex = 0;

};