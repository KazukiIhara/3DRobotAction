#pragma once

// C++
#include <memory>
#include <vector>

// 攻撃オブジェクトの基底クラス
#include "GameObject/Damage/Object/Base/BaseDamageObject.h"

// 攻撃判定マネージャ
class DamageCollisionSystem;

/// <summary>
/// 攻撃オブジェクトマネージャ
/// </summary>
class DamageObjectManager {
public:
	DamageObjectManager(DamageCollisionSystem* collisionSystem);
	~DamageObjectManager() = default;

	void Update();
	void Draw();
	void Add(std::unique_ptr<BaseDamageObject> object);

private:
	void DeleteGarbage();
private:
	// オブジェクトリスト
	std::vector<std::unique_ptr<BaseDamageObject>> objects_;

	// コリジョンマネージャの参照ポインタ
	DamageCollisionSystem* collisionSystem_ = nullptr;
};