#pragma once

#include <map>
#include <string>
#include <memory>

#include "3D/GameObject3DGroup/GameObject3DGroup.h"

/// <summary>
/// 3Dゲームオブジェクトグループのマネージャ
/// </summary>
class GameObject3DGroupManager {
public:
	GameObject3DGroupManager();
	~GameObject3DGroupManager();

	// 更新
	void Update();
	// 描画
	void Draw();

	// 新規オブジェクトグループを追加
	std::string Add(std::unique_ptr<GameObject3DGroup> newGameObjectGroup);
	// 名前からオブジェクトグループを検索
	GameObject3DGroup* Find(const std::string& objectGroupName);
	// 削除フラグの立ったオブジェクトグループを削除する	
	void DeleteGarbages();

	void Clear();

private:
	// ゲームオブジェクトグループコンテナ
	std::map<std::string, std::unique_ptr<GameObject3DGroup>> gameObject3DGroups_;
};

