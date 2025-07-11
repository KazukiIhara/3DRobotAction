#include "AnimationDataContainer.h"

#include <iostream>
#include <cassert>
#include <filesystem>

#include "Logger/Logger.h"

using namespace MAGIMath;

AnimationDataContainer::AnimationDataContainer() {
	Initialize();
	Logger::Log("AnimationDataContainer Initialize\n");
}

AnimationDataContainer::~AnimationDataContainer() {
	animationDatas_.clear();
	Logger::Log("AnimationDataContainer Finalize\n");
}

void AnimationDataContainer::Initialize() {
	animationDatas_.clear();
}

void AnimationDataContainer::Load(const std::string& animationFileName) {
	// すでに読み込んだファイルかチェック
	auto it = std::find(loadedAnimationFileNames_.begin(), loadedAnimationFileNames_.end(), animationFileName);
	if (it != loadedAnimationFileNames_.end()) {
		// すでに読み込まれていれば早期リターン
		return;
	}

	// アニメーションデータ用意
	AnimationData animation;
	// 対応する拡張子
	std::vector<std::string> supportedExtensions = { ".gltf" };

	// ディレクトリパスを作成
	const std::string animationDirectoryPath = "Assets/Animations/";
	std::string fileDirectoryPath = animationDirectoryPath + animationFileName;

	// ファイル探索
	std::filesystem::path baseDir(fileDirectoryPath);
	std::string animationFilePath;
	for (const auto& entry : std::filesystem::directory_iterator(baseDir)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			if (std::find(supportedExtensions.begin(), supportedExtensions.end(), ext) != supportedExtensions.end()) {
				if (entry.path().stem().string() == animationFileName) {
					animationFilePath = entry.path().string();
					break;
				}
			}
		}
	}

	// ファイルが見つからない場合
	if (animationFilePath.empty()) {
		assert(false && "Not Found Animation File!");
	}

	// Assimpで読み込み
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationFilePath.c_str(), 0);

	// アニメーションが見つからない場合
	if (scene->mNumAnimations == 0) {
		assert(false && "Not Found Animation Data!");
	}

	// すべてのアニメーションを読み取る
	for (unsigned int animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex) {
		aiAnimation* animationAssimp = scene->mAnimations[animIndex];
		AnimationData animationData;

		// アニメーション名設定
		if (animationAssimp->mName.length > 0) {
			animationData.name = animationAssimp->mName.C_Str();
		} else {
			animationData.name = "Unnamed_" + std::to_string(animIndex);
		}

		// アニメーションの長さを設定
		float ticksPerSecond = (animationAssimp->mTicksPerSecond != 0.0)
			? float(animationAssimp->mTicksPerSecond)
			: 25.0f;
		animationData.duration = float(animationAssimp->mDuration / ticksPerSecond);

		// ノードごとのアニメーションを読み取る
		for (unsigned int channelIndex = 0; channelIndex < animationAssimp->mNumChannels; channelIndex++) {
			aiNodeAnim* nodeAnim = animationAssimp->mChannels[channelIndex];
			NodeAnimation& nodeAnimation = animationData.nodeAnimations[nodeAnim->mNodeName.C_Str()];

			// 位置キー
			nodeAnimation.translate.reserve(nodeAnim->mNumPositionKeys);
			for (unsigned int keyIndex = 0; keyIndex < nodeAnim->mNumPositionKeys; keyIndex++) {
				const aiVectorKey& key = nodeAnim->mPositionKeys[keyIndex];
				KeyframeVector3 kf;
				kf.time = float(key.mTime / ticksPerSecond);
				kf.value = { -key.mValue.x, key.mValue.y, key.mValue.z };
				nodeAnimation.translate.push_back(kf);
			}

			// 回転キー
			nodeAnimation.rotate.reserve(nodeAnim->mNumRotationKeys);
			for (unsigned int keyIndex = 0; keyIndex < nodeAnim->mNumRotationKeys; keyIndex++) {
				const aiQuatKey& key = nodeAnim->mRotationKeys[keyIndex];
				KeyframeQuaternion kf;
				kf.time = float(key.mTime / ticksPerSecond);
				kf.value = { key.mValue.x, -key.mValue.y, -key.mValue.z, key.mValue.w };
				nodeAnimation.rotate.push_back(kf);
			}

			// スケールキー
			nodeAnimation.scale.reserve(nodeAnim->mNumScalingKeys);
			for (unsigned int keyIndex = 0; keyIndex < nodeAnim->mNumScalingKeys; keyIndex++) {
				const aiVectorKey& key = nodeAnim->mScalingKeys[keyIndex];
				KeyframeVector3 kf;
				kf.time = float(key.mTime / ticksPerSecond);
				kf.value = { key.mValue.x, key.mValue.y, key.mValue.z };
				nodeAnimation.scale.push_back(kf);
			}
		}

		// アニメーションをコンテナに格納
		animationDatas_[animationFileName] = animationData;
	}

	// 読み込み完了したファイル名をリストに追加
	loadedAnimationFileNames_.push_back(animationFileName);
}

AnimationData AnimationDataContainer::FindAnimationData(const std::string& animationName) {
	// 読み込み済みアニメーションを検索
	if (animationDatas_.contains(animationName)) {
		// 読み込み済みアニメーションを返す
		return animationDatas_.at(animationName);
	}
	assert(false && "Warning: Not Found Animation!!");
	return AnimationData{};
}