#pragma once

// C++
#include <vector>
#include <unordered_map>
#include <memory>

// MyHedder
#include "GameObject3D/GameObject3D.h"

/// <summary>
/// 3Dゲームオブジェクトマネージャー
/// </summary>
class GameObject3DManager {
public:
	GameObject3DManager();
	~GameObject3DManager();

	std::weak_ptr<GameObject3D> Add(std::shared_ptr<GameObject3D> gameObject3D, bool insertMap);
	std::weak_ptr<GameObject3D> Find(const std::string& objectName);
	void DeleteGarbage();
	void Clear();
	void ClearWithOutUnique();

private:
	std::vector<std::shared_ptr<GameObject3D>> gameObjects_;
	std::unordered_map<std::string, std::weak_ptr<GameObject3D>> gameObjectList_;
};