#pragma once

// C++
#include <vector>
#include <memory>

// 攻撃コライダー
#include "GameObject/AttackCollider/AttackCollider.h"

// 前方宣言
class MechCore;

/// <summary>
/// 攻撃コリジョンマネージャ
/// </summary>
class AttackCollisionManager {
public:
	AttackCollisionManager();
	~AttackCollisionManager() = default;

	void Update();
	void Draw();

	void AddMech(std::weak_ptr<MechCore> mechCore);
	std::weak_ptr<AttackCollider> AddAttackCollider(std::shared_ptr<AttackCollider> collider);

private:
	// 機体のリスト
	std::vector<std::weak_ptr<MechCore>> mechs_;
	// 攻撃コライダーのリスト
	std::vector<std::shared_ptr<AttackCollider>> attackColliders_;

};