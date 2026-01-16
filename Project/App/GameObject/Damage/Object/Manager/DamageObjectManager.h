#pragma once

// C++
#include <memory>
#include <vector>

// 攻撃オブジェクトの基底クラス
#include "GameObject/Damage/Object/Base/BaseDamageObject.h"

// 攻撃判定マネージャ
class DamageCollisionManager;

/// <summary>
/// 攻撃オブジェクトマネージャ
/// </summary>
class DamageObjectManger {
public:
	DamageObjectManger(DamageCollisionManager* damageCollisionManager);
	~DamageObjectManger() = default;

	void Update();
	void Draw();

	void AddObject(std::unique_ptr<BaseDamageObject> object);

	const std::vector<BaseDamageObject>& GetObjects();

private:

};