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
	MechAnimationContainer();
	~MechAnimationContainer();

	// クリップを追加（overwrite=trueで上書き）
	bool AddClip(const std::string& name, const MechAnimation::Clip& clip, bool overwrite = false);

	// 指定名のクリップを削除
	bool RemoveClip(const std::string& name);

	// 指定名のクリップが存在するか
	bool HasClip(const std::string& name) const;

	// クリップ取得（読み取り専用）
	const MechAnimation::Clip* GetClip(const std::string& name) const;

	// クリップ取得（編集用）
	MechAnimation::Clip* GetClipMutable(const std::string& name);

	// クリップのフレーム配列を取得
	const std::vector<MechAnimation::Pose>* GetFrames(const std::string& name) const;

	// 登録されているクリップ名一覧を取得
	std::vector<std::string> GetClipNames() const;

	// 全クリップを書き出し
	bool SaveAllClips() const;

	// ディレクトリ内の全jsonを読み込んで追加
	bool LoadAllClips(bool overwrite = true);

private:
	// 名前 -> クリップ
	std::unordered_map<std::string, MechAnimation::Clip> clips_;

	const std::string kDirectoryPath_ = "Assets/MechAnimations/";
};
