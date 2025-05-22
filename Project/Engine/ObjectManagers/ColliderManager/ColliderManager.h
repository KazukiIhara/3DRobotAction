#pragma once

#include <vector>
#include <string>

#include "3D/Colliders3D/BaseCollider3D/BaseCollider3D.h"
#include "3D/Colliders3D/SphereCollider/SphereCollider.h"
#include "3D/Colliders3D/AABBCollider3D/AABBCollider3D.h"

/// <summary>
/// コライダーマネージャ
/// </summary>
class ColliderManager {
public:
	ColliderManager();
	~ColliderManager();

	void Update();

	// 新規コライダー作成
	std::string Create(const std::string& name, Collider3DType colliderType);
	// 指定した名前のコライダーを削除
	void Remove(const std::string& name);
	// 指定し棚前のコライダーを取得
	BaseCollider3D* Find(const std::string& name);
	// コライダーをクリア
	void Clear();
	// 削除フラグが立っているコライダーを削除
	void DeleteGarbages();

	// 生成された全コライダーのリストを取得
	const std::vector<std::unique_ptr<BaseCollider3D>>& GetColliders() const;

private:
	// コライダーコンテナ
	std::vector<std::unique_ptr<BaseCollider3D>> colliders_;
	// コライダーID
	uint32_t currentID_ = 0;
};