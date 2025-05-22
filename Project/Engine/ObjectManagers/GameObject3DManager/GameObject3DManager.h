#pragma once

#include <map>
#include <string>
#include <memory>

#include "3D/GameObject3D/GameObject3D.h"

/// <summary>
/// 3Dゲームオブジェクトのマネージャー
/// </summary>
class GameObject3DManager {
public:
	GameObject3DManager();
	~GameObject3DManager();

	// 更新
	void Update();
	// 描画
	void Draw();

	// 新規オブジェクトを追加
	std::string Add(std::unique_ptr<GameObject3D> newGameObject);
	// 名前からオブジェクトを検索
	GameObject3D* Find(const std::string& objectName);
	// 削除フラグの立ったオブジェクトを削除する
	void DeleteGarbages();

	void Clear();

private:
	// ゲームオブジェクトコンテナ
	std::map<std::string, std::unique_ptr<GameObject3D>> gameObjects3D_;
};