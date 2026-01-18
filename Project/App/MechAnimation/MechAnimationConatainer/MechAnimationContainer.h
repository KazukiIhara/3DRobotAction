#pragma once

// C++
#include <string>
#include <unordered_map>
#include <vector>

// 共通アニメ型
#include "MechAnimation/MechAnimation.h"

/// <summary>
/// アニメーション格納コンテナ
/// </summary>
class MechAnimationContainer {
public:
	MechAnimationContainer() = default;
	~MechAnimationContainer() = default;

	bool AddClip(const std::string& name, const MechAnimation::Clip& clip, bool overwrite = false);
	bool RemoveClip(const std::string& name);

	bool HasClip(const std::string& name) const;

	const MechAnimation::Clip* GetClip(const std::string& name) const;
	MechAnimation::Clip* GetClipMutable(const std::string& name);

	const std::vector<MechAnimation::Pose>* GetFrames(const std::string& name) const;

	std::vector<std::string> GetClipNames() const;

private:
	std::unordered_map<std::string, MechAnimation::Clip> clips_;
};
