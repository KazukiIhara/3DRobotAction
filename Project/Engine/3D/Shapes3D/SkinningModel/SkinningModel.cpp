#include "SkinningModel.h"

#include "Framework/MAGI.h"

#include "MAGIUitility/MAGIUtility.h"
#include <iostream>

using namespace MAGIMath;
using namespace MAGIUtility;

SkinningModel::SkinningModel(const ModelData& modeldata)
	:Model(modeldata) {
}

SkinningModel::~SkinningModel() {
	skeleton_.reset();
}

void SkinningModel::Initialize() {
	CreateMeshes();
	skeleton_ = std::make_unique<Skeleton>(modelData_.rootNode);
	CreateSkinPaletteResource();
	CreateInverseBindPoseMatrix();
	SettingInfluenceAllMeshes();
}

void SkinningModel::Update() {
	// スケルトンの更新
	skeleton_->Update();
	// パレットの更新
	SkinPaletteUpdate();

	// 全メッシュの更新
	Model::Update();

	// スキニング
	for (auto& mesh : meshes_) {
		// キャスト
		if (auto* skinMesh = static_cast<SkinningMesh*>(mesh.get())) {
			skinMesh->Skinning(paletteSrvIndex_);
		}
	}
}

void SkinningModel::ApplyAnimation(const AnimationData& animation, float animationTime) {
	for (Joint& joint : skeleton_->joints) {
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate, animationTime);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate, animationTime);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale, animationTime);
		}
	}
}

void SkinningModel::CreateMeshes() {
	for (uint32_t i = 0; i < modelData_.meshes.size(); i++) {
		std::unique_ptr<Mesh> newMesh = std::make_unique<SkinningMesh>(modelData_.meshes[i]);
		newMesh->Initialize();
		meshes_.push_back(std::move(newMesh));
	}
}

void SkinningModel::CreateSkinPaletteResource() {
	// palette用のリソースを確保
	paletteResource_ = MAGISYSTEM::CreateBufferResource(sizeof(WellForGPU) * skeleton_->joints.size());
	WellForGPU* mappedPalette = nullptr;
	paletteResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	mappedPalette_ = { mappedPalette,skeleton_->joints.size() };

	// srvのインデックスを割り当て
	paletteSrvIndex_ = MAGISYSTEM::SrvUavAllocate();

	// srvを作成
	MAGISYSTEM::CreateSrvStructuredBuffer(paletteSrvIndex_, paletteResource_.Get(), UINT(skeleton_->joints.size()), sizeof(WellForGPU));
}

void SkinningModel::CreateInverseBindPoseMatrix() {
	// InverseBindPoseMatrixの保存領域を作成
	inverseBindPoseMatrices_.resize(skeleton_->joints.size());
	for (size_t i = 0; i < skeleton_->joints.size(); i++) {
		inverseBindPoseMatrices_[i] = MakeIdentityMatrix4x4();
	}

	for (const auto& jointWeight : modelData_.skinClusterData) {
		auto it = skeleton_->jointMap.find(jointWeight.first);
		if (it == skeleton_->jointMap.end()) {
			continue;
		}
		inverseBindPoseMatrices_[(*it).second] = jointWeight.second.inverseBindPoseMatrix;

	}
}

void SkinningModel::SettingInfluenceAllMeshes() {

	// Jointの数ループ
	for (const auto& [jointName, jointWeightData] : modelData_.skinClusterData) {
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
			auto& targetMesh = meshes_[targetMeshIndex];
			// スキン用メッシュにキャスト
			if (auto* skinMesh = static_cast<SkinningMesh*>(targetMesh.get())) {

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

void SkinningModel::SkinPaletteUpdate() {
	for (size_t jointIndex = 0; jointIndex < skeleton_->joints.size(); ++jointIndex) {
		assert(jointIndex < inverseBindPoseMatrices_.size());

		mappedPalette_[jointIndex].skeletonSpaceMatrix =
			inverseBindPoseMatrices_[jointIndex] * skeleton_->joints[jointIndex].skeletonSpaceMatrix;

		mappedPalette_[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(mappedPalette_[jointIndex].skeletonSpaceMatrix));
	}
}
