#pragma once

// C++
#include <string>
#include <unordered_map>
#include <list>

// Assimp
#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>

// MyHedder
#include "Structs/AnimationStruct.h"

/// <summary>
/// アニメーションのデータコンテナ
/// </summary>
class AnimationDataContainer {
public:
	AnimationDataContainer();
	~AnimationDataContainer();

	void Initialize();
	void Load(const std::string& animationFileName);

	AnimationData FindAnimationData(const std::string& animationName);
private:
	// 読み込み済みのファイルコンテナ
	std::list<std::string> loadedAnimationFileNames_;
	// アニメーションデータコンテナ
	std::unordered_map<std::string, AnimationData> animationDatas_;
};