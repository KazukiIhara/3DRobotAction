#pragma once

// C++
#include <memory>
#include <vector>

// 攻撃オブジェクトの基底クラス
#include "Feature/Damage/Object/Base/BaseDamageObject.h"

/// <summary>
/// 攻撃オブジェクトマネージャ
/// </summary>
class DamageObjectManager {
public:
	DamageObjectManager();
	~DamageObjectManager() = default;

	void Update();
	void Draw();
	void Add(std::unique_ptr<BaseDamageObject> object);

private:
	void DeleteGarbage();
private:
	// オブジェクトリスト
	std::vector<std::unique_ptr<BaseDamageObject>> objects_;

};