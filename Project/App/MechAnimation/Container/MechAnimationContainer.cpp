#include "MechAnimationContainer.h"

MechAnimationContainer::MechAnimationContainer() {
	
}

bool MechAnimationContainer::AddClip(const std::string& name, const MechAnimation::Clip& clip, bool overwrite) {
	if (name.empty()) { return false; } // 名前チェック

	auto it = clips_.find(name);
	if (it != clips_.end()) {
		if (!overwrite) { return false; } // 上書き禁止
		it->second = clip; // 上書き
		return true;
	}

	clips_.emplace(name, clip); // 追加
	return true;
}

bool MechAnimationContainer::RemoveClip(const std::string& name) {
	auto it = clips_.find(name);
	if (it == clips_.end()) { return false; } // 存在チェック

	clips_.erase(it); // 削除
	return true;
}

bool MechAnimationContainer::HasClip(const std::string& name) const {
	return clips_.find(name) != clips_.end(); // 存在チェック
}

const MechAnimation::Clip* MechAnimationContainer::GetClip(const std::string& name) const {
	auto it = clips_.find(name);
	if (it == clips_.end()) { return nullptr; } // 取得失敗
	return &it->second; // 参照返し
}

MechAnimation::Clip* MechAnimationContainer::GetClipMutable(const std::string& name) {
	auto it = clips_.find(name);
	if (it == clips_.end()) { return nullptr; } // 取得失敗
	return &it->second; // 参照返し
}

const std::vector<MechAnimation::Pose>* MechAnimationContainer::GetFrames(const std::string& name) const {
	const MechAnimation::Clip* clip = GetClip(name);
	if (!clip) { return nullptr; } // 取得失敗
	return &clip->frames; // フレーム参照
}

std::vector<std::string> MechAnimationContainer::GetClipNames() const {
	std::vector<std::string> names;
	names.reserve(clips_.size()); // 予約

	for (const auto& kv : clips_) {
		names.push_back(kv.first); // 名前追加
	}

	return names; // 返却
}
