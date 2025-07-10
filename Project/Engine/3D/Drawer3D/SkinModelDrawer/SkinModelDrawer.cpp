#include "SkinModelDrawer.h"

// C++
#include <cassert>

// MyHedder
#include "Logger/Logger.h"
#include "Framework/MAGI.h"
#include "Math/Utility/MathUtility.h"

using namespace MAGIMath;

SkinModelDrawer::SkinModelDrawer(const ModelData& modelData) {
	// モデルのメッシュの数を取得
	const uint32_t meshSize = static_cast<uint32_t>(modelData.meshes.size());

	// メッシュを作成
	for (uint32_t i = 0; i < meshSize; i++) {
		std::unique_ptr<SkinMeshDrawer> newMesh = std::make_unique<SkinMeshDrawer>(modelData.meshes[i]);
		skinMeshes_.push_back(std::move(newMesh));
	}

	// 描画用のリソースを準備
	for (uint32_t i = 0; i < kBlendModeNum; i++) {
		instancingResource_[i] = MAGISYSTEM::CreateBufferResource(sizeof(ModelDataForGPU) * kNumMaxInstance);
		instancingSrvIndex_[i] = MAGISYSTEM::SrvUavAllocate();
		MAGISYSTEM::CreateSrvStructuredBuffer(instancingSrvIndex_[i], instancingResource_[i].Get(), kNumMaxInstance, sizeof(ModelDataForGPU));
		instancingResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_[i]));

		currentIndex_[i] = 0;
		instanceCount_[i] = 0;
	}

	// スケルトンを作成
	skeleton_ = std::make_unique<Skeleton>(modelData.rootNode);

	// palette用のリソースを確保
	paletteResource_ = MAGISYSTEM::CreateBufferResource(sizeof(WellForGPU) * skeleton_->joints.size());
	WellForGPU* mappedPalette = nullptr;
	paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	mappedPalette_ = { mappedPalette,skeleton_->joints.size() };

	// srvのインデックスを割り当て
	paletteSrvIndex_ = MAGISYSTEM::SrvUavAllocate();

	// srvを作成
	MAGISYSTEM::CreateSrvStructuredBuffer(paletteSrvIndex_, paletteResource_.Get(), UINT(skeleton_->joints.size()), sizeof(WellForGPU));

	// InverseBindPoseMatrixの保存領域を作成
	inverseBindPoseMatrices_.resize(skeleton_->joints.size());
	for (size_t i = 0; i < skeleton_->joints.size(); i++) {
		inverseBindPoseMatrices_[i] = MakeIdentityMatrix4x4();
	}

	for (const auto& jointWeight : modelData.skinClusterData) {
		auto it = skeleton_->jointMap.find(jointWeight.first);
		if (it == skeleton_->jointMap.end()) {
			continue;
		}
		inverseBindPoseMatrices_[(*it).second] = jointWeight.second.inverseBindPoseMatrix;

	}

	// Jointの数ループ
	for (const auto& [jointName, jointWeightData] : modelData.skinClusterData) {
		// このループのジョイント
		auto it = skeleton_->jointMap.find(jointName);
		if (it == skeleton_->jointMap.end()) {
			continue;
		}

		// このループのジョイントID
		uint32_t jointId = (*it).second;

		// このループのJointが影響を与える頂点の数ループ
		for (const auto& vertexWeight : jointWeightData.jointToVertexWeights) {
			// Jointが影響を与える頂点がいるメッシュのインデックス
			uint32_t targetMeshIndex = vertexWeight.meshIndex;
			// Jointが影響を与える頂点のインデックス(メッシュ内)
			uint32_t localVertexIndex = vertexWeight.localVertexIndex;
			// Jointが頂点に与える影響度
			float weightValue = vertexWeight.weight;

			// 今回影響を与えられる頂点がいるメッシュ
			auto& targetMesh = skinMeshes_[targetMeshIndex];
			// スキン用メッシュにキャスト
			if (auto* skinMesh = static_cast<SkinMeshDrawer*>(targetMesh.get())) {

				// 今回影響を与えられるメッシュの頂点が持つ影響度具合の参照
				auto& currentInfluence = skinMesh->GetMappdInfluence()[localVertexIndex];

				// kNumMaxInfluence スロットのうち空いているところへ書き込み
				for (uint32_t idx = 0; idx < kNumMaxInfluence; ++idx) {
					if (currentInfluence.weights[idx] == 0.0f) {

						currentInfluence.weights[idx] = weightValue;
						currentInfluence.jointIndices[idx] = jointId;
						break;
					}
				}
			}
		}
	}

}

void SkinModelDrawer::AddDrawCommand(const Matrix4x4& worldMatrix, const ModelMaterial& material) {
	const uint32_t blendIndex = static_cast<uint32_t>(material.blendMode);

#if defined(DEBUG) || defined(DEVELOP)
	if (currentIndex_[blendIndex] >= kNumMaxInstance) {
		Logger::Log("ModelDrawer3D: Max instance count exceeded!\n");
		return;
	}
#endif // _DEBUG

	ModelDataForGPU newModelData{
		.worldMatrix = worldMatrix,
		.WorldInverseTransepose = MakeInverseTransposeMatrix(worldMatrix),
		.color = material.color,
	};

	// コンテナに挿入
	instancingData_[blendIndex][currentIndex_[blendIndex]] = newModelData;
	// インデックスをインクリメント
	currentIndex_[blendIndex]++;
}

void SkinModelDrawer::Update() {
	for (uint32_t i = 0; i < kBlendModeNum; i++) {
		assert(currentIndex_[i] <= kNumMaxInstance);
		instanceCount_[i] = currentIndex_[i];
		currentIndex_[i] = 0;
	}

	// スケルトンの更新
	skeleton_->Update();

	// スキンパレットの更新
	for (size_t jointIndex = 0; jointIndex < skeleton_->joints.size(); ++jointIndex) {
		assert(jointIndex < inverseBindPoseMatrices_.size());

		mappedPalette_[jointIndex].skeletonSpaceMatrix =
			inverseBindPoseMatrices_[jointIndex] * skeleton_->joints[jointIndex].skeletonSpaceMatrix;

		mappedPalette_[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(mappedPalette_[jointIndex].skeletonSpaceMatrix));
	}


	// 各メッシュの描画
	for (auto& mesh : skinMeshes_) {
		mesh->Skinning(paletteSrvIndex_);
	}
}

void SkinModelDrawer::Draw(BlendMode mode) {
	const uint32_t blendIndex = static_cast<uint32_t>(mode);
	if (instanceCount_[blendIndex] == 0) return;
	ID3D12GraphicsCommandList6* commandList = MAGISYSTEM::GetDirectXCommandList6();

	// 
	// パイプラインの設定
	// 
	commandList->SetPipelineState(MAGISYSTEM::GetGraphicsPipelineState(GraphicsPipelineStateType::Model3D, mode));

	// カメラの送信
	MAGISYSTEM::TransferCamera3D(0);
	MAGISYSTEM::TransferCurrentCamera3DFrustum(10);

	// inctancing描画用のデータを送信
	commandList->SetGraphicsRootDescriptorTable(1, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(instancingSrvIndex_[blendIndex]));

	// テクスチャ一覧 (t1000)
	commandList->SetGraphicsRootDescriptorTable(3, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(0));

	// 各メッシュの描画
	for (auto& mesh : skinMeshes_) {
		mesh->Draw(instanceCount_[blendIndex]);
	}
}

void SkinModelDrawer::DrawShadow(BlendMode mode) {
	const uint32_t blendIndex = static_cast<uint32_t>(mode);
	if (instanceCount_[blendIndex] == 0) return;
	ID3D12GraphicsCommandList6* commandList = MAGISYSTEM::GetDirectXCommandList6();

	// 
	// パイプラインの設定
	// 
	commandList->SetPipelineState(MAGISYSTEM::GetShadowPipelineState(ShadowPipelineStateType::Model));

	// ライトのVPを転送　(b0)
	MAGISYSTEM::TransferDirectionalLightCamera(0);
	// ライトカメラのフラスタムを送信
	MAGISYSTEM::TransferDirectionalLightFrustum(8);

	// inctancing描画用のデータを送信
	commandList->SetGraphicsRootDescriptorTable(1, MAGISYSTEM::GetSrvUavDescriptorHandleGPU(instancingSrvIndex_[blendIndex]));

	// 各メッシュの描画
	for (auto& mesh : skinMeshes_) {
		mesh->DrawShadow(instanceCount_[blendIndex]);
	}
}